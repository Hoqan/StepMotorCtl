#ifndef _MOTION_PARAM_H
#define _MOTION_PARAM_H

#include <stdint.h>

typedef struct
{
	float jerk;			// (m/(s*s*s))
	float acc;			// m/(s*s) 0~1
	float speed;		// m/s 0.1~0.4
	float dist;			// m
	float realDistance; // m
} PanelParam;

typedef struct
{
	PanelParam *panPar;
	float equalVal;
} MotionParam;

typedef struct
{
	float acc;
	float speed;
	float dist;
	float realDist;
} SaveAxisParam;

extern MotionParam xMotPar;
extern MotionParam yMotPar;

// extern uint32_t pulNum;
// extern uint32_t maxFreq;
// extern uint32_t accFreq;
// extern uint8_t freqStep;
// extern float equalVal; // Âö³åµ±Á¿

void motionParamInit(void);
void motionParamUpd(void);

#endif /* _MOTION_PARAM_H */
