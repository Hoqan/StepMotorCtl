#include "motion_param.h"
#include "trap_vel_profile.h"
#include "stm32f4xx_hal.h"

#define Pi 3.14159f
#define xiFenNum 3200
int32_t freq = 1; // hz
// uint32_t pulNum = 0;
// uint32_t maxFreq;
// uint32_t accFreq;
// uint8_t freqStep;

extern VelProfileCtl xAxis;
extern VelProfileCtl yAxis;

PanelParam xPanelPar = {
	.jerk = 8,
	.acc = 2,
	.speed = 0.2,
	.dist = 1,
	.realDistance = 0.970};

PanelParam yPanelPar = {
	.jerk = 16,
	.acc = 4,
	.speed = 0.4,
	.dist = 1,
	.realDistance = 0.970};

MotionParam xMotPar;
MotionParam yMotPar;

SaveAxisParam savedAllParam[2];
	
static uint32_t sectorError;
static FLASH_EraseInitTypeDef eraseInitStruct = 
{
  .TypeErase = FLASH_TYPEERASE_SECTORS,
  .VoltageRange = FLASH_VOLTAGE_RANGE_3,
  .Sector = 11,
  .NbSectors = 1
};

void	motionParamRead(void);	
uint32_t *flashTest = (uint32_t *)0x80E0000;
void motionParamInit()
{
	motionParamRead();
	
	if (*(uint32_t *)0x80E0000 != 0xFFFFFFFF && *(uint32_t *)0x80E0000 != 0)
	{
		xPanelPar.acc = savedAllParam[0].acc;
		xPanelPar.speed = savedAllParam[0].speed;
		xPanelPar.dist = savedAllParam[0].dist;
		xPanelPar.realDistance = savedAllParam[0].realDist;	
		
		yPanelPar.acc = savedAllParam[1].acc;
		yPanelPar.speed = savedAllParam[1].speed;
		yPanelPar.dist = savedAllParam[1].dist;
		yPanelPar.realDistance = savedAllParam[1].realDist;	
	}
	
	xMotPar.panPar = &xPanelPar;
	xMotPar.equalVal = 0;

	yMotPar.panPar = &yPanelPar;
	yMotPar.equalVal = 0;
	
	motionParamUpd();
}

void motionParamSave()
{
	uint32_t flashAddr = 0x80E0000;

	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&eraseInitStruct, &sectorError);
	for (uint8_t i = 0; i < 2; i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashAddr, *(uint32_t *)&(savedAllParam[i].acc));
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashAddr + 4, *(uint32_t *)&(savedAllParam[i].speed));
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashAddr + 8, *(uint32_t *)&(savedAllParam[i].dist));
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashAddr + 12, *(uint32_t *)&(savedAllParam[i].realDist));
		flashAddr += 16;
	}
	
	HAL_FLASH_Lock();
}

void motionParamRead()
{
	uint32_t flashAddr = 0x80E0000;
	
	for(uint8_t i = 0; i < 2; i++)
	{
		savedAllParam[i].acc = *(float *)flashAddr;
		savedAllParam[i].speed = *(float *)(flashAddr + 4);
		savedAllParam[i].dist = *(float *)(flashAddr + 8);
		savedAllParam[i].realDist = *(float *)(flashAddr + 12);
		flashAddr += 16;
	}
}

void motionParamUpd()
{
	float motMaxSpd = 1; // r/s
//	float motAngleRad;
	float motAcc = 2; // r/(s*s)
	float reduceRatio = 2;
	float whellR = 0.05; // m

//	uint32_t pulNum = 0;
	uint32_t maxFreq;
	uint32_t accFreq;
	uint8_t freqStep;

	float dist;
	float realDist;
	float speed;
	float acc;
	float equalVal; // Âö³åµ±Á¿
	float k;
	// distance = motAngleRad / reduceRatio * whellR;

	// --------------x axis motion param update--------------------//
	dist = xMotPar.panPar->dist;
	realDist = xMotPar.panPar->realDistance;
	speed = xMotPar.panPar->speed;
	acc = xMotPar.panPar->acc;

	k = dist / realDist;
//	motAngleRad = dist / whellR * reduceRatio * k;
	motMaxSpd = speed / whellR * reduceRatio * k;
	motAcc = acc / whellR * reduceRatio * k;
//	pulNum = motAngleRad * (xiFenNum / 2 / Pi);
	maxFreq = motMaxSpd * (xiFenNum / 2 / Pi);
	accFreq = motAcc * (xiFenNum / 2 / Pi);
	freqStep = accFreq * 0.002;
	equalVal = 0.001f / whellR * reduceRatio * k * (xiFenNum / 2 / Pi);

	xMotPar.equalVal = equalVal;
	xAxis.velProfPa.accFreq = accFreq;
	xAxis.velProfPa.accFreqStep = freqStep;
	xAxis.velProfPa.maxFreq = maxFreq;
	xAxis.velProfPa.dccFreq = accFreq;
	xAxis.velProfPa.dccFreqStep = freqStep;

  savedAllParam[0].acc = acc;
	savedAllParam[0].speed = speed;
  savedAllParam[0].dist = dist;
	savedAllParam[0].realDist = realDist;

	// --------------y axis motion param update--------------------//
	dist = yMotPar.panPar->dist;
	realDist = yMotPar.panPar->realDistance;
	speed = yMotPar.panPar->speed;
	acc = yMotPar.panPar->acc;

	k = dist / realDist;
//	motAngleRad = dist / whellR * reduceRatio * k;
	motMaxSpd = speed / whellR * reduceRatio * k;
	motAcc = acc / whellR * reduceRatio * k;
//	pulNum = motAngleRad * (xiFenNum / 2 / Pi);
	maxFreq = motMaxSpd * (xiFenNum / 2 / Pi);
	accFreq = motAcc * (xiFenNum / 2 / Pi);
	freqStep = accFreq * 0.002;
	equalVal = 0.001f / whellR * reduceRatio * k * (xiFenNum / 2 / Pi);

	yMotPar.equalVal = equalVal;
	yAxis.velProfPa.accFreq = accFreq;
	yAxis.velProfPa.accFreqStep = freqStep;
	yAxis.velProfPa.maxFreq = maxFreq;
	yAxis.velProfPa.dccFreq = accFreq;
	yAxis.velProfPa.dccFreqStep = freqStep;
	
	savedAllParam[1].acc = acc;
	savedAllParam[1].speed = speed;
  savedAllParam[1].dist = dist;
	savedAllParam[1].realDist = realDist;
	
	motionParamSave();
}
