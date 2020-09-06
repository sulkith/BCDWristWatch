/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
enum {
	Init, TestLEDs, WaitForTimer, Finished, Error
} State = Init;
uint8_t Errortype = 0;
uint8_t ErrorVar = 0;
const uint8_t LED_Brightness = 240;

//Maximum of 130000
#define secondsToMeasure 300
int16_t SecondsElapsed = -1;
uint32_t startMeasure = 0;
uint32_t endMeasure = 0;
int64_t diffperppm = 0;


const uint32_t Timerrun_offset = 0x8000 * 8; //Prediv 8
uint32_t DWT_Delay_Init(void) {
	/* Disable TRC */
	CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
	/* Enable TRC */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;
	/* Disable clock cycle counter */
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
	/* Enable  clock cycle counter */
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; //0x00000001;
	/* Reset the clock cycle counter value */
	DWT->CYCCNT = 0;
	/* 3 NO OPERATION instructions */
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	volatile uint8_t a;
	a++;
	a++;
	a++;
	/* Check if clock cycle counter has started */
	if (DWT->CYCCNT) {
		return 0; /*clock cycle counter started*/
	} else {
		return 1; /*clock cycle counter not started*/
	}
}
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t microseconds) {
	uint32_t clk_cycle_start = DWT->CYCCNT;

	/* Go to number of cycles for system */
	microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);

	/* Delay till end */
	while ((DWT->CYCCNT - clk_cycle_start) < microseconds)
		;
}
void showLEDs(uint16_t DisplayBuffer[], uint16_t duration) {
	const uint8_t perc = LED_Brightness;
	for (uint16_t j = 0; j < duration; ++j)
		for (uint8_t i = 0; i < 4; ++i) {
			//PORTD=DisplayBuffer[i]|((0b00111100)&(~(1<<(2+i))));
			GPIOA->BSRR = DisplayBuffer[3 - i] | Column[i];
			DWT_Delay_us(perc);
			GPIOA->BSRR = mask_clear;
			DWT_Delay_us(250 - perc);
		}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	/* Prevent unused argument(s) compilation warning */

	if(SecondsElapsed == -1)
	{
		startMeasure = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		SecondsElapsed=1;
	}
	else if(SecondsElapsed == secondsToMeasure)
	{
		endMeasure = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		SecondsElapsed++;
	}
	else if(SecondsElapsed > secondsToMeasure)
	{

	}
	else
	{
		SecondsElapsed++;
	}

}

void writeFlash(uint32_t Address, uint64_t data) {
//	uint32_t Address = FLASH_BASE+FLASH_SIZE-FLASH_PAGE_SIZE;
	HAL_StatusTypeDef status = 0;
	uint64_t currdata = *((uint64_t*) Address);
	if (data != currdata) {
		status = HAL_FLASH_Unlock();
		FLASH_EraseInitTypeDef erase = { 0 };
		erase.TypeErase = FLASH_TYPEERASE_PAGES;
		erase.Banks = FLASH_BANK_1;
		erase.Page = (Address - FLASH_BASE) / PAGESIZE;
		erase.NbPages = 1;
		uint32_t Error = 0;
		status = HAL_FLASHEx_Erase(&erase, &Error);
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, data);
		status = HAL_FLASH_Lock();
		UNUSED(status);
	}
}
uint64_t readFlash(uint32_t Address) {
	return *((uint64_t*) Address);
}

