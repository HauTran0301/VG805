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
#include "stm32f1xx_hal.h"

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
#define RUN_Pin GPIO_PIN_0
#define RUN_GPIO_Port GPIOA
#define SPI1_SS_Pin GPIO_PIN_4
#define SPI1_SS_GPIO_Port GPIOA
#define RELAY_Pin GPIO_PIN_0
#define RELAY_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_1
#define BUZZER_GPIO_Port GPIOB
#define SW1_1_Pin GPIO_PIN_2
#define SW1_1_GPIO_Port GPIOB
#define OUT3_Pin GPIO_PIN_10
#define OUT3_GPIO_Port GPIOB
#define OUT4_Pin GPIO_PIN_11
#define OUT4_GPIO_Port GPIOB
#define OUT5_Pin GPIO_PIN_12
#define OUT5_GPIO_Port GPIOB
#define OUT6_Pin GPIO_PIN_13
#define OUT6_GPIO_Port GPIOB
#define OUT7_Pin GPIO_PIN_14
#define OUT7_GPIO_Port GPIOB
#define OUT8_Pin GPIO_PIN_15
#define OUT8_GPIO_Port GPIOB
#define SW1_2_Pin GPIO_PIN_3
#define SW1_2_GPIO_Port GPIOB
#define SW1_3_Pin GPIO_PIN_4
#define SW1_3_GPIO_Port GPIOB
#define SW1_4_Pin GPIO_PIN_5
#define SW1_4_GPIO_Port GPIOB
#define SW2_1_Pin GPIO_PIN_6
#define SW2_1_GPIO_Port GPIOB
#define SW2_2_Pin GPIO_PIN_7
#define SW2_2_GPIO_Port GPIOB
#define OUT2_Pin GPIO_PIN_8
#define OUT2_GPIO_Port GPIOB
#define OUT1_Pin GPIO_PIN_9
#define OUT1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
