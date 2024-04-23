#include "control.h"
#include "motion_param.h"
#include "trap_vel_profile.h"
#include "button.h"
#include "pwm.h"
#include "tim.h"
#include "gpio.h"
#include "pulin_sig.h"
#include "gui.h"

extern uint8_t yAxisStartFlg;
extern uint8_t yAxisDccFlg;
extern uint8_t yAxisStopFlg;

#define DIA_ACTIVE (HAL_GPIO_ReadPin(DI_A_GPIO_Port, DI_A_Pin) == GPIO_PIN_RESET) 
#define DIB_ACTIVE  (HAL_GPIO_ReadPin(DI_B_GPIO_Port, DI_B_Pin) == GPIO_PIN_RESET)

uint8_t xAccStartFlg;
uint8_t xDccStartFlg;
uint8_t xIdleFlg;
uint8_t yAccStartFlg;
uint8_t yDccStartFlg;
uint8_t yIdleFlg;

VelProfileCtl xAxis;
VelProfileCtl yAxis;

uint8_t stopJudgeState;
// uint8_t stopJudgeCnt;

typedef enum
{
	INIT = 0U,
	READY,
	MANU,
	AUTO
} State;

typedef enum
{
	XDIR_NULL = 0U,
	XDIR_LEFT,
	XDIR_RIGHT
} XDir;

State state;
uint8_t mode;
XDir xDir;
uint8_t yDir;

/**
 * @breaf 手动模式，按钮控制左右移动
 *
 */
void xAxisManuFlgInput()
{
	if (xAxis.state == PROFILE_IDLE)
	{
		if (button.LEFT)
		{
			xDir = XDIR_LEFT;
			HAL_GPIO_WritePin(DO2_GPIO_Port, DO2_Pin, GPIO_PIN_SET);
			*xAxis.flgs.pAccStartFlg = 1;
		}
		else if (button.RIGHT)
		{
			xDir = XDIR_RIGHT;
			HAL_GPIO_WritePin(DO2_GPIO_Port, DO2_Pin, GPIO_PIN_RESET);
			*xAxis.flgs.pAccStartFlg = 1;
		}
		else
		{
			xDir = XDIR_NULL;
		}
	}
	else if (xAxis.state == PROFILE_CONSTANT)
	{
		if ((!button.LEFT && (xDir == XDIR_LEFT))
				|| (!button.RIGHT && (xDir == XDIR_RIGHT)))
		{
			*xAxis.flgs.pDccStartFlg = 1;
		}
	}
  else if (xAxis.state == PROFILE_DCC)
	{
		if (xAxis.flgs.dccFinished)
		{
			xAxis.state = PROFILE_IDLE;
		}		
	}
	else
	{
		
	}
}

/**
 * @breaf 自动模式，x轴纠偏
 *
 */
void xAxisAutoFlgInput()
{
	if (xAxis.state == PROFILE_IDLE)
	{
		if ((!DIA_ACTIVE) && (!DIB_ACTIVE))
		{
			xDir = XDIR_LEFT;	// 往左纠偏
			HAL_GPIO_WritePin(DO2_GPIO_Port, DO2_Pin, GPIO_PIN_SET);
			*xAxis.flgs.pAccStartFlg = 1;
		}
		else if (DIA_ACTIVE && DIB_ACTIVE)
		{
			xDir = XDIR_RIGHT;	// 往右纠偏
			HAL_GPIO_WritePin(DO2_GPIO_Port, DO2_Pin, GPIO_PIN_RESET);
			*xAxis.flgs.pAccStartFlg = 1;
		}
		else
		{
			xDir = XDIR_NULL;
		}
	}
	else if (xAxis.state == PROFILE_ACC)
	{
		if (xDir == XDIR_LEFT)
		{
			if (!((!DIA_ACTIVE) && (!DIB_ACTIVE)))
				xAxis.state = PROFILE_CONSTANT;
		}
		else if (xDir == XDIR_RIGHT)
		{
			if (!(DIA_ACTIVE && DIB_ACTIVE))
				xAxis.state = PROFILE_CONSTANT;
		}
	}
	else if (xAxis.state == PROFILE_CONSTANT)
	{
		if (xDir == XDIR_LEFT)
		{
			if (!((!DIA_ACTIVE) && (!DIB_ACTIVE)))
				xAxis.state = PROFILE_DCC;
		}
		else if (xDir == XDIR_RIGHT)
		{
			if (!(DIA_ACTIVE && DIB_ACTIVE))
				xAxis.state = PROFILE_DCC;
		}
	}
  else if (xAxis.state == PROFILE_DCC)
	{
		if (xDir == XDIR_LEFT)
		{
			if (!((!DIA_ACTIVE) && (!DIB_ACTIVE)))
				xAxis.state = PROFILE_IDLE;
		}
		else if (xDir == XDIR_RIGHT)
		{
			if (!(DIA_ACTIVE && DIB_ACTIVE))
				xAxis.state = PROFILE_IDLE;
		}
		
		if (xAxis.flgs.dccFinished)
		{
			xAxis.state = PROFILE_IDLE;
		}	
	}
	else
	{
		
	}
}

