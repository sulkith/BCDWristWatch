
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "types.h"
#include "settings.h"
#include "constants.h"
#include "Debouncer.h"


extern correction_t EEMEM correction_EEP;

static void init(void);
static void updateDisplayBuffer(void);
uint8_t getRightButton();
uint8_t getLeftButton();

time t = {30,59,23,0};
enum state_t setting = idle;
enum state_t State = idle;
correction_t correction={0,4,4};
uint8_t DisplayBuffer[4];
uint8_t wakeupTriggered=0;
Debouncer<uint8_t> rightButton(Button_debounceTime,0,&getRightButton);
Debouncer<uint8_t> leftButton(Button_debounceTime,0,&getLeftButton);

inline void setupPorts()
{
	DDRB = 0xFE;
	PORTB = 0x01;
	DDRC = 0x00;
	PORTC = 0x10;
	DDRD=0xFF;
}
inline void setupPCINT()
{
	//enable PinChangeInterrupt
	PCMSK0|= 0x01; //PCINT0
	PCMSK1|= 0x10; //PCINT12
}
inline void activatePCINT()
{
	//enable PinChangeInterrupt
	PCICR |= (1<<PCIE0)|(1<<PCIE1);
}
inline void deactivatePCINT()
{
	PCICR &= ~((1<<PCIE0)|(1<<PCIE1));
}
inline void setupTimer0()
{
	//_delay_us(15000);	//Wait for external clock crystal to stabilize;//Unnecessary since init is called after reading EEP

	TIMSK2 &= ~((1<<TOIE2)|(1<<OCIE2A)|(1<<OCIE2B));						//Make sure all TC0 interrupts are disabled
	ASSR |= (1<<AS2);										//set Timer/counter0 to be asynchronous from the CPU clock
	//with a second external clock (32,768kHz)driving it.
	TCNT2 =0;												//Reset timer
	TCCR2B =(1<<CS00)|(1<<CS02);								//Prescale the timer to be clock source/128 to make it
	//exactly 1 second for every overflow to occur
	while(ASSR&((1<<TCN2UB)|(1<<OCR2BUB)|(1<<TCR2BUB)|(1<<OCR2AUB)|(1<<TCR2AUB)));	//Wait until TC0 is updated
	TIMSK2 |= (1<<TOIE2);									//Set 8-bit Timer/Counter0 Overflow Interrupt Enable
}

uint8_t getRightButton()
{
	if((PINC&0x10)==0)
		return 1;
	else
		return 0;
}

uint8_t getLeftButton()
{
	if((PINB&0x01)==0)
		return 1;
	else
		return 0;
}

void showLEDs(uint16_t duration)
{
		const uint8_t perc = 100;
		for(uint16_t j = 0; j < duration; ++j)
			for(uint8_t i = 0; i < 4; ++i)
			{
				PORTD=DisplayBuffer[i]|((0b00111100)&(~(1<<(2+i))));
				_delay_us(perc);
				PORTD=0;
				_delay_us(250-perc);
			}
}

void TestAllLEDs()
{
	for(uint8_t i = 0;i<(sizeof(shifting)-3);++i)
	{
		DisplayBuffer[0] = shifting[i+0];
		DisplayBuffer[1] = shifting[i+1];
		DisplayBuffer[2] = shifting[i+2];
		DisplayBuffer[3] = shifting[i+3];
		showLEDs(100);
	}
}

