#include "BinaryGWatch.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <util/delay.h>
#include "ClockM.h"
#include "libs/Bosch_BMA456/Bosch_BMA.h"
#include "libs/Bosch_BMA456/spi_adap.h"
#include "libs/spi/spi.h"
#include "settings.h"

BinaryGWatch bwatch;
HAL *hal=&bwatch;
DisplayManager *dman=&bwatch;
TwoButtonHAL *tbh=&bwatch;
GForceHAL *_gHAL=&bwatch;
Bosch_BMA bma;



const uint8_t LED_Brightness = 200;
const uint16_t Button_debounceTime=30;
const uint8_t HourlyDisplay_enabled = 0;

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
inline void setupTimer2()
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
		showERROR(1,bma.getChipID());//E1
	}
	bma.init();
	bma.startInterruptConfig();
	bma.setTapDetection(0, 5, 1);
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
		showERROR(2,bma.getInternalState());//E2
	}
	while((bma.getInternalState()) != 0x01)//should be 0x01
	{
		showERROR(3,bma.getInternalState());//E3
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
	power_all_disable();//powerdown everything
	power_timer2_enable();
	power_spi_enable();

	setupPorts();
	spi_init();
	set_SPI_activate_CS(&setCS);
	setupBMA();
	setupUBattMeasure(); // start initial Measuring of UBatt

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

  setupTimer2();
  setupPCINT();
  sei();													//Set the Global Interrupt Enable Bit
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);					//Selecting power save mode as the sleep mode to be used
	sleep_enable();											//Enabling sleep mode
  HAL_releaseInts();
	updateSteps();

	//Check config
	while(LED_Brightness >=250)showERROR(0xC,1);//EC01
}
void BinaryGWatch::setupUBattMeasure()
{
	while((UBattCtr) != 0)showERROR(0xA, 1);////EA01
	power_adc_enable();
	ADMUX |= (1<<REFS0); //VCC as refernce for ADC
	ADMUX |= (1<<MUX3) | (1<<MUX2) | (1<<MUX1);  //1.1V refernce Voltage as input
	UBattCtr = 1;
}
void BinaryGWatch::ADC_cyclic()
{
	if(UBattCtr == 0)return;

	while (((ADCSRA&(1<<ADSC))>0)) showERROR(0xA,10+UBattCtr);////EA1x

	if(UBattCtr == 1)
	{
		ADCSRA |= (1<<ADEN);   //activate ADC
		ADCSRA |= (1<<ADSC);  //start Measurement
		UBattCtr=2;
	}
	else
	{
		uint8_t adc_low = ADCL;
		uint8_t adc_high = ADCH;
		uint16_t adc_result = (adc_high<<8) | adc_low; //Gesamtergebniss der ADC-Messung
	  uint16_t vcc = 1125300L / adc_result;  //Versorgungsspannung in mV berechnen (1100mV * 1023 = 1125300)
		UBattFilterValues[(UBattCtr)-2] = vcc;
		if(UBattCtr < UBattFilterLen+2)
		{
			UBattCtr++;
			ADCSRA |= (1<<ADSC);  //restart measurement
		}
		else
		{
			ADCSRA &= ~(1<<ADEN);//deactivate ADC
			power_adc_disable();//powerdown ADC module
			uint16_t avg = 0;
			for(uint8_t i=0;i<UBattFilterLen;++i)
			{
				avg += UBattFilterValues[i];
			}
			UBatt = avg / UBattFilterLen;
			UBattCtr = 0;
		}
	}
}
uint16_t BinaryGWatch::getUBatt()
{
	return UBatt;
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
	ADC_cyclic();
	if(ClockM::getInstance().isHourChanged())
	{
		if(HourlyDisplay_enabled == 1)wakeupReason = 255; //Wakeup for hourly display only if enabled
		if((bma.getInternalState()) != 0x01)//should be 0x01
		{
			setupBMA();//Reinit BMA if something went wrong --> Will also show init Errors if there is a HW Problem
		}
		if(development==1)
		{
			while((ADCSRA & 1<<ADEN) > 0)showERROR(0xD, 1);//ED01// ADC is still enabled
			while((PRR & (1<<PRADC)) == 0)showERROR(0xD, 2);//ED02// ADC is still powered up
			while((PRR & (1<<PRUSART0)) == 0)showERROR(0xD, 3);//ED03// USART is still powered up
			while((PRR & (1<<PRSPI)) > 0)showERROR(0xD, 4);//ED04// SPI is powered down
			while((PRR & (1<<PRTIM1)) == 0)showERROR(0xD, 5);//ED05// Timer1 is still powered up
			while((PRR & (1<<PRTIM0)) == 0)showERROR(0xD, 6);//ED06// Timer0 is still powered up
			while((PRR & (1<<PRTIM2)) > 0)showERROR(0xD, 7);//ED07// Timer2 is powered down
			while((PRR & (1<<PRTWI)) == 0)showERROR(0xD, 8);//ED08// Timer0 is still powered up
		}
		if(ClockM::getInstance().getHour()==0)
		{
			//bma.resetSteps();
			pushNewSteps(bma.getSteps()-stepsOffset);
			stepsOffset = bma.getSteps();
			setupUBattMeasure();
		}
	}

	if(development > 1)
	{
		if(UBattCtr == 0)
		{
			while((ADCSRA & 0x80) > 0)showERROR(0xD, 9);//ED09// ADC is still enabled
			while((PRR & (1<<PRADC)) == 0)showERROR(0xD, 10);//ED10// ADC is still powered up
		}
		else
		{
			while(ClockM::getInstance().getMinute()>=1)showERROR(0xD, 11);//ED11//ADC Measurement is not done after 1 Minute
			//This Error will be show at startup, since the Clock is at 23:59 and the ADC Measurement is triggered in the init
			//But it will disapear, when the Clock gets increased to 0:00 --> then the Watch will behave normally.
		}
	}

	if(wakeupReason == 0xF0)//Wakeup from BMA
	{
		uint8_t interruptState = bma.readAddress(0x1C);
		if((interruptState&0x20)>0)
		{
			if(getZ() < 2000) wakeupReason = 0;//Ignore Tap if the Watch is not facing upwards
			wakeupReason |= 0x01;//Set Bit for Tap Detected
		}
		if((interruptState&0x08)>0)
		{
			wakeupReason |= 0x02;//Set Bit for WristTilt
		}

	}
	if(wakeupReason != 0)updateSteps();
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
		{
				uint16_t steps = request[2]/100;
				DisplayBuffer[2]=numToPortD[(steps/100)%16];
				DisplayBuffer[1]=numToPortD[((steps)/10)%10];
				DisplayBuffer[0]=numToPortD[((steps)%10)];
				DisplayBuffer[3]=DISP_3;
				break;
		}
		case ShowStepsHistory:
		{
				uint16_t steps = request[(request[0]%4)+1]/100;

				DisplayBuffer[2]=numToPortD[(steps/100)%16];
				DisplayBuffer[1]=numToPortD[((steps)/10)%10];
				DisplayBuffer[0]=numToPortD[((steps)%10)];
				DisplayBuffer[3]=DISP_4|numToPortD[request[0]%4];
				break;
		}
		case ShowUBatt:
		{
				uint16_t Ubatt_10mv = request[0]/10;

				DisplayBuffer[2]=numToPortD[(Ubatt_10mv/100)%16];
				DisplayBuffer[1]=numToPortD[((Ubatt_10mv)/10)%10];
				DisplayBuffer[0]=numToPortD[((Ubatt_10mv)%10)];
				DisplayBuffer[3]=DISP_8;
				break;
		}
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
void BinaryGWatch::updateSteps()
{
	uint32_t steps = bma.getSteps();
	if(steps<stepsOffset)stepsOffset=0;
	stepsMeasured = steps-stepsOffset;
}
uint32_t BinaryGWatch::getSteps()
{
	return stepsMeasured;
}
void BinaryGWatch::pushNewSteps(uint16_t steps) {
	stepsHist[3]=stepsHist[2];
	stepsHist[2]=stepsHist[1];
	stepsHist[1]=stepsHist[0];
	stepsHist[0] = steps;
}
uint16_t BinaryGWatch::getHistSteps(uint8_t days)
{
	return stepsHist[days];
}
