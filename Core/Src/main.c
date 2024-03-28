/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define Pi 3.14159
uint16_t xiFenNum = 3200;
int32_t freq = 1;	// hz
uint32_t pulNum = 0;
uint32_t maxFreq;
uint32_t accFreq;
uint8_t freqStep;
float motMaxSpd = 1;	// r/s
float motAngleRad;
float motAcc = 2;		// r/(s*s)
float reduceRatio = 2;
float whellR = 0.05;	// m
uint8_t accStartFlg = 1;	// 开始加速事件
uint8_t accFinished;		// 加速完成标志
uint8_t dccStartFlg = 0;	// 开始减速事件
uint8_t dccFinished;		// 减速完成标志
//float distance = 0.5; // m
//float realDistance = 0.4;
//float k = 1;
float jerk = 8;		// (m/(s*s*s))
float acc = 0.8; 	// m/(s*s) 0~1
float speed = 0.4;  // m/s 0.1~0.4
float distance = 1; // m
float realDistance = 0.4;
float k = 1.15;
volatile uint32_t cnt;
uint32_t zhenShu;
uint32_t xiaoShu;
uint32_t startDeVelPulNum;

typedef enum
{
	IDLE = 0U,
	ACC,
	CONSTANT,
	DCC
} MotionState;

MotionState motionState = IDLE;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

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

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	htim1.Instance->ARR = 72000000 / freq - 1;
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	

	// distance = motAngleRad / reduceRatio * whellR;
	// k = distance / realDistance;
	motAngleRad = distance / whellR * reduceRatio * k;
	motMaxSpd = speed / whellR * reduceRatio * k;
	motAcc = acc / whellR * reduceRatio * k;
//	motAngleRad = 2 * Pi * 30;
	pulNum = motAngleRad * (xiFenNum / 2 / Pi);
	maxFreq = motMaxSpd * (xiFenNum / 2 / Pi);
	accFreq = motAcc * (xiFenNum / 2 / Pi);	
	
	zhenShu = pulNum / 65535;
	xiaoShu = pulNum % 65535;
	
	freqStep = accFreq * 0.002;

	startDeVelPulNum = pulNum - (maxFreq * maxFreq / accFreq) / 2;

	if (zhenShu == 0)
		htim2.Instance->ARR = xiaoShu - 1;
	else
		htim2.Instance->ARR = 65535;
	HAL_TIM_Base_Start_IT(&htim2);	
	HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
//		if (accFinished == 1)
//		{
//			freq += freqStep;
//			if (freq >= maxFreq)
//			{
//				freq = maxFreq;
//				accFinished = 0;
//			}
//					
//			htim1.Instance->ARR = 72000000 / freq - 1;
//			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 72000000 / freq / 2 - 1);
//			HAL_Delay(2);
//		}
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void  HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)    //定时器中断回调函数
{
	if(htim == &htim2)
	{ 
		if (zhenShu == 0)
		{
			HAL_TIM_Base_Stop(&htim2);
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);			 
		}
		else if (zhenShu > 0)
		{
			cnt++;
			if (cnt == zhenShu)
			{
				htim2.Instance->ARR = xiaoShu - 1;
			}
		}
		else if (cnt == zhenShu + 1)
		{
			HAL_TIM_Base_Stop(&htim2);
			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		}
	}
	 
	if (htim == &htim3)
	{
		switch (motionState)
		{
			case IDLE:
				if (HAL_GPIO_ReadPin(IN_PULSE_GPIO_Port, IN_PULSE_Pin) == GPIO_PIN_SET)
				{
					accStartFlg = 1;
					;;
				}
				else
				{
					accStartFlg = 0;
					;;
				}
			
				if (HAL_GPIO_ReadPin(IN_DIR_GPIO_Port, IN_DIR_Pin) == GPIO_PIN_SET)
					HAL_GPIO_WritePin(OUT_DIR_GPIO_Port, OUT_DIR_Pin, GPIO_PIN_SET);
				else
					HAL_GPIO_WritePin(OUT_DIR_GPIO_Port, OUT_DIR_Pin, GPIO_PIN_RESET);
				
				if (accStartFlg)
				{			
					motionState = ACC;
					dccFinished = 0;
					accStartFlg = 0;
				}
				break;
			case ACC:
				freq += freqStep;
	//			tsCnt++;
	//			freq = accFreq * tsCnt * 0.002;
				if (freq >= maxFreq)
				{
					freq = maxFreq;
					accFinished = 1;
					motionState = CONSTANT;
				}
				htim1.Instance->CNT = 0;		
				htim1.Instance->ARR = 72000000 / freq - 1;
				__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 72000000 / freq / 2 - 1);
				break;
			case CONSTANT:
				// 由脉冲数（距离）计算减速
				{
					uint16_t tim2_cnt = htim2.Instance->CNT;
					if (cnt * 65535 + tim2_cnt >= startDeVelPulNum)
					{	
						;
						motionState = DCC;
					}
				}
				// 由时间计算减速			
				break;
			case DCC:
				freq -= freqStep;
				if (freq <= 0)
				{
					freq = 1;
					dccFinished = 1;
					cnt = 0;
					
					//motionState = IDLE;
				}
				htim1.Instance->CNT = 0;
				htim1.Instance->ARR = 72000000 / freq - 1;
				__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 72000000 / freq / 2 - 1);
				break;
			default:
				break;
		}
	}			
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
