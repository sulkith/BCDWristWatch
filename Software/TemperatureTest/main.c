
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

static void init(void);
	
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

unsigned int readchiptemp(void){ 
//read ch n of internal a/d  10 bit unsigned 
	ACSR=0x00;
	ADCSRA = 0x80;   
	_delay_us(1000);

	ADMUX= 0xC0 + 8; //select 1.1 vref + channel 8 
	ADCSRA |= 0x40;  //init conversion 
	while((ADCSRA & 0x40) != 0){}; //wait for conv complete 
	ACSR=0x80;
	ADCSRA = 0x00;  //init conversion 
	return ADC; 
} 
int main(void)
{
	TestAllLEDs();

   init();	//Initialize registers and configure RTC.
	
	while(1)
	{
		//sleep_mode();										//Enter sleep mode. (Will wake up from timer overflow interrupt)
		if(PCINT_activated>0)//wakeup on Keypress
		{
		}
		uint16_t temp_raw = readchiptemp();
		int16_t temp = ((int16_t)(temp_raw*101))-27000;
		uint16_t tempu = (temp<0)?-temp:temp;
		TimeArray[3]=0;
		if(temp<0) TimeArray[3] = DISP_6;
		TimeArray[2]=numToPortD[(temp/1000)%10];
		TimeArray[1]=numToPortD[(temp%1000)/100];
		//TimeArray[0]=numToPortD[(temp%100)/10];
		showLEDs(500);
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
	
	//powersave tweaks
	//DIDR1=0x03;
	//ACSR=0x80;
	
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
}
