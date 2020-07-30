/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
extern const uint16_t DISP_0;
extern const uint16_t DISP_1;
extern const uint16_t DISP_2;
extern const uint16_t DISP_3;
extern const uint16_t DISP_4;
extern const uint16_t DISP_5;
extern const uint16_t DISP_6;
extern const uint16_t DISP_7;
extern const uint16_t DISP_8;
extern const uint16_t DISP_9;
extern const uint16_t DISP_A;
extern const uint16_t DISP_B;
extern const uint16_t DISP_C;
extern const uint16_t DISP_D;
extern const uint16_t DISP_E;
extern const uint16_t DISP_F;
extern const uint16_t numToPort[];
extern const uint32_t mask_for_pins;
extern const uint32_t mask_clear;
extern const uint32_t Column[];
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
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
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BMA_CS_Pin GPIO_PIN_6
#define BMA_CS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
