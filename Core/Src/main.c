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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "button.h"
#include "oled.h"
#include "gui.h"
#include "control.h"
#include "pwm.h"
#include "pulin_sig.h"
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_touch_gtxx.h"
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
  /* USER CODE BEGIN 2 */
//	HAL_Delay(100);
	    /* 必须先初始化触摸屏, 读取触摸芯片ID以判断不同尺寸类型的屏幕 */
    GTP_Init_ReadID();
		
    /* LCD 端口初始化 */ 
    LCD_Init();
    /* LCD 第一层初始化 */ 
    LCD_LayerInit(0, LCD_FB_START_ADDRESS,RGB565);
	/* LCD 第二层初始化 */ 
    LCD_LayerInit(1, LCD_FB_START_ADDRESS+(LCD_GetXSize()*LCD_GetYSize()*4),ARGB8888);
    /* 使能LCD，包括开背光 */ 
    LCD_DisplayOn(); 

    /* 选择LCD第一层 */
    LCD_SelectLayer(0);

    /* 第一层清屏，显示全黑 */ 
    LCD_Clear(LCD_COLOR_BLACK);  

    /* 选择LCD第二层 */
    LCD_SelectLayer(1);

    /* 第二层清屏，显示全黑 */ 
    LCD_Clear(LCD_COLOR_TRANSPARENT);

    /* 配置第一和第二层的透明度,最小值为0，最大值为255*/
    LCD_SetTransparency(0, 255);
    LCD_SetTransparency(1, 0);
	
    /* 选择LCD第一层 */
    LCD_SelectLayer(0);
	
	/* 清屏，显示全黑 */
	LCD_Clear(LCD_COLOR_BLACK);	
	/*设置字体颜色及字体的背景颜色(此处的背景不是指LCD的背景层！注意区分)*/
	LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	/*选择字体*/
	LCD_SetFont(&LCD_DEFAULT_FONT);
	
	LCD_DisplayStringLineEx(0,5,16,16,(uint8_t* )"F429 16*16 ",0);
//	guiInit();
//	setFreq(&htim1, 1);
//	setFreq(&htim8, 1);

//	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
//	__HAL_TIM_CLEAR_IT(&htim5, TIM_IT_UPDATE);
	
//	ctlInit();


//	HAL_TIM_Base_Start_IT(&htim2);	
//	HAL_TIM_Base_Start_IT(&htim5);
//	HAL_TIM_Base_Start_IT(&htim6);  
//  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
//	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);

	// HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//		buttonUpdate();		
//		guiTask();
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


//void  HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)    //定时器中断回调函数
//{
//	if(htim == &htim2)
//	{ 
////		if (zhenShu == 0)
////		{
////			HAL_TIM_Base_Stop(&htim2);
////			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);			 
////		}
////		else if (zhenShu > 0)
////		{
////			cnt++;
////			if (cnt == zhenShu)
////			{
////				htim2.Instance->ARR = xiaoShu - 1;
////			}
////		}
////		else if (cnt == zhenShu + 1)
////		{
////			HAL_TIM_Base_Stop(&htim2);
////			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
////		}
//	}
//	 
//	if (htim == &htim5)
//	{
////			HAL_TIM_Base_Stop(&htim2);
////			HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);		
//	}
//	
//	if (htim == &htim6)
//	{
//		ctlFixedUpd();
//		
//	}
//}

/**
  * @brief  Conversion complete callback in non blocking mode 
  * @param  htim: TIM handle
  * @retval None
  */
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
//{
//  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
//  {
//		// freqCalcHandler();
//  }
//}

//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
//{
//	if (htim == &htim3)
//	{
//		switch (capState)
//		{
//			case 0:  // 未捕获
//				capState = 1;
//			case 1:  // 捕获到下降沿
//				capState = 2;
//			  if (!yAxisStarted && yAxisStoped)
//				{
//			    yAxisStartFlg = 1;
//					yAxisStarted = 1;
//					yAxisDcced = 0;
//					yAxisStoped = 0;
//				}
//				__HAL_TIM_DISABLE(&htim3);
//				__HAL_TIM_SET_COUNTER(&htim3, 0);
//			  TIM_RESET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1);
//				TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);
//			  __HAL_TIM_ENABLE(&htim3);
//				break;
//			case 2:  // 捕获到上升沿
//				capState = 0;
//				capVal = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);	
//			  if (capVal < minCapVal)
//				{
//					minCapVal = capVal;
//					stopJudgeCnt = 0;
//				}
//				else if (capVal > minCapVal + 10)  // TODO
//				{
//					dccJudgeCnt++;
//					if (dccJudgeCnt >= 10)
//					{
//						if (!yAxisDcced)
//						{
//							yAxisDccFlg = 1;
//							yAxisDcced = 1;
//						}
//					  dccJudgeCnt = 0;
//					}
//				}
//				
//				if (capVal > 60000)
//				{
//					stopJudgeCnt++;
//					if (stopJudgeCnt >= 10)
//					{
//						stopJudgeCnt = 0;
//						if (!yAxisStoped)
//						{
//							yAxisStopFlg = 1;
//						  yAxisStoped = 1;
//							yAxisStarted = 0;
//						}
//					  
//					}
//				}
//				
//				TIM_RESET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1);
//			  TIM_SET_CAPTUREPOLARITY(&htim3, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING);
//			break;
//			default:
//				break;
//		}
//	}
//}

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
