#include "control.h"
#include "motion_param.h"
#include "trap_vel_profile.h"
#include "button.h"
#include "pwm.h"
#include "tim.h"
#include "gpio.h"
#include "pulin_sig.h"
#include "gui.h"
#include "core_cm4.h"

extern uint8_t yAxisStartFlg;
extern uint8_t yAxisDccFlg;
extern uint8_t yAxisStopFlg;

#define DIA_ACTIVE (HAL_GPIO_ReadPin(DIA_GPIO_Port, DIA_Pin) == GPIO_PIN_RESET)
#define DIB_ACTIVE (HAL_GPIO_ReadPin(DIB_GPIO_Port, DIB_Pin) == GPIO_PIN_RESET)

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
	MANU_STATE,
	AUTO_STATE
} State;

typedef enum
{
	XDIR_NULL = 0U,
	XDIR_LEFT,
	XDIR_RIGHT
} XDir;

State state;
Mode mode;
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
		if (buttons.LEFT.val)
		{
			xDir = XDIR_LEFT;
			HAL_GPIO_WritePin(DO2_GPIO_Port, DO2_Pin, GPIO_PIN_SET);
			*xAxis.flgs.pAccStartFlg = 1;
		}
		else if (buttons.RIGHT.val)
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
		if ((!buttons.LEFT.val && (xDir == XDIR_LEFT)) || (!buttons.RIGHT.val && (xDir == XDIR_RIGHT)))
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
			xDir = XDIR_LEFT; // 往左纠偏
			HAL_GPIO_WritePin(DO2_GPIO_Port, DO2_Pin, GPIO_PIN_SET);
			*xAxis.flgs.pAccStartFlg = 1;
		}
		else if (DIA_ACTIVE && DIB_ACTIVE)
		{
			xDir = XDIR_RIGHT; // 往右纠偏
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
		if (buttons.FORWARD.val)
		{
			yDir = 1;
			HAL_GPIO_WritePin(DO4_GPIO_Port, DO4_Pin, GPIO_PIN_SET);
			*yAxis.flgs.pAccStartFlg = 1;
		}
		else if (buttons.BACK.val)
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
		if ((!buttons.FORWARD.val && (yDir == 1)) || (!buttons.BACK.val && (yDir == 2)))
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

		if (buttons.FORWARD.val)
		{
			yDir = 1;
			HAL_GPIO_WritePin(DO4_GPIO_Port, DO4_Pin, GPIO_PIN_SET);
			*yAxis.flgs.pAccStartFlg = 1;
		}
		else if (buttons.BACK.val)
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
		if ((!buttons.FORWARD.val && (yDir == 1)) || (!buttons.BACK.val && (yDir == 2)))
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
	motionParamInit();

	xAxis.curFreq = 1;
	xAxis.outputFreq = 1;
	xAxis.flgs.pAccStartFlg = &xAccStartFlg;
	xAxis.flgs.pDccStartFlg = &xDccStartFlg;
	xAxis.flgs.pIdleFlg = &xIdleFlg;

//	xAxis.velProfPa.accFreq = 2 * accFreq;
//	xAxis.velProfPa.accFreqStep = xAxis.velProfPa.accFreq * 0.002;
//	xAxis.velProfPa.maxFreq = 2 * maxFreq;
//	xAxis.velProfPa.dccFreq = 2 * accFreq;
//	xAxis.velProfPa.dccFreqStep = xAxis.velProfPa.dccFreq * 0.002;

	yAxis.curFreq = 1;
	yAxis.outputFreq = 1;
	yAxis.flgs.pAccStartFlg = &yAccStartFlg;
	yAxis.flgs.pDccStartFlg = &yDccStartFlg;
	yAxis.flgs.pIdleFlg = &yIdleFlg;
//	yAxis.velProfPa.accFreq = accFreq;
//	yAxis.velProfPa.accFreqStep = freqStep;
//	yAxis.velProfPa.maxFreq = maxFreq;
//	yAxis.velProfPa.dccFreq = accFreq;
//	yAxis.velProfPa.dccFreqStep = freqStep;
}

void ctlFixedUpd()
{
	if (buttons.SWI.clickEvt)
	{
		buttons.SWI.clickEvt = 0;
		mode = (mode == MANU) ? AUTO : MANU;
	}
	else if (buttons.S_RST.clickEvt)
	{
		buttons.S_RST.clickEvt = 0;
		NVIC_SystemReset();	
	}

	switch (state)
	{
	case INIT:
		if (HAL_GPIO_ReadPin(RIGHT_LMT_GPIO_Port, RIGHT_LMT_Pin) == GPIO_PIN_SET)
		{
			// X轴向右移动
			HAL_GPIO_WritePin(DO2_GPIO_Port, DO2_Pin, GPIO_PIN_RESET);
			xAxis.curFreq += 50;
			if (xAxis.curFreq >= 10000)
				xAxis.curFreq = 10000;
			xAxis.outputFreq = xAxis.curFreq;
		}
		else
		{
			xAxis.curFreq = 1;
			xAxis.outputFreq = 1;
			mode = MANU;
			state = READY;
		}
		setFreq(&htim1, xAxis.outputFreq);

		break;
	case READY:
		if (mode == MANU)
		{
			state = MANU_STATE;
		}
		else if (mode == AUTO)
		{
			state = AUTO_STATE;
		}
		break;
	case MANU_STATE:
		if (guiState == SETTINGS)
			return;
		xAxisManuFlgInput();
		// yAxisManuFlgInput();
		yAxisAutoFlgInput();
		if (yAxis.state == PROFILE_IDLE && xAxis.state == PROFILE_IDLE && mode == AUTO)
		{
			state = AUTO_STATE;
		}
		velProfileCtl(&xAxis);
		setFreq(&htim1, xAxis.outputFreq);

		velProfileCtl(&yAxis);
		setFreq(&htim8, yAxis.outputFreq);
		break;
	case AUTO_STATE:
		if (guiState == SETTINGS)
			return;
		xAxisAutoFlgInput();
		yAxisAutoFlgInput();
		if (yAxis.state == PROFILE_IDLE && mode == MANU)
		{
			state = MANU_STATE;
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
