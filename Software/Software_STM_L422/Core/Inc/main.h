/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BMA_INT_Pin GPIO_PIN_0
#define BMA_INT_GPIO_Port GPIOA
#define LSH01_Pin GPIO_PIN_3
#define LSH01_GPIO_Port GPIOA
#define LSH10_Pin GPIO_PIN_4
#define LSH10_GPIO_Port GPIOA
#define HS1_Pin GPIO_PIN_5
#define HS1_GPIO_Port GPIOA
#define HS2_Pin GPIO_PIN_6
#define HS2_GPIO_Port GPIOA
#define HS8_Pin GPIO_PIN_9
#define HS8_GPIO_Port GPIOA
#define HS4_Pin GPIO_PIN_10
#define HS4_GPIO_Port GPIOA
#define LSM01_Pin GPIO_PIN_11
#define LSM01_GPIO_Port GPIOA
#define LSM10_Pin GPIO_PIN_12
#define LSM10_GPIO_Port GPIOA
#define BMA_CS_Pin GPIO_PIN_6
#define BMA_CS_GPIO_Port GPIOB
void   MX_GPIO_Init(void);
void   MX_RTC_Init(void);
void   MX_SPI1_Init(void);
void   MX_ADC1_Init(void);
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */