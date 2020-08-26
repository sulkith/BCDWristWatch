/*
 * STM32L4_HAL.cpp
 *
 *  Created on: Jul 24, 2020
 *      Author: sulkith
 */
#include "STM32L4_HAL.hpp"
#include "usermain.hpp"
#include "settings.hpp"
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
const uint8_t DateOffset = 2;
const uint8_t ADC_Cal_Offset = 3;
const uint8_t stepsHistOffset = 4;

const uint8_t CommandOffset = 30;
const uint8_t DBG_Offset = 31;
const uint32_t CALR_Address = FLASH_BASE + FLASH_SIZE - FLASH_PAGE_SIZE;
uint64_t readFlash(uint32_t Address) {
	return *((uint64_t*) Address);
}
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

void HAL_MX_RTC_Init(void) {

	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };
	RTC_AlarmTypeDef sAlarm = { 0 };

	/** Initialize RTC Only
	 */
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&hrtc) != HAL_OK) {
		Error_Handler();
	}

//	if (HAL_RTCEx_BKUPRead(&hrtc, 0) != 0) {
//		return; // Only initialize RTC
//	}

	/** Initialize RTC and set the Time and Date
	 */
	sTime.Hours = 0x23;
	sTime.Minutes = 0x59;
	sTime.Seconds = 0x45;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
		Error_Handler();
	}
	sDate.WeekDay = RTC_WEEKDAY_SUNDAY;
	sDate.Month = RTC_MONTH_AUGUST;
	sDate.Date = 0x9;
	sDate.Year = 0x20;

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
		Error_Handler();
	}
	/** Enable the Alarm A
	 */
	sAlarm.AlarmTime.Hours = 0x0;
	sAlarm.AlarmTime.Minutes = 0x0;
	sAlarm.AlarmTime.Seconds = 0x0;
	sAlarm.AlarmTime.SubSeconds = 0x0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = 0x1;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK) {
		Error_Handler();
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
	bma.writeAddress(BMA4_ACCEL_CONFIG_ADDR, 0x09); //Disable ACC performance Mode --> only draws ~14µA
	bma.writeAddress(BMA4_POWER_CONF_ADDR, 0x01); //Enable advanced Powersave
	bma.writeAddress(BMA4_ACCEL_CONFIG_ADDR, 0x09); //Disable ACC performance Mode --> only draws ~14µA
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
void STM32L4_HAL::HAL_driverInit() {
	HAL_PWR_EnableBkUpAccess();
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	uint32_t command = HAL_RTCEx_BKUPRead(&hrtc, CommandOffset);
	if ((__HAL_RCC_GET_RTC_SOURCE() == 0)||((command&0x0002)>0)) {
		__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
		/** Initializes the RCC Oscillators according to the specified parameters
		 * in the RCC_OscInitTypeDef structure.
		 */
		__HAL_RCC_LSE_CONFIG(RCC_LSE_ON);
		__HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
		HAL_MX_RTC_Init();
		__HAL_RCC_RTC_ENABLE();
	}
	HAL_PWREx_EnableLowPowerRunMode();
	HAL_RTC_WaitForSynchro(&hrtc);
}
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
		for (uint8_t i = 0; i < stepsHistSize; ++i) {
			stepsHist[i] = HAL_RTCEx_BKUPRead(&hrtc, stepsHistOffset + i);
		}
	} else {
		wakeupReason = WAKEUP_POR;
		uint32_t EEPVAL = readFlash(CALR_Address);

		uint16_t CALM = EEPVAL & 0x1FF;
		uint16_t CALP = EEPVAL & RTC_SMOOTHCALIB_PLUSPULSES_SET;
		if (CALM != 0 || CALP != 0) {
			HAL_RTCEx_SetSmoothCalib(&hrtc, RTC_SMOOTHCALIB_PERIOD_32SEC, CALP,
					CALM);
		}
	}

	if ((bma.getInternalState()) != 0x01) //should be 0x01
			{
		//This will only initialize the BMA if it is not already running
		setupBMA();
	} else {
		if (HAL_GPIO_ReadPin(BMA_INT_GPIO_Port, BMA_INT_Pin) == GPIO_PIN_SET) {
			uint8_t interruptState = bma.readAddress(0x1C);
			if ((interruptState & 0x08) > 0) {
				wakeupReason = WAKEUP_BMA_TILT;
			} else if ((interruptState & 0x20) > 0) {
				wakeupReason = WAKEUP_BMA_TAP;
			} else {
				showERROR(4, interruptState); //E4
			}
		}
		updateSteps();
	}

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

	if (wakeupReason == WAKEUP_UNKNOWN) {
		//WakeupReason is NOT BMA, is NOT Alarm, and RTC is already Running.
		//--> Reset Pin Triggered, or running via Debugger, therefore Activate Debugging in Sleep
		//	showERROR(0xD, 1);
	}
	ClockM::getInstance().updateTime();
	dman_loc->setBrightness(LED_Brightness_Daytime[ClockM::getInstance().getHour()%24]);

	if(HAL_RTCEx_BKUPRead(&hrtc, ADC_Cal_Offset) == 0)
	{
		MX_ADC1_Init();
		HAL_Delay(100);//give the ADC 100ms to settle
		HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
		HAL_Delay(10);//Calibration should not take longer than 10ms
		//Value between Min_Data=0x00 and Max_Data=0x7F
		uint32_t CalibrationValue = HAL_ADCEx_Calibration_GetValue(&hadc1, ADC_SINGLE_ENDED);
		CalibrationValue |= 0x10000000; // Set at least one Bit so it is not 0 next Time
		HAL_RTCEx_BKUPWrite(&hrtc, ADC_Cal_Offset,CalibrationValue);
	}
	if (wakeupReason >= 0x10) {
		MX_ADC1_Init();
		HAL_ADC_Start(&hadc1);//ADC needs to be anabled to set the Calibration Value
		HAL_ADCEx_Calibration_SetValue(&hadc1, ADC_SINGLE_ENDED, (HAL_RTCEx_BKUPRead(&hrtc, ADC_Cal_Offset)&0x7F));
		HAL_ADC_Stop(&hadc1);
		ADC_State = initialized;
	//	HAL_Delay(1);
		//HAL_ADC_Start(&hadc1);
	}

	HAL_RTCEx_BKUPWrite(&hrtc, alreadyRunningOffset, 1);//set already running
	if((HAL_RTCEx_BKUPRead(&hrtc, CommandOffset)&0x0001)>0)
	{
		pushNewSteps(HAL_RTCEx_BKUPRead(&hrtc, CommandOffset-1));
		uint32_t temp = HAL_RTCEx_BKUPRead(&hrtc, CommandOffset)&(~0x0001);
		HAL_RTCEx_BKUPWrite(&hrtc, CommandOffset, temp);
	}

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
	for(uint8_t i = (stepsHistSize-1); i > 0;--i)
	{
		stepsHist[i] = stepsHist[i-1];
	}
	stepsHist[0] = steps;
}
uint16_t STM32L4_HAL::getHistSteps(uint8_t days) {
	return stepsHist[days];
}
void STM32L4_HAL::setAxisMappingVariant(Watch_Type_t av) {
	AxisMappingVariant = av;
}
void STM32L4_HAL::writeDataToBackupRegisters() {
	HAL_RTCEx_BKUPWrite(&hrtc, alreadyRunningOffset, 1);//set already running
	HAL_RTCEx_BKUPWrite(&hrtc, stepsOffsetOffset, stepsOffset);
	for (uint8_t i = 0; i < stepsHistSize; ++i)
		HAL_RTCEx_BKUPWrite(&hrtc, stepsHistOffset + i, stepsHist[i]);
}
uint8_t STM32L4_HAL::HAL_sleep() {


	  __disable_irq();

	  __disable_fault_irq();
//	  __HAL_RCC_PWR_CLK_ENABLE(); //Is already enabled
//	  HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_B, BMA_CS_Pin); //takes a lot of power extra
//	  HAL_PWREx_EnablePullUpPullDownConfig();

	writeDataToBackupRegisters();
	getTap();//Clear a possibly latched Interrupt
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);	//Clear Wakeup Flag
//	if (HAL_RTCEx_BKUPRead(&hrtc, DBG_Offset) == 1)
//		HAL_PWR_EnterSTANDBYMode();	//Enter Standby Mode if Debuger is present
//	HAL_SuspendTick();
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
	switch(ADC_State)
	{
		case initialized:
			if(HAL_GetTick()>100)//give VREG 100ms to settle on the reference Voltage
			{
				HAL_ADC_Start(&hadc1);
				ADC_State = running;
			}
			break;
		case running:
			HAL_ADC_PollForConversion(&hadc1, 0);
			if ((HAL_ADC_GetState(&hadc1) & HAL_ADC_STATE_REG_EOC) > 0) {
				uint32_t adc_result = HAL_ADC_GetValue(&hadc1);
				ADC_Disable(&hadc1);
				HAL_ADCEx_DisableVoltageRegulator(&hadc1);
				LL_ADC_EnableDeepPowerDown(hadc1.Instance);
				RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
				PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_NONE;
				if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
				{
				Error_Handler();
				}
				uint16_t *uRefPtr = (uint16_t *)(0x1FFF75AA);//Datasheet describes to position of the Reference measurement from ST
				uint16_t uRefCtr = *uRefPtr;
				uint16_t uRef = ((uint32_t)3000)*uRefCtr/4095;
				uint32_t refVal = uRef*1023;//Versorgungsspannung in mV berechnen (1212mV * 1023 = 1239876)
				UBatt = refVal / adc_result;
				ADC_State = finished;
			}
			break;
		case finished:
		case uninitialized:
			break;

	}
	dman_loc->setBrightness(LED_Brightness_Daytime[ClockM::getInstance().getHour()%24]);
	if(HAL_RTCEx_BKUPRead(&hrtc, DateOffset)!=ClockM::getInstance().getDateCode() && HAL_GetTick()<100) //Only in the first 100ms --> So it wont block Date Setting
	{
		if(HAL_RTCEx_BKUPRead(&hrtc, DateOffset)<ClockM::getInstance().getDateCode())
		{
			pushNewSteps(bma.getSteps() - stepsOffset);
			stepsOffset = bma.getSteps();
			HAL_RTCEx_BKUPWrite(&hrtc, DateOffset, ClockM::getInstance().getDateCode());//write new Date
		}
		else if (HAL_RTCEx_BKUPRead(&hrtc, DateOffset)>ClockM::getInstance().getDateCode())
		{
			showERROR(8, HAL_RTCEx_BKUPRead(&hrtc, DateOffset)); //E8
		}
	}
}
uint16_t STM32L4_HAL::getUBatt() {
	return UBatt;
}
uint8_t STM32L4_HAL::getHistStepsSize()
{
	return stepsHistSize;
}

