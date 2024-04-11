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
#include "stm32f4xx_hal.h"

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
#define MANU_Pin GPIO_PIN_2
#define MANU_GPIO_Port GPIOE
#define BACK_Pin GPIO_PIN_3
#define BACK_GPIO_Port GPIOE
#define RIGHT_Pin GPIO_PIN_4
#define RIGHT_GPIO_Port GPIOE
#define LEFT_Pin GPIO_PIN_5
#define LEFT_GPIO_Port GPIOE
#define FWD_Pin GPIO_PIN_6
#define FWD_GPIO_Port GPIOE
#define DO2_Pin GPIO_PIN_13
#define DO2_GPIO_Port GPIOB
#define DO4_Pin GPIO_PIN_8
#define DO4_GPIO_Port GPIOC
#define DI_A_Pin GPIO_PIN_10
#define DI_A_GPIO_Port GPIOA
#define DI_B_Pin GPIO_PIN_15
#define DI_B_GPIO_Port GPIOA
#define LEFT_LMT_Pin GPIO_PIN_10
#define LEFT_LMT_GPIO_Port GPIOC
#define RIGHT_LMT_Pin GPIO_PIN_11
#define RIGHT_LMT_GPIO_Port GPIOC
#define CONF_Pin GPIO_PIN_10
#define CONF_GPIO_Port GPIOG
#define ADD_Pin GPIO_PIN_11
#define ADD_GPIO_Port GPIOG
#define SUB_Pin GPIO_PIN_12
#define SUB_GPIO_Port GPIOG
#define SETTINGS_Pin GPIO_PIN_13
#define SETTINGS_GPIO_Port GPIOG
#define IN5V_DIR_Pin GPIO_PIN_5
#define IN5V_DIR_GPIO_Port GPIOB
#define AUTO_Pin GPIO_PIN_1
#define AUTO_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