void writeDataToEEPROM()
{
	if(correction.everyMinute>59 || correction.everyHour>59 || correction.everyDay>23)
	{
		//unplausible Data
		correction.everyMinute = 0;
		correction.everyHour = 0;
		correction.everyDay= 0;
		correction.everyMonth= 0;
	}
	eeprom_update_block(&correction,&correction_EEP,sizeof(correction_t));
}
void readEEP()
{
	eeprom_read_block(&correction,&correction_EEP,sizeof(correction_t));
	if(correction.everyMinute>59 || correction.everyHour>59 || correction.everyDay>23)
	{
		//unplausible Data
		correction.everyMinute = 0;
		correction.everyHour = 0;
		correction.everyDay= 0;
		correction.everyMonth= 0;
	}
}
void showEEPValues()
{
	DisplayBuffer[0]=numToPortD[correction.everyMinute&0x0F];
	DisplayBuffer[1]=numToPortD[correction.everyHour&0x0F];
	DisplayBuffer[2]=numToPortD[correction.everyDay&0x0F];
	DisplayBuffer[3]=numToPortD[correction.everyMonth&0x0F];
	showLEDs(1000);
}
template <typename T>
uint8_t caseForAdjusting(T* const value, uint16_t* const ontime, const T maxvalue, const T minvalue=0)
{
	if(rightButton.valueUpdatedTo(1)==1)
	{
		if((*value)==minvalue)
			(*value)=maxvalue;
		else
			(*value)--;
		*ontime = 0;
		if(leftButton.getValue() == 1 && rightButton.getValue() == 1)
		{
			*ontime = 0;
			return 1;
		}
	}
	if(leftButton.valueUpdatedTo(1)==1)
	{
		if((*value)==maxvalue)
			(*value)=minvalue;
		else
			(*value)++;
		*ontime = 0;
		if(leftButton.getValue() == 1 && rightButton.getValue() == 1)
		{
			*ontime = 0;
			return 1;
		}
	}
	return 0;
}
int main(void)
{

  init();	//Initialize registers and configure RTC.

	const uint16_t dT = 1;
	uint16_t ontime = 0;
	uint16_t max_ontime = ontime_short;
	while(1)
	{
		switch(State)
		{
			case idle:
				if(wakeupTriggered>0)//wakeup on Keypress
				{
					wakeupTriggered=0;
					rightButton.directSetValue(0);
					leftButton.directSetValue(0);
					ontime = 0;
					State = display_on;
				}
				else
				{
					wakeupTriggered=0;
					activatePCINT();
					sleep_mode();
					break;
				}
				break;
			case display_on:
				max_ontime = ontime_short;
				if(leftButton.getValue() == 1 && rightButton.getValue() == 1)
				{
					ontime = 0;
					State = set_hour;
				}
				if(leftButton.getValue() == 1 || rightButton.getValue() == 1)
				{
					ontime = 0;
				}
				break;
			case set_hour:
				max_ontime = ontime_long;
				if(rightButton.valueUpdatedTo(1)==1)
				{
					ontime = 0;
					State = set_minute;
				}
				if(leftButton.valueUpdatedTo(1)==1)
				{
					ontime = 0;
					t.hour = (t.hour+1)%24;
				}
				break;
			case set_minute:
				max_ontime = ontime_long;
				if(rightButton.valueUpdatedTo(1)==1)
				{
					ontime = 0;
					t.second = 0;
					State = display_on;
				}
				if(leftButton.valueUpdatedTo(1)==1)
				{
					ontime = 0;
					t.second = 0;
					t.minute = (t.minute+1)%60;
				}
				if(leftButton.getValue() == 1 && rightButton.getValue() == 1)
				{
					//further settings
					State=set_eep_min_correction;
					//ontime=max_ontime+1;
					//TestAllLEDs();
				}
				break;
			case set_eep_min_correction:
				if(caseForAdjusting<uint8_t>(&(correction.everyMinute),&ontime,60))State = set_eep_hour_correction;
				break;
			case set_eep_hour_correction:
				if(caseForAdjusting<uint8_t>(&(correction.everyHour),&ontime,60))State = set_eep_day_correction;
				break;
			case set_eep_day_correction:
				if(caseForAdjusting<uint8_t>(&(correction.everyDay),&ontime,24))State = set_eep_month_correction;
				break;
			case set_eep_month_correction:
				if(caseForAdjusting<uint8_t>(&(correction.everyMonth),&ontime,30))State = save_eeprom;
				break;
			case save_eeprom:
				writeDataToEEPROM();
				State = display_on;
				ontime = 0;
				break;
			case read_temperature:
				//empty
				TestAllLEDs();
				State = idle;
				break;
		}
		if((State&0xF0) != 0x00)
		{
			updateDisplayBuffer();
			showLEDs(dT);
			rightButton.loop(dT);
			leftButton.loop(dT);
			if((State&0xF0) == 0x10)max_ontime = ontime_short;
			if((State&0xF0) == 0x20)max_ontime = ontime_long;
			if((State&0xF0) == 0x30)max_ontime = 65535;
			if(ontime > max_ontime)
			{
				State = idle;
				wakeupTriggered=0;
			}
			else
			{
				ontime += dT;
			}
		}
	}
}
static void init(void)
{
	setupPorts();
	TestAllLEDs();
	readEEP();
	showEEPValues();
	setupPCINT();
	activatePCINT();
	setupTimer0();

	sei();													//Set the Global Interrupt Enable Bit
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);					//Selecting power save mode as the sleep mode to be used
	sleep_enable();											//Enabling sleep mode
}

