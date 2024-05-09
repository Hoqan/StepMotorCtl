#include "pulin_sig.h"
#include "tim.h"

/* Captured Values */
uint32_t               uwIC2Value1 = 0;
uint32_t               uwIC2Value2 = 0;
uint32_t               uwDiffCapture = 0;

/* Capture index */
uint16_t               uhCaptureIndex = 0;

/* Frequency Value */
uint32_t               uwFreqSingle = 0;
uint32_t 							 uwFrequency = 0;

uint32_t maxPulInFreq;
uint32_t maxDeltaFreq = 1000;
uint32_t deltaFreq = 0;
uint32_t dccFreqTh = 100;
uint32_t startJudgeCnt;
uint32_t dccJudgeCnt;
uint32_t prevFreq;
bool freqUpdated = false;

bool isFreqStart()
{
	// static uint32_t prevFreq = 0;
	bool started = 0;
	
#if 1
	if (HAL_GPIO_ReadPin(IN5V_PUL_GPIO_Port, IN5V_PUL_Pin) == GPIO_PIN_RESET)
	{
		started = true;
	}
#else
	if (freqUpdated)
	{
		freqUpdated = 0;
		if (uwFreqSingle > prevFreq)
		{
			startJudgeCnt++;
			if (startJudgeCnt >= 10)
			{
				startJudgeCnt = 0;
				// prevFreq = 0;
				started = true;
			}
		}
		else
		{
			startJudgeCnt = 0;
		}
		prevFreq = uwFreqSingle;
		uwFreqSingle = 0;  // 用完清零		
	}
#endif
	
	return started;
}

bool isFreqDcc()
{
	static uint32_t cnt = 0;
	// static uint32_t prevFreq = 0;
	uint8_t started = 0;

#if 1
	if (HAL_GPIO_ReadPin(IN5V_PUL_GPIO_Port, IN5V_PUL_Pin) == GPIO_PIN_SET)
	{
		cnt++;
		if (cnt >= 20)
		{
			cnt = 0;
			started = 1;
		}
	}
	else
	{
		cnt = 0;
	}
#else
	if (freqUpdated)
	{
		freqUpdated = false;
		if (uwFreqSingle < prevFreq)
		{
			cnt++;
			if (cnt >= 1)
			{
				cnt = 0;
				uwFreqSingle = 0;
				prevFreq = 0;
				started = 1;
			}	
		}
		else
		{
			cnt = 0;
		}	
		prevFreq = uwFreqSingle;		
		uwFreqSingle = 0;  // 用完清零
	}
#endif
	
	return started;
}

void judgeMaxFreq()
{
//	deltaFreq = uwFreqSingle - prevFreq;
//	prevFreq = uwFreqSingle;
//	
//	if (deltaFreq < maxDeltaFreq)
//	{
//		if (uwFreqSingle > maxPulInFreq)
//		{
//			maxPulInFreq = uwFreqSingle;
//		}
//	}
}

//uint8_t isStartDcc()
//{
//	static uint32_t cnt = 0;
//	static uint32_t prevFreq = 0;
//	uint8_t started = 0;
//	
//	if (uwFreqSingle > prevFreq)
//	{
//		cnt++;
//		if (cnt >= 5)
//		{
//			cnt = 0;
//			prevFreq = 0;
//			started = 1;
//		}
//	}
//	else
//	{
//		cnt = 0;
//	}
//	
//	return started;
//}

//void freqCalcHandler()
//{
//    if(uhCaptureIndex == 0)
//    {
//      /* Get the 1st Input Capture value */
//      uwIC2Value1 = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);
//      uhCaptureIndex = 1;
//    }
//    else if(uhCaptureIndex == 1)
//    {
//      /* Get the 2nd Input Capture value */
//      uwIC2Value2 = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1); 
//      
//      /* Capture computation */
//      if (uwIC2Value2 > uwIC2Value1)
//      {
//        uwDiffCapture = (uwIC2Value2 - uwIC2Value1); 
//      }
//      else  /* (uwIC2Value2 <= uwIC2Value1) */
//      {
//        uwDiffCapture = ((0xFFFF - uwIC2Value1) + uwIC2Value2); 
//      }

//      /* Frequency computation: for this example TIMx (TIM1) is clocked by
//         2xAPB2Clk */      
//      uwFrequency = (2*HAL_RCC_GetPCLK1Freq()/168) / uwDiffCapture;
//			uwFreqSingle = uwFrequency;
//			freqUpdated = true;
//      uhCaptureIndex = 0;
//    }	
//}
