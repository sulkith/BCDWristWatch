
#define ANALOG_CLOCK

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "types.h"
#include "settings.h"
#include "Debouncer.h"
#include "OnChipTempSensor.h"
#ifdef ANALOG_CLOCK
	#include "charlieArrays.h"
#else
#endif
	#include "constants.h"


extern correction_t EEMEM correction_EEP;
extern uint16_t EEMEM tempCorrectionOffset_EEP;

static void init(void);
static void updateDisplayBuffer(void);
uint8_t getRightButton();
uint8_t getLeftButton();

time t = {30,59,23,0};
enum state_t setting = idle;
enum state_t State = idle;
correction_t correction={0,4,4};
const uint16_t tempCorrection[]={	0,	1,	5,	10,	17,	26,	37,	50,	65,	82,	100,	121,	144,	168,	195,	223,	254,	286,	320,	356,	394,	434,	476,	520,	566,	614,	664,	715,	769,	824,	882,	941,	1003,	1066,	1131,	1198,	1267,	1338,	1411,	1486,	1563,	1641,	1722,	1805,	1889,	1976,	2064,	2154,	2247,	2341};
uint16_t tempCorrectionOffset = 345;
uint8_t DisplayBuffer[5];
uint8_t wakeupTriggered=0;
Debouncer<uint8_t> rightButton(Button_debounceTime,0,&getRightButton);
Debouncer<uint8_t> leftButton(Button_debounceTime,0,&getLeftButton);
OnChipTempSensor octs;

#ifndef ANALOG_CLOCK
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
inline void activatePCINT()
{
	//enable PinChangeInterrupt
	PCICR |= (1<<PCIE0)|(1<<PCIE1);
}
inline void deactivatePCINT()
{
	PCICR &= ~((1<<PCIE0)|(1<<PCIE1));
}
#else
inline void setupPorts()
{
	//BT1 --> PB3 --> left
	//BT2 --> PB4 --> right
	DDRB = 0x00;
	PORTB = pbmask;
}
inline void setupPCINT()
{
	//enable PinChangeInterrupt
	PCMSK0|= 0x18; //PCINT3 PCINT4
	//PCMSK1|= 0x10; //PCINT12
}
uint8_t getRightButton()
{
	if((PINB&0x10)==0)
		return 1;
	else
		return 0;
}
uint8_t getLeftButton()
{
	if((PINB&0x08)==0)
		return 1;
	else
		return 0;
}
inline void activatePCINT()
{
	//enable PinChangeInterrupt
	PCICR |= (1<<PCIE0);
}
inline void deactivatePCINT()
{
	PCICR &= ~((1<<PCIE0));
}

#endif
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


void showLEDs(uint16_t duration)
{
		const uint8_t perc = LED_Brightness;
		for(uint16_t j = 0; j < (duration*DisplayBuffer[4]); ++j)
			//for(uint8_t i = 0; i < 4; ++i)
			{
				uint8_t i = j%DisplayBuffer[4];
				const uint8_t k=DisplayBuffer[i];
				if(k<30)
				{
					DDRB  = dbmin[k];
					PORTB = pbmin[k]|pbmask;
					DDRC  = dcmin[k];
					PORTC = pcmin[k];
					DDRD  = ddmin[k];
					PORTD = pdmin[k];
				}
				else if(k<42)
				{
					DDRB  = dbh[k-30];
					PORTB = pbh[k-30]|pbmask;
					DDRC  = dch[k-30];
					PORTC = pch[k-30];
					DDRD  = ddh[k-30];
					PORTD = pdh[k-30];
				}
				else
				{
					DDRB  = 0;
					PORTB = pbmask;
					DDRC  = 0;
					PORTC = 0;
					DDRD  = 0;
					PORTD = 0;
				}

				_delay_us(perc);
				DDRB  = 0;
				PORTB = pbmask;
				DDRC  = 0;
				PORTC = 0;
				DDRD  = 0;
				PORTD = 0;
				_delay_us(250-perc-50);//50 as correction value
			}
}

