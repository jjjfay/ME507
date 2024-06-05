/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MYO1_Pin GPIO_PIN_0
#define MYO1_GPIO_Port GPIOC
#define MYO2_Pin GPIO_PIN_1
#define MYO2_GPIO_Port GPIOC
#define PWMB_Pin GPIO_PIN_1
#define PWMB_GPIO_Port GPIOA
#define PWMA_Pin GPIO_PIN_5
#define PWMA_GPIO_Port GPIOA
#define STBY_Pin GPIO_PIN_6
#define STBY_GPIO_Port GPIOA
#define AIN1_Pin GPIO_PIN_7
#define AIN1_GPIO_Port GPIOA
#define AIN2_Pin GPIO_PIN_4
#define AIN2_GPIO_Port GPIOC
#define BIN1_Pin GPIO_PIN_5
#define BIN1_GPIO_Port GPIOC
#define BIN2_Pin GPIO_PIN_0
#define BIN2_GPIO_Port GPIOB
#define HAND_ENC_B_Pin GPIO_PIN_8
#define HAND_ENC_B_GPIO_Port GPIOA
#define HAND_ENC_A_Pin GPIO_PIN_9
#define HAND_ENC_A_GPIO_Port GPIOA
#define RADIO_Pin GPIO_PIN_10
#define RADIO_GPIO_Port GPIOA
#define MOTOR_ENC_A_Pin GPIO_PIN_6
#define MOTOR_ENC_A_GPIO_Port GPIOB
#define MOTOR_ENC_B_Pin GPIO_PIN_7
#define MOTOR_ENC_B_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
