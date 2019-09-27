#include "BinaryWatch.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "ClockM.h"
#include "libs/Bosch_BMA456/Bosch_BMA.h"
#include "libs/spi/spi.h"

BinaryWatch bwatch;
HAL *hal=&bwatch;
DisplayManager *dman=&bwatch;
TwoButtonHAL *tbh=&bwatch;

Bosch_BMA bma;

const uint8_t LED_Brightness = 120;
const uint16_t Button_debounceTime=30;

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

uint8_t getRightButton();
uint8_t getLeftButton();
static uint8_t wakeupTriggered = 0;
Debouncer<uint8_t> leftButton(Button_debounceTime*32,0,&getLeftButton);
Debouncer<uint8_t> rightButton(Button_debounceTime*32,0,&getRightButton);

uint16_t cyclicCounterGlobal = 0;
uint16_t cyclicCounterGlobalLatched = 1000;//best case guess

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
void BinaryWatch::HAL_init()
{
  setupPorts();
	spi_init();
	bma.init();
  setupTimer0();
  setupPCINT();
  sei();													//Set the Global Interrupt Enable Bit
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);					//Selecting power save mode as the sleep mode to be used
	sleep_enable();											//Enabling sleep mode
  HAL_releaseInts();

}
uint8_t BinaryWatch::HAL_sleep()
{
	wakeupTriggered=0;
	wakeupReason=0;
	activatePCINT();
	sleep_mode();
	leftButton.directSetValue(getLeftButton());
	rightButton.directSetValue(getRightButton());
	wakeupReason = wakeupTriggered;
	if(ClockM::getInstance().isHourChanged())wakeupReason = 255; //Wakeup for hourly display
	return wakeupReason;
}
void BinaryWatch::HAL_lockInts()
{
  deactivatePCINT();
}
void BinaryWatch::HAL_releaseInts()
{
  activatePCINT();
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
	++ClockM::getInstance();
	if(cyclicCounterGlobal > 10)
	{
		cyclicCounterGlobalLatched = cyclicCounterGlobal;
		cyclicCounterGlobal=0;
	}
}

Debouncer<uint8_t> *BinaryWatch::getRightButtonDeb()
{
	return &rightButton;
}
Debouncer<uint8_t> *BinaryWatch::getLeftButtonDeb()
{
	return &leftButton;
}
void showLEDs(uint8_t DisplayBuffer[], uint16_t duration)
{
		const uint8_t perc = LED_Brightness;
		for(uint16_t j = 0; j < duration; ++j)
			for(uint8_t i = 0; i < 4; ++i)
			{
				PORTD=DisplayBuffer[i]|((0b00111100)&(~(1<<(2+i))));
				_delay_us(perc);
				PORTD=0;
				_delay_us(250-perc);
			}
}
void BinaryWatch::show()
{
	uint8_t DisplayBuffer[4];
	switch (request.getType()) {
		case Empty:
			return; // no need to stay awake for long
		case FadeIn:
		case Time:
		case SetHour:
		case SetMinute:
			DisplayBuffer[3]=numToPortD[request[0]/10];
			DisplayBuffer[2]=numToPortD[request[0]%10];
			DisplayBuffer[1]=numToPortD[request[1]/10];
			DisplayBuffer[0]=numToPortD[request[1]%10];
			if(request[0]==0 && request[1]==0)
			{
				DisplayBuffer[3]=numToPortD[2];
				DisplayBuffer[2]=numToPortD[4];
				DisplayBuffer[1]=numToPortD[0];
				DisplayBuffer[0]=numToPortD[0];
			}
			if(request.getType() == SetHour)DisplayBuffer[3]|=DISP_8;
			if(request.getType() == SetMinute)DisplayBuffer[1]|=DISP_8;
			break;
		case SetCorrMinute:
		case SetCorrHour:
		case SetCorrDay:
		case SetCorrMonth:
			DisplayBuffer[1]=numToPortD[request[0]/10];
			DisplayBuffer[0]=numToPortD[request[0]%10];
			DisplayBuffer[2]=DISP_F;
			if(request.getType() == SetCorrMinute)DisplayBuffer[3] = DISP_1;
			if(request.getType() == SetCorrHour)DisplayBuffer[3] = DISP_2;
			if(request.getType() == SetCorrDay)DisplayBuffer[3] = DISP_3;
			if(request.getType() == SetCorrMonth)DisplayBuffer[3] = DISP_4;
			break;
		case SetTempCorr:
			DisplayBuffer[2]=numToPortD[request[0]/100];
			DisplayBuffer[1]=numToPortD[(request[0]/10)%10];
			DisplayBuffer[0]=numToPortD[request[0]%10];
			DisplayBuffer[3]=DISP_F;
			break;
		case ShowTemperature:
		default:
			DisplayBuffer[0]=DISP_F;
			DisplayBuffer[1]=DISP_F;
			DisplayBuffer[2]=DISP_F;
			DisplayBuffer[3]=DISP_F;
			break;
	}
	showLEDs(DisplayBuffer,1);

	DisplayRequest empty_dr;
	request = empty_dr;
}
void BinaryWatch::HAL_cyclic()
{
	cyclicCounterGlobal++;
	cyclicCounter = cyclicCounterGlobalLatched;
	uint16_t dT = 32000/cyclicCounter;
	rightButton.loop(dT);
	leftButton.loop(dT);
}
uint8_t BinaryWatch::HAL_getWakeupReason()
{
	return wakeupReason;
}