#ifndef ANALOG_CLOCK
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
#else
void fadeIn()
{
	DisplayBuffer[4] = 3;//Maximum 3 LEDs displayed
	for(uint8_t i = 0;i<59;++i)
	{
		uint8_t ctr_h = ((i+2)/5);
		uint8_t ctr_m1 = i/2;
		uint8_t ctr_m2 = i/2+i%2;
		uint8_t ctr_m1c = t.minute/2;
		uint8_t ctr_m2c = (t.minute/2)+(t.minute%2);
		if((t.hour%12)!=0)
			ctr_h = (ctr_h>(t.hour%12))?(t.hour%12):ctr_h;
		if(t.minute != 0)
		{		
			ctr_m1 = (ctr_m1>ctr_m1c)?ctr_m1c:ctr_m1;
			ctr_m2 = (ctr_m2>ctr_m2c)?ctr_m2c:ctr_m2;
		}

		DisplayBuffer[0] = ctr_m1;
		if(ctr_m1!=ctr_m2)
			DisplayBuffer[1] = ctr_m2;
		else
			DisplayBuffer[1] = 255;
		DisplayBuffer[2] = ctr_h+30;
		DisplayBuffer[3] = 255;
		showLEDs(5);
		_delay_ms(5);
	}

}
void TestAllLEDs()
{
	DisplayBuffer[4] = 3;//Maximum 1 LEDs displayed, 3 is set, so brightness is the same as in normaloperation
	for(uint8_t i = 0;i<30;++i)
	{
		DisplayBuffer[0] = i;
		DisplayBuffer[1] = 255;
		DisplayBuffer[2] = 255;
		DisplayBuffer[3] = 255;
		showLEDs(25);
	}
	for(uint8_t i = 0;i<12;++i)
	{
		DisplayBuffer[0] = i+30;
		DisplayBuffer[1] = 255;
		DisplayBuffer[2] = 255;
		DisplayBuffer[3] = 255;
		showLEDs(63);
	}
	//for(uint8_t i = 0;i<60;++i)
	//{
	//	t.minute = i;
	//	t.hour=(i+2)/5;
	//	updateDisplayBuffer();
	//	showLEDs(10);
	//}
	//for(uint8_t i = 0;i<24;++i)
	//{
	//	t.minute = 0;
	//	t.hour=i;
	//	updateDisplayBuffer();
	//	showLEDs(25);
	//}
}
#endif

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
	eeprom_update_block(&tempCorrectionOffset,&tempCorrectionOffset_EEP,sizeof(uint16_t));
}
void readEEP()
{
	eeprom_read_block(&correction,&correction_EEP,sizeof(correction_t));
	eeprom_read_block(&tempCorrectionOffset,&tempCorrectionOffset_EEP,sizeof(uint16_t));
	if(correction.everyMinute>59 || correction.everyHour>59 || correction.everyDay>23)
	{
		//unplausible Data
		correction.everyMinute = 0;
		correction.everyHour = 0;
		correction.everyDay= 0;
		correction.everyMonth= 0;
	}
}
void showEEPValues(uint16_t duration = 1000)
{
	DisplayBuffer[0]=numToPortD[correction.everyMinute&0x0F];
	DisplayBuffer[1]=numToPortD[correction.everyHour&0x0F];
	DisplayBuffer[2]=numToPortD[correction.everyDay&0x0F];
	DisplayBuffer[3]=numToPortD[correction.everyMonth&0x0F];
	showLEDs(duration);
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

  //while(1)TestAllLEDs();
  init();	//Initialize registers and configure RTC.

	const uint16_t dT = 2;
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
					rightButton.directSetValue(1);
					leftButton.directSetValue(1);
					ontime = 0;
					State = display_on;
					fadeIn();
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
				if(leftButton.getValue() == 0 && rightButton.getValue() == 0)
				{
					State = display_on_latched;
				}
				ontime = 0;
				break;
			case display_on_latched:
				max_ontime = ontime_short;
				if(rightButton.valueUpdatedTo(1) == 1 && leftButton.getValue() == 1)
				{
					ontime = 0;
					State = set_hour;
				}
				if(leftButton.valueUpdatedTo(1) == 1 && rightButton.getValue() == 1)
				{
					ontime = 0;
					octs.trigger();
					State = show_temperature;
				}
				break;
			case show_temperature:
				max_ontime = ontime_long;
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
				if(caseForAdjusting<uint8_t>(&(correction.everyMinute),&ontime,29))State = set_eep_hour_correction;
				break;
			case set_eep_hour_correction:
				if(caseForAdjusting<uint8_t>(&(correction.everyHour),&ontime,29))State = set_eep_day_correction;
				break;
			case set_eep_day_correction:
				if(caseForAdjusting<uint8_t>(&(correction.everyDay),&ontime,24))State = set_eep_month_correction;
				break;
			case set_eep_month_correction:
				if(caseForAdjusting<uint8_t>(&(correction.everyMonth),&ontime,29))State = set_eep_temp_correction;
				break;
			case set_eep_temp_correction:
				if(caseForAdjusting<uint16_t>(&(tempCorrectionOffset),&ontime,370,340))State = save_eeprom;
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
		rightButton.loop(dT);
		leftButton.loop(dT);
		octs.loop();
		if((State&0xF0) != 0x00)
		{
			updateDisplayBuffer();
			_delay_us(200);//this has to be trimmed to the 1ms minus the average mainloop overhead
			showLEDs(dT-1);
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
	//showEEPValues();
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
	if(getRightButton())
	{
		wakeupTriggered=2;
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
				uint16_t temp_raw = octs.get();
				if(temp_raw!=0xFFFF)
				{
					int16_t temp_raw2 = temp_raw - tempCorrectionOffset;
					if(temp_raw2<0) temp_raw2 = -temp_raw2;
					t.steps+=tempCorrection[temp_raw2%50];//%50 so we dont get a segfault
					t.second += t.steps/10000;
					t.steps = t.steps % 10000;
				}
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
	if(t.second==50 && t.minute==59)
	{
		octs.trigger();
	}
}
static void updateDisplayBuffer(void)
{
	if((State&0xF0)<0x20)
	{
		DisplayBuffer[0]=(t.hour%12)+30;
		DisplayBuffer[1]=t.minute/2;
		if(t.minute%2>0)
			DisplayBuffer[2]=(t.minute/2+1)%30;
		else
			DisplayBuffer[2]=255;
		DisplayBuffer[3]=255;
		DisplayBuffer[4]=3; //3 elements set, sometimes there are also only two, but 3 is set to keep the Brightness equal
	}
	else if((State&0xF0)==0x20)
	{
		if(State == set_hour)
		{
			DisplayBuffer[0] = 0;
			DisplayBuffer[1] = 15;
			DisplayBuffer[2] = (t.hour%12) + 30;
			DisplayBuffer[3] = 255;
			DisplayBuffer[4]=3; //3 elements set
		}
		else if(State == set_minute)
		{
			DisplayBuffer[0] = 30;
			DisplayBuffer[1] = 36;
			DisplayBuffer[2] = t.minute/2;
			DisplayBuffer[3] = (t.minute/2)+(t.minute%2);
			DisplayBuffer[4]=4; //4 elements set
		}
	}
	else if((State&0xF0)==0x30)
	{
		DisplayBuffer[3] = 41;
		DisplayBuffer[2] = 40;
		DisplayBuffer[1]=30+(State&0x0F);//Display Setting number in first Row
		switch (State) {
			case set_eep_min_correction:
				DisplayBuffer[0] = correction.everyMinute;
				break;
			case set_eep_hour_correction:
				DisplayBuffer[0] = correction.everyHour;
				break;
			case set_eep_day_correction:
				DisplayBuffer[0] = correction.everyDay;
				break;
			case set_eep_month_correction:
				DisplayBuffer[0] = correction.everyMonth;
				break;
			case set_eep_temp_correction:
				DisplayBuffer[0] = tempCorrectionOffset-340;
				break;
			case save_eeprom:
				DisplayBuffer[0] = 255;
				break;
			case idle:
			case set_minute:
			case set_hour:
			case read_temperature:
			case display_on:
			case display_on_latched:
			case show_temperature:
				//Will not happen
				break;
		}
		DisplayBuffer[4]=3; //3 elements set
	}
	else if((State)==0x40)
	{
	
		uint16_t valueToDisplay = octs.get();
		DisplayBuffer[0] = numToPortD[(valueToDisplay&0x0F)];
		DisplayBuffer[1] = numToPortD[((valueToDisplay>>4)&0x0F)];
		DisplayBuffer[2] = numToPortD[((valueToDisplay>>8)&0x0F)];
		DisplayBuffer[3]=DISP_F;//Display 0xF to signalize temperature display
		DisplayBuffer[4]=4; //4 elements set
	}

}
