#ifndef _TRAP_VEL_PROFILE_H
#define _TRAP_VEL_PROFILE_H

#include <stdint.h>

typedef enum
{
	PROFILE_IDLE,
	PROFILE_ACC,
	PROFILE_CONSTANT,
  PROFILE_DCC
} MotionState;

typedef struct
{
	// 外部事件
	uint8_t *pAccStartFlg;
	uint8_t *pDccStartFlg;
	uint8_t *pIdleFlg;
	
	// 状态标记
	uint8_t accFinished;
	uint8_t dccFinished;
} Flgs;

typedef struct
{
	uint32_t accFreq;
	uint32_t maxFreq;
	uint32_t dccFreq;
	
	uint32_t accFreqStep;
	uint32_t dccFreqStep;
} VelProfileParam;

typedef struct
{
	MotionState state;
	uint8_t accStartFlg;
  Flgs flgs;
	VelProfileParam velProfPa;
	int32_t curFreq;
	int32_t outputFreq;
} VelProfileCtl;

void velProfileCtl(VelProfileCtl *p);

#endif /* _TRAP_VEL_PROFILE_H */
