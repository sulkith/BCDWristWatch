/*
 * STM32L4_HAL.cpp
 *
 *  Created on: Jul 24, 2020
 *      Author: sulkith
 */
#include "STM32L4_HAL.hpp"
#include "usermain.hpp"
#include "main.h"
#include "ClockM.hpp"
#include "libs/Bosch_BMA456/Bosch_BMA.h"
#include "libs/Bosch_BMA456/Bosch_BMA456_defs.h"
#include "libs/Bosch_BMA456/spi_adap.h"

extern RTC_HandleTypeDef hrtc;
extern ADC_HandleTypeDef hadc1;

Bosch_BMA bma;
const uint8_t alreadyRunningOffset = 0;
const uint8_t stepsOffsetOffset = 1;
const uint8_t stepsHistOffset = 2;
const uint8_t axisVariantOffset = 6;
const uint8_t DBG_Offset = 31;

void setCS(uint8_t t) {
	if (t == 0)
		HAL_GPIO_WritePin(BMA_CS_GPIO_Port, BMA_CS_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(BMA_CS_GPIO_Port, BMA_CS_Pin, GPIO_PIN_RESET);
}
void STM32L4_HAL::showERROR(uint8_t error, uint8_t data) {
	if (dman_loc != 0) {
		dman_loc->showERROR(error, data);
	}
}
extern void spi_write(uint8_t *data, uint8_t length,
		void (*callback)(uint8_t*));
void STM32L4_HAL::setupBMA() {
	while (bma.getChipID() != 0x16) {
		showERROR(1, bma.getChipID()); //E1
	}
	bma.init();
	bma.startInterruptConfig();
	bma.setTapDetection(0, 5, 1);
	//bma.setMotionDetection(10,5,7,1);
	bma.setConfigID(1);
	if (AxisMappingVariant == Binary_v1) {
		//map_x_axis = 0 map_x_axis_sign = 0 map_y_axis = 1 map_y_axis_sign = 1 map_z_axis = 2 map_z_axis_sign = 1
		bma.setAxisMapping(0, 0, 1, 0, 2, 0);
	} else if (AxisMappingVariant == Analog_v1) {
		//map_x_axis = 0 map_x_axis_sign = 0 map_y_axis = 1 map_y_axis_sign = 1 map_z_axis = 2 map_z_axis_sign = 1
		bma.setAxisMapping(0, 0, 1, 1, 2, 1); //TODO
	}
	bma.setWristTiltFunction(1);
	bma.setStepCounter(1);
	bma.writeInterruptConfig();
	bma.writeAddress(0x00, 0xb0);
	//bma.writeAddress(0x56,0x28);//Enable DoubleTap and WristTilt Interrupt on INT1 --> PC0
	bma.writeAddress(0x56, 0x28); //Enable All Interrupts
	bma.writeAddress(0x53, 0x0A); //Interrupt Output, LevelBased (High_Active) interrupt and PushPull
	bma.writeAddress(0x55, 0x01); //latched output
	bma.writeAddress(BMA4_POWER_CONF_ADDR, 0x01); //Enable advanced Powersave
	bma.writeAddress(BMA4_ACCEL_CONFIG_ADDR, 0x09); //Disable ACC per Mode --> only draws ~14µA
	while ((bma.getInternalState() & 0x01) != 0x01) //Bit 0 should be set
	{
		showERROR(2, bma.getInternalState()); //E2
	}
	while ((bma.getInternalState()) != 0x01) //should be 0x01
	{
		showERROR(3, bma.getInternalState()); //E3
	}

}
extern void initComDriver();
extern void spi_init();
#define MIN(a,b) (a>b)?b:a;
void STM32L4_HAL::HAL_init() {

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(BMA_CS_GPIO_Port, BMA_CS_Pin, GPIO_PIN_SET);

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/*Configure GPIO pin : BMA_CS_Pin */
	GPIO_InitStruct.Pin = BMA_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(BMA_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : BMA_INT_Pin */
	GPIO_InitStruct.Pin = BMA_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(BMA_INT_GPIO_Port, &GPIO_InitStruct);

	if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != 0) { // Wakeup Code from devboard
		if (__HAL_RTC_ALARM_GET_FLAG(&hrtc, RTC_FLAG_ALRAF) != 0U) //Wakeup from AlarmA --> Status Flag is in Backup Domain.
				{
			wakeupReason = WAKEUP_AlarmA;
			__HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);
		}
		/* Clear Standby flag */
		HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
		//			HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1); // gets enabled by end of this Function
	}
	set_SPI_activate_CS(&setCS);
	//initComDriver();

	MX_SPI1_Init();

	spi_init();

	initComDriver();

	if (HAL_RTCEx_BKUPRead(&hrtc, alreadyRunningOffset) == 1) {
		stepsOffset = HAL_RTCEx_BKUPRead(&hrtc, stepsOffsetOffset);
		for (uint8_t i = 0; i < 4; ++i) {
			stepsHist[i] = HAL_RTCEx_BKUPRead(&hrtc, stepsHistOffset + i);
		}
		AxisMappingVariant = (Watch_Type_t) HAL_RTCEx_BKUPRead(&hrtc,
				axisVariantOffset);
	} else {
		wakeupReason = WAKEUP_POR;
		//TODO Init RTC Correction Registers
		uint16_t CALM = 0;
		uint16_t CALP = 0;
		uint32_t *ID = (uint32_t*)(0x1FFF7590);
		if(ID[0] == 0x002c0030 && ID[1] == 0x58525018 && ID[2] == 0x20393357)
		{
			CALM = 302;
			CALP = RTC_SMOOTHCALIB_PLUSPULSES_SET;
		}
		if(CALM != 0 || CALP != 0)
		{
			HAL_RTCEx_SetSmoothCalib(&hrtc, RTC_SMOOTHCALIB_PERIOD_32SEC, CALP, CALM);
		}
	}

	if ((bma.getInternalState()) != 0x01) //should be 0x01
	{
		//This will only initialize the BMA if it is not already running
		setupBMA();
	}
	else
	{
		if(HAL_GPIO_ReadPin(BMA_INT_GPIO_Port, BMA_INT_Pin) == GPIO_PIN_SET)
		{
			uint8_t interruptState = bma.readAddress(0x1C);
			if ((interruptState & 0x08) > 0) {
				wakeupReason = WAKEUP_BMA_TILT;
			}
			else if ((interruptState & 0x20) > 0) {
				wakeupReason = WAKEUP_BMA_TAP;
			}
			else
			{
				showERROR(4, interruptState);//E4
			}
		}
		updateSteps();
	}
	//TODO setupUBattMeasure(); // start initial Measuring of UBatt

	while (0) { //Testing of Axis Mapping
		uint16_t test[4];
		int16_t xAccel = getX();
		int16_t yAccel = getY();
		int16_t zAccel = getZ();
		uint8_t xSign = 0;
		uint8_t ySign = 0;
		uint8_t zSign = 0;
		if (xAccel < 0) {
			xAccel = -xAccel;
			xSign = 0x08;
		}
		if (yAccel < 0) {
			yAccel = -yAccel;
			ySign = 0x08;
		}
		if (zAccel < 0) {
			zAccel = -zAccel;
			zSign = 0x08;
		}
		//65535 = 4g
		//16384 = 1g
		//16384/64 = 256
		//256/32 = 8
		xAccel = MIN(xAccel / 2048, 7)
		;
		yAccel = MIN(yAccel / 2048, 7)
		;
		zAccel = MIN(zAccel / 2048, 7)
		;

		xSign |= xAccel & 0x07;
		ySign |= yAccel & 0x07;
		zSign |= zAccel & 0x07;

		test[3] = xSign;
		test[2] = ySign;
		test[1] = zSign;
		test[0] = 0x0F;

		DisplayRequest dr(Raw_Output, test);
		dman_loc->requestDisplay(dr);
		dman_loc->show();
	}

	if(wakeupReason == WAKEUP_UNKNOWN)
	{
		//WakeupReason is NOT BMA, is NOT Alarm, and RTC is already Running.
		//--> Reset Pin Triggered, or running via Debugger, therefore Activate Debugging in Sleep
		//	showERROR(0xD, 1);
	}

	if(wakeupReason == WAKEUP_AlarmA)
	{
		if(ClockM::getInstance().getHour()==0)
		{
			//Ghost Hour ;-)
			pushNewSteps(bma.getSteps()-stepsOffset);
			stepsOffset = bma.getSteps();
		}
	}
	if(wakeupReason >= 0x10)
	{
	  MX_ADC1_Init();
	  HAL_ADC_Start(&hadc1);
	}

	//Check config
	if (HAL_RTCEx_BKUPRead(&hrtc, DBG_Offset) == 1)
	{
		DBGMCU->CR = 7;	 //Debugging in Standby//XXX
	}
	HAL_RTCEx_BKUPWrite(&hrtc, alreadyRunningOffset, 1);//set already running

	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
}
void STM32L4_HAL::setDisplayManager(DisplayManager *dman_arg) {
	dman_loc = dman_arg;
}