/**
 * @breaf 手动模式，y轴前后移动
 *
 */
void yAxisManuFlgInput()
{
	if (yAxis.state == PROFILE_IDLE)
	{
		if (button.FORWARD)
		{
			yDir = 1;
			HAL_GPIO_WritePin(DO4_GPIO_Port, DO4_Pin, GPIO_PIN_SET);
			*yAxis.flgs.pAccStartFlg = 1;
		}
		else if (button.BACK)
		{
			yDir = 2;
			HAL_GPIO_WritePin(DO4_GPIO_Port, DO4_Pin, GPIO_PIN_RESET);
			*yAxis.flgs.pAccStartFlg = 1;
		}
		else
		{
			yDir = 0;
		}
	}
	else if (yAxis.state == PROFILE_CONSTANT)
	{
		if ((!button.FORWARD && (yDir == 1))
				|| (!button.BACK && (yDir == 2)))
		{
			*yAxis.flgs.pDccStartFlg = 1;
		}
	}
  else if (yAxis.state == PROFILE_DCC)
	{
		if (yAxis.flgs.dccFinished)
		{
			yAxis.state = PROFILE_IDLE;
		}
	}
	else
	{
		
	}
}

/**
 * @breaf 自动模式，y轴同步主机运动
 *
 */
void yAxisAutoFlgInput()
{
	if (yAxis.state == PROFILE_IDLE)
	{
		// Old method
		// if (HAL_GPIO_ReadPin(IN5V_PUL_GPIO_Port, IN5V_PUL_Pin) == GPIO_PIN_RESET)
		
		if (button.FORWARD)
		{
			yDir = 1;
			HAL_GPIO_WritePin(DO4_GPIO_Port, DO4_Pin, GPIO_PIN_SET);
			*yAxis.flgs.pAccStartFlg = 1;
		}
		else if (button.BACK)
		{
			yDir = 2;
			HAL_GPIO_WritePin(DO4_GPIO_Port, DO4_Pin, GPIO_PIN_RESET);
			*yAxis.flgs.pAccStartFlg = 1;
		}
		else
		{
			yDir = 0;
		}
		
		if (isFreqStart())
		{
			*yAxis.flgs.pAccStartFlg = 1;
			// 空闲状态，同步主机方向
			if (HAL_GPIO_ReadPin(IN5V_DIR_GPIO_Port, IN5V_DIR_Pin) == GPIO_PIN_SET) // TODO
				HAL_GPIO_WritePin(DO4_GPIO_Port, DO4_Pin, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(DO4_GPIO_Port, DO4_Pin, GPIO_PIN_RESET);	
		}
	}
	else if (yAxis.state == PROFILE_CONSTANT)
	{
//		uint32_t tim5_cnt = htim5.Instance->CNT;
//		uint32_t startDeVelPulNum = pulNum - (maxFreq * maxFreq / accFreq) / 2;
//		if (tim5_cnt >= startDeVelPulNum)
//		{
//			yAxis.state = PROFILE_DCC;
//		}
		if ((!button.FORWARD && (yDir == 1))
				|| (!button.BACK && (yDir == 2)))
		{
			*yAxis.flgs.pDccStartFlg = 1;
		}		
		
		if (yDir == 0)
		{
			if (isFreqDcc())
			{
				// yAxis.state = PROFILE_DCC;
				*yAxis.flgs.pDccStartFlg = 1;
			}
		}
		
	}
  else if (yAxis.state == PROFILE_DCC)
	{
//		switch (stopJudgeState)
//		{
//			case 0:
//				if (HAL_GPIO_ReadPin(IN5V_PUL_GPIO_Port, IN5V_PUL_Pin) == GPIO_PIN_SET)
//			  if (yAxisStopFlg)
//				{
//					stopJudgeState = 1;
//				}
//				break;
//			case 1:
////				if (HAL_GPIO_ReadPin(IN5V_PUL_GPIO_Port, IN5V_PUL_Pin) == GPIO_PIN_SET)				
//				{
//					stopJudgeCnt++;
//					if (stopJudgeCnt >= 10)
//					{
//						yAxis.state = PROFILE_IDLE;
//					}
//				}
//				else
//				{
//					stopJudgeCnt = 0;;
//				}
//				if (yAxisStopFlg)
//				{
//					yAxisStopFlg = 0;
//					yAxis.state = PROFILE_IDLE;
//				}
	if (yAxis.flgs.dccFinished)
	{
		yAxis.state = PROFILE_IDLE;
	}
//				break;
//			default:
//				break;
	}
	else
	{
		
	}
}

void leftRightLimit()
{
	if (HAL_GPIO_ReadPin(RIGHT_LMT_GPIO_Port, RIGHT_LMT_Pin) == GPIO_PIN_RESET)
	{
		if (xDir == XDIR_RIGHT)
		{
			xAxis.outputFreq = xAxis.curFreq = 1;
		  setFreq(&htim1, xAxis.outputFreq);
		  xAxis.state = PROFILE_IDLE;
		}
	}
	
	if (HAL_GPIO_ReadPin(LEFT_LMT_GPIO_Port, LEFT_LMT_Pin) == GPIO_PIN_RESET)
	{
		if (xDir == XDIR_LEFT)
		{
			xAxis.outputFreq = xAxis.curFreq = 1;
		  setFreq(&htim1, xAxis.outputFreq);
		  xAxis.state = PROFILE_IDLE;
		}
	}
}

void ctlInit()
{
	motionParamUpd();
	
	xAxis.curFreq = 1;
  xAxis.outputFreq = 1;
	xAxis.flgs.pAccStartFlg = &xAccStartFlg;
	xAxis.flgs.pDccStartFlg = &xDccStartFlg;
	xAxis.flgs.pIdleFlg = &xIdleFlg;
//	xAxis.velProfPa.accFreq = accFreq;
//	xAxis.velProfPa.accFreqStep = freqStep;
//	xAxis.velProfPa.maxFreq = maxFreq;
//	xAxis.velProfPa.dccFreq = accFreq;
//	xAxis.velProfPa.dccFreqStep = freqStep;
	xAxis.velProfPa.accFreq = 2 * accFreq;
	xAxis.velProfPa.accFreqStep = xAxis.velProfPa.accFreq  * 0.002;
	xAxis.velProfPa.maxFreq = 2 * maxFreq;
	xAxis.velProfPa.dccFreq = 2 * accFreq;
	xAxis.velProfPa.dccFreqStep = xAxis.velProfPa.dccFreq  * 0.002;
	
	yAxis.curFreq = 1;
	yAxis.outputFreq = 1;
	yAxis.flgs.pAccStartFlg = &yAccStartFlg;
	yAxis.flgs.pDccStartFlg = &yDccStartFlg;
	yAxis.flgs.pIdleFlg = &yIdleFlg;
	yAxis.velProfPa.accFreq = accFreq;
	yAxis.velProfPa.accFreqStep = freqStep;
	yAxis.velProfPa.maxFreq = maxFreq;
	yAxis.velProfPa.dccFreq = accFreq;
	yAxis.velProfPa.dccFreqStep = freqStep;
}

void ctlFixedUpd()
{
	if (button.MANU)
		mode = 1;
	else if (button.AUTO)
		mode = 2;
	
	switch (state)
	{
		case INIT:
				if (HAL_GPIO_ReadPin(RIGHT_LMT_GPIO_Port, RIGHT_LMT_Pin) == GPIO_PIN_SET)
				{
					// X轴向右移动
					HAL_GPIO_WritePin(DO2_GPIO_Port, DO2_Pin, GPIO_PIN_RESET);
					xAxis.curFreq += 50;
					if (xAxis.curFreq >= 6000)
						xAxis.curFreq = 6000;
					xAxis.outputFreq = xAxis.curFreq;
				}
				else
				{
					xAxis.curFreq = 1;
					xAxis.outputFreq = 1;
					mode = 1;
					state = READY;		
				}
				setFreq(&htim1, xAxis.outputFreq);
				
			break;
		case READY:
			if (mode == 1)
			{
				state = MANU;
			}
			else if (mode == 2)
			{
				state = AUTO;
			}
			break;
		case MANU:
			if (guiState == SETTINGS)
				return;
			xAxisManuFlgInput();
			//yAxisManuFlgInput();
		  yAxisAutoFlgInput();
			if (yAxis.state == PROFILE_IDLE
					&& xAxis.state == PROFILE_IDLE
					&& mode == 2)
			{
				state = AUTO;
			}
			velProfileCtl(&xAxis);
			setFreq(&htim1, xAxis.outputFreq);

			velProfileCtl(&yAxis);
			setFreq(&htim8, yAxis.outputFreq);
			break;
		case AUTO:
			if (guiState == SETTINGS)
				return;
			xAxisAutoFlgInput();
			yAxisAutoFlgInput();
		  if (yAxis.state == PROFILE_IDLE && mode == 1)
			{
				state = MANU;
			}
			velProfileCtl(&xAxis);
			setFreq(&htim1, xAxis.outputFreq);

			velProfileCtl(&yAxis);
			setFreq(&htim8, yAxis.outputFreq);
			break;
		default:
			break;
	} /* state */
	
	leftRightLimit();
}

