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
#define DOWN_Pin GPIO_PIN_3
#define DOWN_GPIO_Port GPIOE
#define SET_Pin GPIO_PIN_10
#define SET_GPIO_Port GPIOH
#define SUB_Pin GPIO_PIN_11
#define SUB_GPIO_Port GPIOH
#define ADD_Pin GPIO_PIN_12
#define ADD_GPIO_Port GPIOH
#define DO2_Pin GPIO_PIN_13
#define DO2_GPIO_Port GPIOB
#define DO4_Pin GPIO_PIN_8
#define DO4_GPIO_Port GPIOC
#define DIA_Pin GPIO_PIN_10
#define DIA_GPIO_Port GPIOA
#define CONF_Pin GPIO_PIN_14
#define CONF_GPIO_Port GPIOH
#define RIGHT_Pin GPIO_PIN_1
#define RIGHT_GPIO_Port GPIOI
#define S_RST_Pin GPIO_PIN_3
#define S_RST_GPIO_Port GPIOI
#define DIB_Pin GPIO_PIN_15
#define DIB_GPIO_Port GPIOA
#define LEFT_LMT_Pin GPIO_PIN_10
#define LEFT_LMT_GPIO_Port GPIOC
#define RIGHT_LMT_Pin GPIO_PIN_11
#define RIGHT_LMT_GPIO_Port GPIOC
#define SWI_Pin GPIO_PIN_3
#define SWI_GPIO_Port GPIOD
#define IN5V_PUL_Pin GPIO_PIN_4
#define IN5V_PUL_GPIO_Port GPIOB
#define IN5V_DIR_Pin GPIO_PIN_5
#define IN5V_DIR_GPIO_Port GPIOB
#define UP_Pin GPIO_PIN_7
#define UP_GPIO_Port GPIOB
#define LEFT_Pin GPIO_PIN_6
#define LEFT_GPIO_Port GPIOI

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
