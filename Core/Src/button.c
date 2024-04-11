#include "button.h"

Button button;

void buttonUpdate()
{
	button.MANU = (HAL_GPIO_ReadPin(MANU_GPIO_Port, MANU_Pin) == GPIO_PIN_RESET) ? 1 : 0;
	button.AUTO = (HAL_GPIO_ReadPin(AUTO_GPIO_Port, AUTO_Pin) == GPIO_PIN_RESET) ? 1 : 0;
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
	button.SETTING = (HAL_GPIO_ReadPin(SETTINGS_GPIO_Port, SETTINGS_Pin) == GPIO_PIN_RESET) ? 1 : 0;
	button.FORWARD = (HAL_GPIO_ReadPin(FWD_GPIO_Port, FWD_Pin) == GPIO_PIN_RESET) ? 1 : 0;
	button.BACK = (HAL_GPIO_ReadPin(BACK_GPIO_Port, BACK_Pin) == GPIO_PIN_RESET) ? 1 : 0;
	button.CONFIRM = (HAL_GPIO_ReadPin(CONF_GPIO_Port, CONF_Pin) == GPIO_PIN_RESET) ? 1 : 0;
}

