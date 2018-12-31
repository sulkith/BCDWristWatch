
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
	DDRD=0xFF;
	for(uint8_t i = 0;i<(sizeof(shifting)-3);++i)
	{
		DisplayBuffer[0] = shifting[i+0];
		DisplayBuffer[1] = shifting[i+1];
		DisplayBuffer[2] = shifting[i+2];
		DisplayBuffer[3] = shifting[i+3];
		showLEDs(100);
	}
}

void readEEP()
{
	correction.everyMinute = (int8_t)eeprom_read_byte((uint8_t*)&(correction_EEP.everyMinute));
	correction.everyHour = (int8_t)eeprom_read_byte((uint8_t*)&(correction_EEP.everyHour));
	correction.everyDay = (int8_t)eeprom_read_byte((uint8_t*)&(correction_EEP.everyDay));
	correction.everyMonth = (int8_t)eeprom_read_byte((uint8_t*)&(correction_EEP.everyMonth));
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

int main(void)
{
	TestAllLEDs();

	readEEP();
	showEEPValues();

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
				if(rightButton.valueUpdated()==1 && rightButton.getValue()==1)
				{
					ontime = 0;
					State = set_minute;
				}
				if(leftButton.valueUpdated()==1 && leftButton.getValue()==1)
				{
					ontime = 0;
					t.hour = (t.hour+1)%24;
				}
				break;
			case set_minute:
				max_ontime = ontime_long;
				if(rightButton.valueUpdated()==1 && rightButton.getValue()==1)
				{
					ontime = 0;
					t.second = 0;
					State = display_on;
				}
				if(leftButton.valueUpdated()==1 && leftButton.getValue()==1)
				{
					ontime = 0;
					t.second = 0;
					t.minute = (t.minute+1)%60;
				}
				if(leftButton.getValue() == 1 && rightButton.getValue() == 1)
				{
					//further settings
					ontime=max_ontime+1;
					TestAllLEDs();
				}
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
	DDRB =0xFE;
	PORTB=0x01;
	DDRC = 0x00;											//Configure all eight pins of port B as outputs
	PORTC = 0x10;											//Configure all eight pins of port B as outputs
	_delay_us(15000);	//Wait for external clock crystal to stabilize;
	for (uint8_t i=0; i<0x40; i++)
	{
		for (uint32_t j=0; j<0xFFFF; j++);
	}
	TIMSK2 &= ~((1<<TOIE2)|(1<<OCIE2A)|(1<<OCIE2B));						//Make sure all TC0 interrupts are disabled
	ASSR |= (1<<AS2);										//set Timer/counter0 to be asynchronous from the CPU clock
															//with a second external clock (32,768kHz)driving it.
	TCNT2 =0;												//Reset timer
	TCCR2B =(1<<CS00)|(1<<CS02);								//Prescale the timer to be clock source/128 to make it
															//exactly 1 second for every overflow to occur
	while(ASSR&((1<<TCN2UB)|(1<<OCR2BUB)|(1<<TCR2BUB)|(1<<OCR2AUB)|(1<<TCR2AUB)));	//Wait until TC0 is updated
	TIMSK2 |= (1<<TOIE2);									//Set 8-bit Timer/Counter0 Overflow Interrupt Enable

	//enable PinChangeInterrupt
	PCICR |= (1<<PCIE0)|(1<<PCIE1);
	PCMSK0|= 0x01; //PCINT0
	PCMSK1|= 0x10; //PCINT12

	sei();													//Set the Global Interrupt Enable Bit
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);					//Selecting power save mode as the sleep mode to be used
	sleep_enable();											//Enabling sleep mode
}

ISR(PCINT0_vect)
{
	if(getLeftButton())
		wakeupTriggered=1;
}
ISR(PCINT1_vect)
{
	if(getRightButton())
		wakeupTriggered=2;
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
