#include "trap_vel_profile.h"

void velProfileCtl(VelProfileCtl *p)
{
	switch (p->state)
	{
		case PROFILE_IDLE:
			if (*(p->flgs.pAccStartFlg))
			{
				*(p->flgs.pAccStartFlg) = 0;
				p->state = PROFILE_ACC;
				p->flgs.dccFinished = 0;
			}
			p->outputFreq = p->curFreq = 1;
			break;
		case PROFILE_ACC:
			p->curFreq += p->velProfPa.accFreqStep;
//			tsCnt++;
//			freq = accFreq * tsCnt * 0.002;
			if (p->curFreq >= p->velProfPa.maxFreq)
			{
				p->curFreq = p->velProfPa.maxFreq;
				p->flgs.accFinished = 1;
				p->state = PROFILE_CONSTANT;
			}
			p->outputFreq = p->curFreq;
			break;
		case PROFILE_CONSTANT:
				if (*p->flgs.pDccStartFlg)
				{
					*p->flgs.pDccStartFlg = 0;
					p->state = PROFILE_DCC;
				}
				p->outputFreq = p->curFreq;
				break;
		case PROFILE_DCC:
			p->curFreq -= p->velProfPa.dccFreqStep;
			if (p->curFreq <= 0)
			{
				p->curFreq = 1;
				p->flgs.dccFinished = 1;				
			}
			p->outputFreq = p->curFreq;
			
			if (*p->flgs.pIdleFlg)
			{
				*p->flgs.pIdleFlg = 0;
				p->state = PROFILE_IDLE;
			}
			break;
		default:
			break;
	}
}
