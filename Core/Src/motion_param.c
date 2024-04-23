#include "motion_param.h"
#include "trap_vel_profile.h"

#define Pi 3.14159f
uint16_t xiFenNum = 3200;
int32_t freq = 1;	// hz
uint32_t pulNum = 0;
uint32_t maxFreq;
uint32_t accFreq;
uint8_t freqStep;
float motMaxSpd = 1;	// r/s
float motAngleRad;
float motAcc = 2;		// r/(s*s)
float reduceRatio = 2;
float whellR = 0.05;	// m

float jerk = 8;		// (m/(s*s*s))
float acc = 2; 	// m/(s*s) 0~1
float speed = 0.2;  // m/s 0.1~0.4
float distance = 1; // m
//float realDistance = 0.8696;
float realDistance = 0.970;
float k = 1.15;
float equalVal;	// Âö³åµ±Á¿

extern VelProfileCtl yAxis;

void motionParamUpd()
{
	// distance = motAngleRad / reduceRatio * whellR;
	k = distance / realDistance;
	motAngleRad = distance / whellR * reduceRatio * k;
	motMaxSpd = speed / whellR * reduceRatio * k;
	motAcc = acc / whellR * reduceRatio * k;

	pulNum = motAngleRad * (xiFenNum / 2 / Pi);
	maxFreq = motMaxSpd * (xiFenNum / 2 / Pi);
	accFreq = motAcc * (xiFenNum / 2 / Pi);	
	
	freqStep = accFreq * 0.002;
	equalVal = 0.001f / whellR * reduceRatio * k * (xiFenNum / 2 / Pi);
	
	yAxis.velProfPa.accFreq = accFreq;
	yAxis.velProfPa.accFreqStep = freqStep;
	yAxis.velProfPa.maxFreq = maxFreq;
	yAxis.velProfPa.dccFreq = accFreq;
	yAxis.velProfPa.dccFreqStep = freqStep;	
}
