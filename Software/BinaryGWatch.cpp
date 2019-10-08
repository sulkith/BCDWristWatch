#include "BinaryGWatch.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "ClockM.h"
#include "libs/Bosch_BMA456/Bosch_BMA.h"
#include "libs/Bosch_BMA456/spi_adap.h"
#include "libs/spi/spi.h"

BinaryGWatch bwatch;
HAL *hal=&bwatch;
DisplayManager *dman=&bwatch;
TwoButtonHAL *tbh=&bwatch;
GForceHAL *_gHAL=&bwatch;
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
void showLEDs(uint8_t DisplayBuffer[], uint16_t duration);
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
uint8_t getINT0()
{
	if((PINC&0x01)==0x01)
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
	PCMSK1|= 0x11; //PCINT12 PCINT8
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
void showERROR(uint8_t error, uint8_t data) {
	uint8_t test[4];
	test[0] = numToPortD[data%10];
	test[1] = numToPortD[(data/10)%10];
	test[2] = numToPortD[error&0x0F];
	test[3] = numToPortD[0x0E];
	showLEDs(test,1);
}
inline void setupBMA()
{
	while(bma.getChipID() != 0x16)
	{
		showERROR(1,bma.getChipID());
	}
	bma.init();
	bma.startInterruptConfig();
	bma.setTapDetection(0, 3, 1);
	//bma.setMotionDetection(10,5,7,1);
	bma.setConfigID(1);
	//map_x_axis = 0 map_x_axis_sign = 0 map_y_axis = 1 map_y_axis_sign = 1 map_z_axis = 2 map_z_axis_sign = 1
	bma.setAxisMapping(0, 0, 1, 1, 2, 1);
	bma.setWristTiltFunction(1);
	bma.setStepCounter(1);
	bma.writeInterruptConfig();
	bma.writeAddress(0x00,0xb0);
	//bma.writeAddress(0x56,0x28);//Enable DoubleTap and WristTilt Interrupt on INT1 --> PC0
	bma.writeAddress(0x56,0x28);//Enable All Interrupts
	bma.writeAddress(0x53,0x0A);//Interrupt Output, LevelBased (High_Active) interrupt and PushPull
	bma.writeAddress(0x55,0x01);//latched output
	bma.writeAddress(BMA4_POWER_CONF_ADDR,0x01);//Enable advanced Powersave
  bma.writeAddress(BMA4_ACCEL_CONFIG_ADDR,0x09);//Disable ACC per Mode --> only draws ~14µA
	while((bma.getInternalState()&0x01) != 0x01)//Bit 0 should be set
	{
		showERROR(2,bma.getInternalState());
	}
	while((bma.getInternalState()) != 0x01)//should be 0x01
	{
		showERROR(3,bma.getInternalState());
	}

}
void setCS(uint8_t t){
	if(t==0)
		PORTB |= _BV(PB2);
	else

		PORTB &= ~_BV(PB2);
}
#define MIN(a,b) (a>b)?b:a;
void BinaryGWatch::HAL_init()
{
	uint8_t test[4];
  setupPorts();
	spi_init();
	set_SPI_activate_CS(&setCS);
	setupBMA();

/*
	if(bma.getInternalState() != 0x01)
	{
		while (1) {
			test[1] = numToPortD[0];
			test[0] = numToPortD[bma.getInternalState()];
			test[2] = numToPortD[0x01];
			test[3] = numToPortD[0x0E];
			showLEDs(test,1);
		}
		//Displays ErrorCode E1
	}
*/
	while (0) {
		int16_t xAccel = getX();
		int16_t yAccel = getY();
		int16_t zAccel = getZ();
		uint8_t xSign = 0;
		uint8_t ySign = 0;
		uint8_t zSign = 0;
		if(xAccel < 0)
		{
			xAccel = -xAccel;
			xSign = 0x08;
		}
		if(yAccel < 0)
		{
			yAccel = -yAccel;
			ySign = 0x08;
		}
		if(zAccel < 0)
		{
			zAccel = -zAccel;
			zSign = 0x08;
		}
		//65535 = 4g
		//16384 = 1g
		//16384/64 = 256
		//256/32 = 8
		xAccel = MIN(xAccel/2048,7);
		yAccel = MIN(yAccel/2048,7);
		zAccel = MIN(zAccel/2048,7);

		xSign |= xAccel&0x07;
		ySign |= yAccel&0x07;
		zSign |= zAccel&0x07;

		test[0] = numToPortD[xSign];
		test[1] = numToPortD[ySign];
		test[2] = numToPortD[zSign];
		test[3] = numToPortD[0x0F];
		showLEDs(test,1);
	}
  setupTimer0();
  setupPCINT();
  sei();													//Set the Global Interrupt Enable Bit
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);					//Selecting power save mode as the sleep mode to be used
	sleep_enable();											//Enabling sleep mode
  HAL_releaseInts();

}
uint8_t BinaryGWatch::HAL_sleep()
{
	wakeupTriggered=0;
	wakeupReason=0;
	bma.readAddress(0x1C);//Clear the Interrupt on the BMA, just in case ;-)
	activatePCINT();
	sleep_mode();
	leftButton.directSetValue(getLeftButton());
	rightButton.directSetValue(getRightButton());
	wakeupReason = wakeupTriggered;
	if(ClockM::getInstance().isHourChanged())
	{
		wakeupReason = 255; //Wakeup for hourly display
		if(ClockM::getInstance().getHour()==0)
		{
			//bma.resetSteps();
			stepsOffset = bma.getSteps();
		}
	}
	if(wakeupReason == 0xF0)//Wakeup from BMA
	{
		uint8_t interruptState = bma.readAddress(0x1C);
		if((interruptState&0x20)>0)
		{
			wakeupReason |= 0x01;//Set Bit for Tap Detected
		}
		if((interruptState&0x08)>0)
		{
			wakeupReason |= 0x02;//Set Bit for WristTilt
		}

	}else{
		if((bma.getInternalState()) != 0x01)//should be 0x01
		{
			setupBMA();//Reinit BMA if something went wrong --> Will also show init Errors if there is a HW Problem
		}
	}
	return wakeupReason;
}
void BinaryGWatch::HAL_lockInts()
{
  deactivatePCINT();
}
void BinaryGWatch::HAL_releaseInts()
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
	else if(getINT0())
	{
		wakeupTriggered=0xF0;
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

Debouncer<uint8_t> *BinaryGWatch::getRightButtonDeb()
{
	return &rightButton;
}
Debouncer<uint8_t> *BinaryGWatch::getLeftButtonDeb()
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
void BinaryGWatch::show()
{
	uint8_t DisplayBuffer[4];
	switch (request.getType()) {
		case Empty:
			return; // no need to stay awake for long
		case FadeIn:
		case Time:
		case SetHour:
		case SetMinute:
		case Debouncing:
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
			if(request.getType() == Debouncing)
			{
				DisplayBuffer[3]|=DISP_8;
				DisplayBuffer[1]|=DISP_8;
			}
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
		case showStepCounter:
				DisplayBuffer[2]=numToPortD[request[0]/10];
				DisplayBuffer[1]=numToPortD[(request[0])%10];
				DisplayBuffer[0]=numToPortD[request[1]&0xF];
				DisplayBuffer[3]=DISP_3;
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
void BinaryGWatch::HAL_cyclic()
{
	cyclicCounterGlobal++;
	cyclicCounter = cyclicCounterGlobalLatched;
	uint16_t dT = 32000/cyclicCounter;
	rightButton.loop(dT);
	leftButton.loop(dT);
}
uint8_t BinaryGWatch::HAL_getWakeupReason()
{
	return wakeupReason;
}
uint8_t BinaryGWatch::getTap()
{
	uint8_t interruptState = bma.readAddress(0x1C);
	if((interruptState&0x20)>0)
	{
		return 1;
	}
	return 0;
}
int16_t BinaryGWatch::getX()
{
	return bma.get_X_Accel();
}
int16_t BinaryGWatch::getY()
{
	return -bma.get_Y_Accel();
}
int16_t BinaryGWatch::getZ()
{
	return -bma.get_Z_Accel();
}
uint32_t BinaryGWatch::getSteps()
{
	if(bma.getSteps()<stepsOffset)stepsOffset=0;
	return bma.getSteps()-stepsOffset;
}