void init_IOs() {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA,
			LSH01_Pin | LSH10_Pin | HS1_Pin | HS2_Pin | HS8_Pin | HS4_Pin
					| LSM01_Pin | LSM10_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(BMA_CS_GPIO_Port, BMA_CS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pins : PA1 PA7 PA8 PA15 */
	GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : LSH01_Pin LSH10_Pin HS1_Pin HS2_Pin
	 HS8_Pin HS4_Pin LSM01_Pin LSM10_Pin */
	GPIO_InitStruct.Pin = LSH01_Pin | LSH10_Pin | HS1_Pin | HS2_Pin | HS8_Pin
			| HS4_Pin | LSM01_Pin | LSM10_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PB0 PB1 PB7 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : BMA_CS_Pin */
	GPIO_InitStruct.Pin = BMA_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BMA_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PH3 */
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
}
/* USER CODE END 0 */

#define WatchVersCodeAddress (FLASH_BASE + FLASH_SIZE - FLASH_PAGE_SIZE + 0x10)

#define Undefined 0xFFFFUL
#define Binary_STM_V1 0x0100UL
#define Analog_STM_V1 0x0200UL
const uint32_t WatchVariant = Binary_STM_V1;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  if(0)//just to get rid of warnings
  {
	  MX_GPIO_Init();
	  MX_SPI1_Init();
  }
  /* USER CODE BEGIN Init */
  uint64_t WatchVersCode = *((uint64_t*) WatchVersCodeAddress);
  if((WatchVersCode&0xFFFF) == Undefined)
  {
	  writeFlash(FLASH_BASE + FLASH_SIZE - FLASH_PAGE_SIZE + 0x10, WatchVariant);
  }

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_TIM2_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
	DWT_Delay_Init();
	init_IOs();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_TIM_Base_MspInit(&htim2);
	if (HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
		State = Error;
	}
	State = TestLEDs;
	while (1) {
		uint16_t Buffer[4] = { 0 };
		switch (State) {
		case Error:
			init_IOs();
			Buffer[3] = DISP_9;
			Buffer[2] = DISP_6;
			Buffer[1] = DISP_6;
			Buffer[0] = DISP_9;
			break;
		case Finished:
			Buffer[3] = DISP_2;
			Buffer[2] = DISP_1;
			Buffer[1] = DISP_2;
			Buffer[0] = DISP_4;
			break;
		case Init:
		case TestLEDs:
			Buffer[3] = DISP_F;
			Buffer[2] = DISP_F;
			Buffer[1] = DISP_F;
			Buffer[0] = DISP_F;
			if (HAL_GetTick() > 10000)
				State = WaitForTimer;
			break;
		case WaitForTimer:
			if(endMeasure!=0)
			{
				const uint32_t measuredClks = endMeasure -startMeasure;
				const uint32_t calculatedClks = 0x8000*secondsToMeasure;

				const int64_t difference = ((int64_t)calculatedClks-measuredClks);
				int64_t diffperppm1 = (difference*1000000);
				if(diffperppm1<0)//round the Value
					diffperppm1 -= (measuredClks/2);
				else
					diffperppm1 += (measuredClks/2);
				diffperppm = (int64_t)diffperppm1/(int64_t)measuredClks;
				if(diffperppm < 512 && diffperppm > -511)
								{
									uint16_t CALM = 0;
									uint16_t CALP = RTC_SMOOTHCALIB_PLUSPULSES_RESET;
									if (diffperppm > 0) {
										CALP = RTC_SMOOTHCALIB_PLUSPULSES_SET;
										CALM = 512 - diffperppm;
									} else {
										CALM = -diffperppm;
									}
									uint32_t EEPVAL = CALP | CALM;
									writeFlash(FLASH_BASE + FLASH_SIZE - FLASH_PAGE_SIZE, EEPVAL); //Write CalibrationValue to beginning of last Page
									HAL_RTCEx_SetSmoothCalib(&hrtc, RTC_SMOOTHCALIB_PERIOD_32SEC, CALP, CALM);//Correct RTC Immediately, to make sure the values are used even without POR
									//Note CALP, CALM, DEVID and REVID for the STM32L4_HAL.cpp File
									//__asm__("BKPT");
									State=Finished;
								}
								else
								{
									State = Error;
								}

			}
		else {
				uint16_t TimeToGo = secondsToMeasure-SecondsElapsed;
				Buffer[3] = DISP_C;
				Buffer[2] = numToPort[((TimeToGo) / 100) % 10];;
				Buffer[1] = numToPort[((TimeToGo) / 10) % 10];
				Buffer[0] = numToPort[(TimeToGo) % 10];
			}
			break;

		}
		showLEDs(Buffer, 500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_LSE
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
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
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xFFFFFFFF;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
  sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
  sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
  sClockSourceConfig.ClockFilter = 0;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIMEx_RemapConfig(&htim2, TIM_TIM2_ITR1_USB_SOF|TIM_TIM2_ETR_LSE) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BMA_CS_GPIO_Port, BMA_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BMA_CS_Pin */
  GPIO_InitStruct.Pin = BMA_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BMA_CS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
