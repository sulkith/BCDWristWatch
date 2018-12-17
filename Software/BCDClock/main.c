
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

//static char not_leap(void);
static void init(void);
//static void updateDate(void);
static void updateTimeArray(void);

typedef struct{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
/*
	uint8_t date;
	uint8_t month;
	uint8_t year;
*/
}time;
	
time t;
uint8_t TimeArray[4];
uint8_t PCINT_activated=0;

#define DISP_0 0b00000000
#define DISP_1 0b01000000
#define DISP_2 0b10000000
#define DISP_3 0b11000000
#define DISP_4 0b00000010
#define DISP_5 0b01000010
#define DISP_6 0b10000010
#define DISP_7 0b11000010
#define DISP_8 0b00000001
#define DISP_9 0b01000001
#define DISP_A 0b10000001
#define DISP_B 0b11000001
#define DISP_C 0b00000011
#define DISP_D 0b01000011
#define DISP_E 0b10000011
#define DISP_F 0b11000011
const uint8_t numToPortD[] =
{
	DISP_0,// 0
	DISP_1,// 1
	DISP_2,// 2
	DISP_3,// 3
	DISP_4,// 4
	DISP_5,// 5
	DISP_6,// 6
	DISP_7,// 7
	DISP_8,// 8
	DISP_9,// 9
	DISP_A,// A
	DISP_B,// B
	DISP_C,// C
	DISP_D,// D
	DISP_E,// E
	DISP_F // F
};
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
const uint8_t shifting[] =
{
	DISP_0,
	DISP_0,
	DISP_0,
	DISP_1,
	DISP_3,
	DISP_7,
	DISP_F,
	DISP_E,
	DISP_C,
	DISP_8,
	DISP_0,
	DISP_0,
	DISP_0
};
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

enum state_t
{
	hour,
	minute,
	idle
};
int main(void)
{
	enum state_t setting = idle;
	TestAllLEDs();

	t.hour = 20;
	t.minute = 7;
	t.second = 57;
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
				//TimeArray[3]|=0x02;
				if(setting==hour)TimeArray[3]|=0x01;
				if(setting==minute)TimeArray[1]|=0x01;
				showLEDs(1);
				remaining--;
			}
			setting = idle;
			PCINT_activated=0;
		}
		//TCCR2B=(1<<CS00)|(1<<CS02);							//Write dummy value to control register
		//while(ASSR&((1<<TCN2UB)|(1<<OCR2BUB)|(1<<TCR2BUB)|(1<<OCR2AUB)|(1<<TCR2AUB)));	//Wait until TC0 is updated
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
		t.second=0;
		if (++t.minute==60)
		{
			t.minute=0;
			if (++t.hour==24)
			{
				t.hour=0;
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
}
/*
static void updateDate()
{
	if (++t.date==32)
	{
		t.month++;
		t.date=1;
	}
	else if (t.date==31)
	{
		if ((t.month==4) || (t.month==6) || (t.month==9) || (t.month==11))
		{
			t.month++;
			t.date=1;
		}
	}
	else if (t.date==30)
	{
		if(t.month==2)
		{
			t.month++;
			t.date=1;
		}
	}
	else if (t.date==29)
	{
		if((t.month==2) && (not_leap()))
		{
			t.month++;
			t.date=1;
		}
	}
	if (t.month==13)
	{
		t.month=1;
		t.year++;
	}
}

static char not_leap(void)      //check for leap year
{
	if (!(t.year%100))
	{
		return (char)(t.year%400);
	}
	else
	{
		return (char)(t.year%4);
	}
}
*/
