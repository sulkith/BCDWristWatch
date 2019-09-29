#include "AnalogWatch.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "ClockM.h"
#include "AnalogClock_charlieArrays.h"

AnalogWatch bwatch;
HAL *hal=&bwatch;
DisplayManager *dman=&bwatch;
TwoButtonHAL *tbh=&bwatch;

const uint8_t LED_Brightness = 120;
const uint16_t Button_debounceTime=30;

uint8_t getRightButton();
uint8_t getLeftButton();
static uint8_t wakeupTriggered = 0;
Debouncer<uint8_t> leftButton(Button_debounceTime*32,0,&getLeftButton);
Debouncer<uint8_t> rightButton(Button_debounceTime*32,0,&getRightButton);

uint16_t cyclicCounterGlobal = 0;
uint16_t cyclicCounterGlobalLatched = 1000;//best case guess
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
void AnalogWatch::HAL_init()
{
  setupPorts();
  setupTimer0();
  setupPCINT();
  sei();													//Set the Global Interrupt Enable Bit
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);					//Selecting power save mode as the sleep mode to be used
	sleep_enable();											//Enabling sleep mode
  HAL_releaseInts();

}
uint8_t AnalogWatch::HAL_sleep()
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
void AnalogWatch::HAL_lockInts()
{
  deactivatePCINT();
}
void AnalogWatch::HAL_releaseInts()
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
	if(getRightButton())
	{
		wakeupTriggered=2;
		deactivatePCINT();
	}
}
ISR(TIMER2_OVF_vect)
{
	++ClockM::getInstance();
	if(cyclicCounterGlobal > 500)
	{
		cyclicCounterGlobalLatched = cyclicCounterGlobal;
		cyclicCounterGlobal=0;
	}
}

Debouncer<uint8_t> *AnalogWatch::getRightButtonDeb()
{
	return &rightButton;
}
Debouncer<uint8_t> *AnalogWatch::getLeftButtonDeb()
{
	return &leftButton;
}
void showLEDs(uint8_t DisplayBuffer[], uint16_t duration)
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
void fadeIn(uint8_t timearr[])
{
	uint8_t DisplayBuffer[5];
	DisplayBuffer[4] = 3;//Maximum 3 LEDs displayed
	for(uint8_t i = 0;i<59;++i)
	{
		uint8_t ctr_h = ((i+2)/5);
		uint8_t ctr_m1 = i/2;
		uint8_t ctr_m2 = i/2+i%2;
		uint8_t ctr_m1c = timearr[1]/2;
		uint8_t ctr_m2c = (timearr[1]/2)+(timearr[1]%2);
		if((timearr[0]%12)!=0)
			ctr_h = (ctr_h>(timearr[0]%12))?(timearr[0]%12):ctr_h;
		if(timearr[0] != 0)
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
		showLEDs(DisplayBuffer,5);
		_delay_ms(5);
	}
}
void AnalogWatch::show()
{
	uint8_t DisplayBuffer[5];
	DisplayBuffer[4]=3;
	switch (request.getType()) {
		case Empty:
			return; // no need to stay awake for long
		case FadeIn:
			DisplayBuffer[0]=request[0];
			DisplayBuffer[1]=request[1];
			fadeIn(DisplayBuffer);
			return;
			break;
		case Time:
			DisplayBuffer[0]=(request[0]%12)+30;
			DisplayBuffer[1]=request[1]/2;
			if(request[1]%2>0)
				DisplayBuffer[2]=(request[1]/2+1)%30;
			else
				DisplayBuffer[2]=255;
			//DisplayBuffer[3]=request[2]/2;//seconds
			DisplayBuffer[3]=255;
			DisplayBuffer[4]=3; //3 elements set, sometimes there are also only two, but 3 is set to keep the Brightness equal
			break;
		case SetHour:
			DisplayBuffer[0] = 0;
			DisplayBuffer[1] = 15;
			DisplayBuffer[2] = (request[0]%12) + 30;
			DisplayBuffer[3] = 255;
			DisplayBuffer[4]=3; //3 elements set
			break;
		case SetMinute:
			DisplayBuffer[0] = 30;
			DisplayBuffer[1] = 36;
			DisplayBuffer[2] = request[1]/2;
			DisplayBuffer[3] = (request[1]/2)+(request[1]%2);
			DisplayBuffer[4]=4; //4 elements set
			break;
		case SetCorrMinute:
		case SetCorrHour:
		case SetCorrDay:
		case SetCorrMonth:
			DisplayBuffer[3] = 41;
			DisplayBuffer[2] = 40;
			DisplayBuffer[0] = request[0];
			if(request.getType() == SetCorrMinute)DisplayBuffer[1] = 31;
			if(request.getType() == SetCorrHour)DisplayBuffer[1] = 32;
			if(request.getType() == SetCorrDay)DisplayBuffer[1] = 33;
			if(request.getType() == SetCorrMonth)DisplayBuffer[1] = 34;
			DisplayBuffer[4] = 4;
			break;
		case SetTempCorr:
		case ShowTemperature:
		default:
			DisplayBuffer[0]=30;
			DisplayBuffer[1]=33;
			DisplayBuffer[2]=36;
			DisplayBuffer[3]=39;
			DisplayBuffer[4]=4;
			break;
	}
	showLEDs(DisplayBuffer,1);

	DisplayRequest empty_dr;
	request = empty_dr;
}
void AnalogWatch::HAL_cyclic()
{
	cyclicCounterGlobal++;
	cyclicCounter = cyclicCounterGlobalLatched;
	uint16_t dT = 32000/cyclicCounter;
	rightButton.loop(dT);
	leftButton.loop(dT);
}
uint8_t AnalogWatch::HAL_getWakeupReason()
{
	return wakeupReason;
}
