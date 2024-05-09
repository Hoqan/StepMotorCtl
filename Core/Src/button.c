#include "button.h"

Button button;

void buttonUpdate()
{
	button.MANU = (HAL_GPIO_ReadPin(SWI_GPIO_Port, SWI_Pin) == GPIO_PIN_RESET) ? 1 : 0;
	button.AUTO = (HAL_GPIO_ReadPin(S_RST_GPIO_Port, S_RST_Pin) == GPIO_PIN_RESET) ? 1 : 0;
	button.ADD = (HAL_GPIO_ReadPin(ADD_GPIO_Port, ADD_Pin) == GPIO_PIN_RESET) ? 1 : 0;
	button.SUB = (HAL_GPIO_ReadPin(SUB_GPIO_Port, SUB_Pin) == GPIO_PIN_RESET) ? 1 : 0;
	//button.LEFT = (HAL_GPIO_ReadPin(GPIOE, LEFT_Pin) == GPIO_PIN_RESET) ? 1 : 0;
	if (HAL_GPIO_ReadPin(LEFT_GPIO_Port, LEFT_Pin) == GPIO_PIN_RESET)
	{
		button.LEFT = 1;
	}
	else
	{
		HAL_Delay(20);
		if (HAL_GPIO_ReadPin(LEFT_GPIO_Port, LEFT_Pin) == GPIO_PIN_SET)
		{
			button.LEFT = 0;
		}
	}
	button.RIGHT = (HAL_GPIO_ReadPin(RIGHT_GPIO_Port, RIGHT_Pin) == GPIO_PIN_RESET) ? 1 : 0;
	button.SETTING = (HAL_GPIO_ReadPin(SET_GPIO_Port, SET_Pin) == GPIO_PIN_RESET) ? 1 : 0;
	button.FORWARD = (HAL_GPIO_ReadPin(UP_GPIO_Port, UP_Pin) == GPIO_PIN_RESET) ? 1 : 0;
	button.BACK = (HAL_GPIO_ReadPin(DOWN_GPIO_Port, DOWN_Pin) == GPIO_PIN_RESET) ? 1 : 0;
	button.CONFIRM = (HAL_GPIO_ReadPin(CONF_GPIO_Port, CONF_Pin) == GPIO_PIN_RESET) ? 1 : 0;
}

