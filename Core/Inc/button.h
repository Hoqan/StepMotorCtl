#ifndef _BUTTON_H
#define _BUTTON_H

#include "gpio.h"

#define BUTTON_NUM 10

typedef unsigned char uint8_t;

typedef enum
{
	RELEASEED = 0U,
	PRESED
} ButtonState;

typedef struct
{
	uint8_t val;
	ButtonState state;
	uint8_t clickEvt;
} Button;

typedef union
{
	struct
	{
		Button SWI;
		Button S_RST;
		Button ADD;
		Button SUB;
		Button LEFT;
		Button RIGHT;
		Button SETTING;
		Button FORWARD;
		Button BACK;
		Button CONFIRM;
	};
	Button btns[BUTTON_NUM];
} Buttons;

extern Buttons buttons;

void buttonInit(void);

void buttonUpdate(void);

#endif /* _BUTTON_H */