uint8_t STM32L4_HAL::HAL_getWakeupReason() {
	return wakeupReason;
}
uint8_t STM32L4_HAL::getTap() {
	uint8_t interruptState = bma.readAddress(0x1C);
	if ((interruptState & 0x20) > 0) {
		return 1;
	}
	return 0;
}
int16_t STM32L4_HAL::getX() {
	return bma.get_X_Accel_corr();
}
int16_t STM32L4_HAL::getY() {
	return -bma.get_Y_Accel_corr();
}
int16_t STM32L4_HAL::getZ() {
	return -bma.get_Z_Accel_corr();
}
void STM32L4_HAL::updateSteps() {
	uint32_t steps = bma.getSteps();
	if (steps < stepsOffset)
		stepsOffset = 0;
	stepsMeasured = steps - stepsOffset;
}
uint32_t STM32L4_HAL::getSteps() {
	return stepsMeasured;
}
void STM32L4_HAL::resetSteps() {
	pushNewSteps(bma.getSteps() - stepsOffset);
	stepsOffset = bma.getSteps();
}
void STM32L4_HAL::pushNewSteps(uint16_t steps) {
	stepsHist[3] = stepsHist[2];
	stepsHist[2] = stepsHist[1];
	stepsHist[1] = stepsHist[0];
	stepsHist[0] = steps;
}
uint16_t STM32L4_HAL::getHistSteps(uint8_t days) {
	return stepsHist[days];
}
void STM32L4_HAL::setAxisMappingVariant(Watch_Type_t av) {
	AxisMappingVariant = av;
}
uint8_t STM32L4_HAL::HAL_sleep() {
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	//Clear Wakeup Flag
	if (HAL_RTCEx_BKUPRead(&hrtc, DBG_Offset) == 1)HAL_PWR_EnterSTANDBYMode();	//Enter Standby Mode if Debuger is present
	HAL_PWREx_EnterSHUTDOWNMode();
	return 0;
}
void STM32L4_HAL::HAL_lockInts() {
	//TODO
}
void STM32L4_HAL::HAL_releaseInts() {
	//TODO
}
void STM32L4_HAL::HAL_cyclic() {
	//TODO
	if(UBatt == 0)
	{
		HAL_ADC_PollForConversion(&hadc1, 0);
		if((HAL_ADC_GetState(&hadc1) & HAL_ADC_STATE_REG_EOC)>0)
		{
			uint32_t adc_result = HAL_ADC_GetValue(&hadc1);
			HAL_ADCEx_DisableVoltageRegulator(&hadc1);
			UBatt = 1125300L / adc_result;  //Versorgungsspannung in mV berechnen (1100mV * 1023 = 1125300)
			HAL_ADC_DeInit(&hadc1);
		}
	}
}
uint16_t STM32L4_HAL::getUBatt() {
	return UBatt;
}

