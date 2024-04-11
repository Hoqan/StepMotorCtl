#include "pwm.h"

void setFreq(TIM_HandleTypeDef *htim, uint32_t freq)
{
	htim->Instance->CNT = 0;		
	htim->Instance->ARR = 1000000 / freq - 1;
	__HAL_TIM_SetCompare(htim, TIM_CHANNEL_1, 1000000 / freq / 2 - 1);
}
