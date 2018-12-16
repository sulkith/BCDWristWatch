
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//FUSES =
//{
//	.low = 0xE3,		//Select cpu clk: Internal calibrated RC-oscillator @ 4MHz
//	.high = 0x99,		//Default settings
//	.extended = 0xE3,	//Default settings
//};

static char not_leap(void);
static void init(void);
static void updateDate(void);
static void updateTimeArray(void);

typedef struct{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t date;
	uint8_t month;
	uint8_t year;
}time;
	
time t;
uint8_t TimeArray[4];
uint8_t PCINT_activated=0;

int main(void)
{
    init();	//Initialize registers and configure RTC.
	
	while(1)
	{
		sleep_mode();										//Enter sleep mode. (Will wake up from timer overflow interrupt)
		if(PCINT_activated==1)
		{
			PORTB=~PORTB;
			PCINT_activated=0;
		}
		//TCCR2B=(1<<CS00)|(1<<CS02);							//Write dummy value to control register
		//while(ASSR&((1<<TCN2UB)|(1<<OCR2BUB)|(1<<TCR2BUB)|(1<<OCR2AUB)|(1<<TCR2AUB)));	//Wait until TC0 is updated
	}
}

static void init(void)
{
	DDRB =0xFF;
	PORTB=0xFF;
	//Wait for external clock crystal to stabilize;
	for (uint8_t i=0; i<0x40; i++)
	{
		for (uint32_t j=0; j<0xFFFF; j++);
	}
	DDRC = 0xFF;											//Configure all eight pins of port B as outputs
	PORTC = 0x00;											//Configure all eight pins of port B as outputs
	TIMSK2 &= ~((1<<TOIE2)|(1<<OCIE2A)|(1<<OCIE2B));						//Make sure all TC0 interrupts are disabled
	ASSR |= (1<<AS2);										//set Timer/counter0 to be asynchronous from the CPU clock
															//with a second external clock (32,768kHz)driving it.								
	TCNT2 =0;												//Reset timer
	TCCR2B =(1<<CS00)|(1<<CS02);								//Prescale the timer to be clock source/128 to make it
															//exactly 1 second for every overflow to occur
	while(ASSR&((1<<TCN2UB)|(1<<OCR2BUB)|(1<<TCR2BUB)|(1<<OCR2AUB)|(1<<TCR2AUB)));	//Wait until TC0 is updated
	{}
	TIMSK2 |= (1<<TOIE2);									//Set 8-bit Timer/Counter0 Overflow Interrupt Enable

	//enable PinChangeInterrupt
	PCICR |= (1<<PCIE2);
	PCMSK2|= 0xFF; //PCINT20
	DDRD = 0x00;
	PORTD = 0x10;
	
	sei();													//Set the Global Interrupt Enable Bit
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);					//Selecting power save mode as the sleep mode to be used
	sleep_enable();											//Enabling sleep mode
}

ISR(PCINT2_vect)
{
	PCINT_activated=1;
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
				updateDate();
			}
		}
	}
//	PORTC=~(((t.second&0x01)|t.minute<<1)|t.hour<<7);
PORTC = (PORTC & 0xFE)|(t.second&0x01);
//	updateTimeArray();
//	PORTC = ~PORTC;
	updateTimeArray();
}
const uint8_t numToPortD[] =
{
	0b00000000,// 0
   0b01000000,// 1
	0b10000000,// 2
	0b11000000,// 3
	0b00000010,// 4
	0b01000010,// 5
	0b10000010,// 6
	0b11000010,// 7
	0b00000001,// 8
	0b01000001,// 9
	0b10000001,// A
	0b11000001,// B
	0b00000011,// C
	0b01000011,// D
	0b10000011,// E
	0b11000011 // F
};
static void updateTimeArray(void)
{
	TimeArray[0]=numToPortD[t.hour/10]|1<<5;
	TimeArray[1]=numToPortD[t.hour%10]|1<<4;
	TimeArray[2]=numToPortD[t.minute/10]|1<<3;
	TimeArray[3]=numToPortD[t.minute%10]|1<<2;
}

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
