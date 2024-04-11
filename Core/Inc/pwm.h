#ifndef _PWM_H
#define _PWM_H

#include "tim.h"

void setFreq(TIM_HandleTypeDef *htim, uint32_t freq);


#endif /* _PWM_H */
