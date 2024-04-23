#ifndef _BUTTON_H
#define _BUTTON_H

#include "gpio.h"

typedef struct
{
	uint8_t MANU;
	uint8_t AUTO;
	uint8_t ADD;
	uint8_t SUB;
	uint8_t LEFT;
	uint8_t RIGHT;
	uint8_t SETTING;
	uint8_t FORWARD;
	uint8_t BACK;
	uint8_t CONFIRM;
} Button;

extern Button button;


void buttonUpdate(void);
	
#endif /* _BUTTON_H */