ISR(PCINT0_vect)
{
	if(getLeftButton())
	{
		wakeupTriggered=1;
		deactivatePCINT();
	}
}
ISR(PCINT1_vect)
{
	if(getRightButton())
	{
		wakeupTriggered=2;
		deactivatePCINT();
	}
}
ISR(TIMER2_OVF_vect)
{
	if (++t.second==60)        //keep track of time, date, month, and year
	{
		t.second=correction.everyMinute;
		if (++t.minute==60)
		{
			t.minute=0;
			t.second+=correction.everyHour;
			wakeupTriggered=0x04;//show Time every Hour
			if (++t.hour==24)
			{
				t.hour=0;
				t.second+=correction.everyDay;
				if(++t.day==30)
				{
					t.second+=correction.everyMonth;
					t.day=0;
				}
				if(t.second>59)
				{
					t.minute+=t.second/60;
					t.second=t.second%60;
				}
				//updateDate();
			}
		}
	}
}
static void updateDisplayBuffer(void)
{
	if((State&0xF0)<0x30)
	{
		DisplayBuffer[3]=numToPortD[t.hour/10];
		DisplayBuffer[2]=numToPortD[t.hour%10];
		DisplayBuffer[1]=numToPortD[t.minute/10];
		DisplayBuffer[0]=numToPortD[t.minute%10];
		if(t.hour==0 && t.minute==0)
		{
			DisplayBuffer[3]=numToPortD[2];
			DisplayBuffer[2]=numToPortD[4];
			DisplayBuffer[1]=numToPortD[0];
			DisplayBuffer[0]=numToPortD[0];
		}
		if(State==set_hour)DisplayBuffer[3]|=0x01;
		if(State==set_minute)DisplayBuffer[1]|=0x01;
	}
	else if((State&0xF0)==0x30)
	{
		DisplayBuffer[3]=numToPortD[(State&0x0F)];//Display Setting number in first Row
		DisplayBuffer[2]=DISP_F;//Display 0xF o signalize settings mode
		uint8_t valueToDisplay = 0;
		switch (State) {
			case set_eep_min_correction:
				valueToDisplay = correction.everyMinute;
				break;
			case set_eep_hour_correction:
				valueToDisplay = correction.everyHour;
				break;
			case set_eep_day_correction:
				valueToDisplay = correction.everyDay;
				break;
			case set_eep_month_correction:
				valueToDisplay = correction.everyMonth;
				break;
			case save_eeprom:
				valueToDisplay=0x00;
				break;
			case idle:
			case set_minute:
			case set_hour:
			case read_temperature:
			case display_on:
				//Will not happen
				break;
		}
		DisplayBuffer[0] = numToPortD[(valueToDisplay&0x0F)];
		DisplayBuffer[1] = numToPortD[((valueToDisplay>>4)&0x0F)];
	}

}
