#include "button.h"

typedef uint8_t (*IsPinActFunc)();

IsPinActFunc isPinAct[BUTTON_NUM];
Buttons buttons;

uint8_t SWI_PIN_ACTIVE() { return (HAL_GPIO_ReadPin(SWI_GPIO_Port, SWI_Pin) == GPIO_PIN_RESET); }
uint8_t S_RST_PIN_ACTIVE() { return (HAL_GPIO_ReadPin(S_RST_GPIO_Port, S_RST_Pin) == GPIO_PIN_RESET); }
uint8_t ADD_PIN_ACTIVE() { return (HAL_GPIO_ReadPin(ADD_GPIO_Port, ADD_Pin) == GPIO_PIN_RESET); }
uint8_t SUB_PIN_ACTIVE() { return (HAL_GPIO_ReadPin(SUB_GPIO_Port, SUB_Pin) == GPIO_PIN_RESET); }
uint8_t LEFT_PIN_ACTIVE() { return (HAL_GPIO_ReadPin(LEFT_GPIO_Port, LEFT_Pin) == GPIO_PIN_RESET); }
uint8_t RIGHT_PIN_ACTIVE() { return (HAL_GPIO_ReadPin(RIGHT_GPIO_Port, RIGHT_Pin) == GPIO_PIN_RESET); }
uint8_t SETTING_PIN_ACTIVE() { return (HAL_GPIO_ReadPin(SET_GPIO_Port, SET_Pin) == GPIO_PIN_RESET); }
uint8_t FORWARD_PIN_ACTIVE() { return (HAL_GPIO_ReadPin(UP_GPIO_Port, UP_Pin) == GPIO_PIN_RESET); }
uint8_t BACK_PIN_ACTIVE() { return (HAL_GPIO_ReadPin(DOWN_GPIO_Port, DOWN_Pin) == GPIO_PIN_RESET); }
uint8_t CONF_PIN_ACTIVE() { return (HAL_GPIO_ReadPin(CONF_GPIO_Port, CONF_Pin) == GPIO_PIN_RESET); }

void buttonInit()
{
	isPinAct[0] = SWI_PIN_ACTIVE;
	isPinAct[1] = S_RST_PIN_ACTIVE;
	isPinAct[2] = ADD_PIN_ACTIVE;
	isPinAct[3] = SUB_PIN_ACTIVE;
	isPinAct[4] = LEFT_PIN_ACTIVE;
	isPinAct[5] = RIGHT_PIN_ACTIVE;
	isPinAct[6] = SETTING_PIN_ACTIVE;
	isPinAct[7] = FORWARD_PIN_ACTIVE;
	isPinAct[8] = BACK_PIN_ACTIVE;
	isPinAct[9] = CONF_PIN_ACTIVE;
}

void buttonUpdate()
{
	for (uint8_t i = 0; i < BUTTON_NUM; i++)
	{
		if (isPinAct[i]())
		{
			HAL_Delay(20);
			buttons.btns[i].val = (isPinAct[i]()) ? 1 : 0;
		}
		else
		{
			buttons.btns[i].val = 0;
		}
	}

	for (uint8_t i = 0; i < BUTTON_NUM; i++)
	{
		switch (buttons.btns[i].state)
		{
		case RELEASEED:
			if (buttons.btns[i].val == 1)
			{
				buttons.btns[i].clickEvt = 1;
				buttons.btns[i].state = PRESED;
			}
			break;
		case PRESED:
			if (buttons.btns[i].val == 0)
			{
				buttons.btns[i].state = RELEASEED;
			}
			break;
		default:
			break;
		}
	}
}
