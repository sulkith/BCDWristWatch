
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "types.h"
#include "settings.h"
#include "constants.h"


extern correction_t EEMEM correction_EEP;

static void init(void);
static void updateTimeArray(void);



time t;
enum state_t setting = idle;
correction_t correction={0,4,4};
uint8_t TimeArray[4];
uint8_t PCINT_activated=0;

void showLEDs(uint16_t duration)
{
		const uint8_t perc = 100;
		for(uint16_t j = 0; j < duration; ++j)
			for(uint8_t i = 0; i < 4; ++i)
			{
				PORTD=TimeArray[i]|((0b00111100)&(~(1<<(2+i))));
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
		TimeArray[0] = shifting[i+0];
		TimeArray[1] = shifting[i+1];
		TimeArray[2] = shifting[i+2];
		TimeArray[3] = shifting[i+3];
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
int main(void)
{
	TestAllLEDs();

	readEEP();
	TimeArray[0]=numToPortD[correction.everyMinute&0x0F];
	TimeArray[1]=numToPortD[correction.everyHour&0x0F];
	TimeArray[2]=numToPortD[correction.everyDay&0x0F];
	TimeArray[3]=numToPortD[correction.everyMonth&0x0F];
	showLEDs(1000);

	t.hour = 23;
	t.minute = 59;
	t.second = 30;
  init();	//Initialize registers and configure RTC.

	while(1)
	{
		sleep_mode();										//Enter sleep mode. (Will wake up from timer overflow interrupt)
		if(PCINT_activated>0)//wakeup on Keypress
		{
			const uint16_t ontime = 5000;
			const uint16_t long_ontime = 30000;
			uint16_t remaining = ontime;
			while(remaining > 0)
			{
				if(PCINT_activated>0)
				{
					if((setting != idle)||(((PINB&0x01)|(PINC&0x10))==0))//both Keys Pressed
					{
						t.second=0;
						if(PCINT_activated>0)
							remaining = long_ontime;
						if(setting == idle)
						{
							remaining = long_ontime;
							setting = hour;
						}
						else if(PCINT_activated==1)
						{
							if(setting == hour)
							{
								t.hour = (t.hour+1)%24;
							}
							else if(setting == minute)
							{
								t.minute = (t.minute+1)%60;
							}
							PCINT_activated=0xF0;
						}
						else if(PCINT_activated==2)
						{
							if(setting == hour)setting=minute;
							else if(setting == minute)
							{
								setting=idle;
								remaining = ontime;
							}
							PCINT_activated=0xF0;
						}
					}
					PCINT_activated++;
				}
				updateTimeArray();
				if(setting==hour)TimeArray[3]|=0x01;
				if(setting==minute)TimeArray[1]|=0x01;
				showLEDs(1);
				remaining--;
			}
			setting = idle;
			PCINT_activated=0;
		}
	}
}

static void init(void)
{
	DDRB =0xFE;
	PORTB=0x01;
	DDRC = 0x00;											//Configure all eight pins of port B as outputs
	PORTC = 0x10;											//Configure all eight pins of port B as outputs
	//Wait for external clock crystal to stabilize;
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
	if((PINB&0x01)==0)
		PCINT_activated=1;
}
ISR(PCINT1_vect)
{
	if((PINC&0x10)==0)
		PCINT_activated=2;
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
			PCINT_activated=0xF0;//show Time every Hour
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
static void updateTimeArray(void)
{
	TimeArray[3]=numToPortD[t.hour/10];
	TimeArray[2]=numToPortD[t.hour%10];
	TimeArray[1]=numToPortD[t.minute/10];
	TimeArray[0]=numToPortD[t.minute%10];
	if(t.hour==0 && t.minute==0)
	{
		TimeArray[3]=numToPortD[2];
		TimeArray[2]=numToPortD[4];
		TimeArray[1]=numToPortD[0];
		TimeArray[0]=numToPortD[0];
	}
}
