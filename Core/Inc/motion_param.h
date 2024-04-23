#ifndef _MOTION_PARAM_H
#define _MOTION_PARAM_H

#include <stdint.h>

extern float acc;
extern float speed;
extern float distance;
extern float realDistance;

extern uint32_t pulNum;
extern uint32_t maxFreq;
extern uint32_t accFreq;
extern uint8_t freqStep;
extern float equalVal;	// Âö³åµ±Á¿

void motionParamUpd(void);

#endif /* _MOTION_PARAM_H */
