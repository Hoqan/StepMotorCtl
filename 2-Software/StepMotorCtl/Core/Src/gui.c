#include <stdio.h>
#include "gui.h"
#include "button.h"
#include "./sdram/bsp_sdram.h"
#include <stdlib.h>
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_touch_gtxx.h"
#include "motion_param.h"
#include "control.h"

extern Mode mode;

#define X_START 16
#define Y_START 16

#define X_Y1LINE (X_START)
#define X_Y2LINE (Y_START + 16 * 4 + 8 * 2) // 第二根垂直线的x轴坐标
#define X_Y3LINE ((480 - X_Y2LINE - X_START) / 2 + X_Y2LINE) // 第二根垂直线的x轴坐标
#define X_Y4LINE (480 - X_START)

#define LINE_HLINE (448) // 水平线的长度
#define LINE_VLINE (238) // 垂直线的长度

#define X_S_PAR_TEXT  (X_START + 8)
#define Y_S_PAR_TEXT  (Y_START + 8)
#define X_XAXIS_TEST ((X_Y3LINE - X_Y2LINE) / 2 - 16 + X_Y2LINE)
#define Y_XAXIS_TEST (Y_S_PAR_TEXT)
#define X_YAXIS_TEST ((X_Y4LINE - X_Y3LINE) / 2 - 16 + X_Y3LINE)
#define Y_YAXIS_TEST (Y_S_PAR_TEXT)

#define A_X ((X_Y3LINE - X_Y2LINE) / 2 - 33 + X_Y2LINE)
#define A_Y (Y_S_PAR_TEXT + 34)

#define SIZE_X 11
#define SIZE_Y 16

#define ACC_Y A_Y
#define VEL_Y (A_Y + 34)
#define POS_Y (VEL_Y + 34)
#define REAL_POS_Y (POS_Y + 34)

typedef struct
{
	int8_t a;
	int8_t b;
	int8_t c;
	int8_t d;
	int8_t e;
} SingleNum;

typedef struct
{
	int16_t x;
	int16_t y;
} Pos;

typedef enum
{
	ACC = 0U,
	VEL,
	POS,
	REAL_POS
} VarType;

typedef enum
{
	A = 0U,
	B,
	C,
	D,
	E
} SingleNumType;

GuiState guiState = NORMAL;
Pos pos = {331, A_Y};
VarType varType;
SingleNumType singNumType;
static char accBuff[30];
static char velBuff[30];
static char posBuff[30];
static char realPosBuff[30];
// static char modeBuff[30];
static char equalValBuff[30];

static void sepData(uint32_t data, SingleNum *singNum)
{
	singNum->a = data / 10000;
	singNum->b = data / 1000 % 10;
	singNum->c = data / 100 % 10;
	singNum->d = data / 10 % 10;
	singNum->e = data % 10;
}

static void mergeData(uint32_t *data, SingleNum *singNum)
{
	*data = singNum->a * 10000 + singNum->b * 1000 + singNum->c * 100 + singNum->d * 10 + singNum->e;
}

static void judgeCoordinate()
{
	if (buttons.FORWARD.clickEvt)
	{
		buttons.FORWARD.clickEvt = 0;
		pos.y -= 34;
		if (pos.y < A_Y)
		{
			pos.y = A_Y + 3 * 34;
		}
	}
	else if (buttons.BACK.clickEvt)
	{
		buttons.BACK.clickEvt = 0;
		pos.y += 34;
		if (pos.y > A_Y + 3 * 34)
		{
			pos.y = A_Y;
		}
	}
	else if (buttons.LEFT.clickEvt)
	{
		buttons.LEFT.clickEvt = 0;
		pos.x -= SIZE_X;
		if (pos.x < A_X)
		{
			pos.x =  4 * SIZE_X + 331;
		}
		else if (pos.x == 331 - SIZE_X)
		{
//			pos.x =  4 * SIZE_X + 331;
			pos.x = 4 * SIZE_X + A_X;
		}
	}
	else if (buttons.RIGHT.clickEvt)
	{
		buttons.RIGHT.clickEvt = 0;
		pos.x += SIZE_X;
		if (pos.x ==  5 * SIZE_X + A_X)
		{
			pos.x = 331;
		}
		else if (pos.x >  4 * SIZE_X + 331)
		{
			pos.x = A_X;
		}
	}
}

static void judgeVar()
{
	if (pos.y == ACC_Y)
	{
		varType = ACC;
	}
	else if (pos.y == VEL_Y)
	{
		varType = VEL;
	}
	else if (pos.y == POS_Y)
	{
		varType = POS;
	}
	else if (pos.y == REAL_POS_Y)
	{
		varType = REAL_POS;
	}
}

static void judgeSingleNum()
{
	if (pos.x == 331 || pos.x == A_X)
	{
		singNumType = A;
	}
	else if (pos.x == SIZE_X + 331 || pos.x == SIZE_X + A_X)
	{
		singNumType = B;
	}
	else if (pos.x == 2 * SIZE_X + 331 || pos.x == 2 * SIZE_X + A_X)
	{
		singNumType = C;
	}
	else if (pos.x == 3 * SIZE_X + 331 || pos.x == 3 * SIZE_X + A_X)
	{
		singNumType = D;
	}
	else if (pos.x == 4 * SIZE_X + 331 || pos.x == 4 * SIZE_X + A_X)
	{
		singNumType = E;
	}
}

static void constrain(int8_t *num)
{
	*num = ((*num > 9) ? 9 : (*num < 0) ? 0
										: *num);
}

static void addSubDeal(SingleNum *singleNum)
{
	switch (singNumType)
	{
	case A:
		if (buttons.ADD.clickEvt)
		{
			buttons.ADD.clickEvt = 0;
			singleNum->a++;
		}
		else if (buttons.SUB.clickEvt)
		{
			buttons.SUB.clickEvt = 0;
			singleNum->a--;
		}
		constrain(&singleNum->a);
		break;
	case B:
		if (buttons.ADD.clickEvt)
		{
			buttons.ADD.clickEvt = 0;
			singleNum->b++;
		}
		else if (buttons.SUB.clickEvt)
		{
			buttons.SUB.clickEvt = 0;
			singleNum->b--;
		}
		constrain(&singleNum->b);
		break;
	case C:
		if (buttons.ADD.clickEvt)
		{
			buttons.ADD.clickEvt = 0;
			singleNum->c++;
		}
		else if (buttons.SUB.clickEvt)
		{
			buttons.SUB.clickEvt = 0;
			singleNum->c--;
		}
		constrain(&singleNum->c);
		break;
	case D:
		if (buttons.ADD.clickEvt)
		{
			buttons.ADD.clickEvt = 0;
			singleNum->d++;
		}
		else if (buttons.SUB.clickEvt)
		{
			buttons.SUB.clickEvt = 0;
			singleNum->d--;
		}
		constrain(&singleNum->d);
		break;
	case E:
		if (buttons.ADD.clickEvt)
		{
			buttons.ADD.clickEvt = 0;
			singleNum->e++;
		}
		else if (buttons.SUB.clickEvt)
		{
			buttons.SUB.clickEvt = 0;
			singleNum->e--;
		}
		constrain(&singleNum->e);
		break;
	default:
		break;
	}
}

void drawLineFrame()
{
	LCD_SetTextColor(LCD_COLOR_BLUE);
	/* 显示边框线条 */
	for (uint8_t i = 0; i < 8; i++)
	{
		LCD_DrawHLine(X_START,  Y_START + i * 34, LINE_HLINE);
	}

	LCD_DrawVLine(X_Y1LINE, Y_START, LINE_VLINE);
	LCD_DrawVLine(X_Y2LINE, Y_START, LINE_VLINE);
	LCD_DrawVLine(X_Y3LINE, Y_START, LINE_VLINE - 34);
	LCD_DrawVLine(X_Y4LINE, Y_START, LINE_VLINE);	
}


void guiInit(void)
{
	/* 必须先初始化触摸屏, 读取触摸芯片ID以判断不同尺寸类型的屏幕 */
//	GTP_Init_ReadID();
	cur_lcd = INCH_4_3;
	/* LCD 端口初始化 */
	LCD_Init();
	/* LCD 第一层初始化 */
	LCD_LayerInit(0, LCD_FB_START_ADDRESS, ARGB8888);
	/* LCD 第二层初始化 */
	LCD_LayerInit(1, LCD_FB_START_ADDRESS + (LCD_GetXSize() * LCD_GetYSize() * 4), ARGB8888);
	/* 使能LCD，包括开背光 */
	LCD_DisplayOn();

	/* 选择LCD第一层 */
	LCD_SelectLayer(0);

	/* 第一层清屏，显示全白 */
	LCD_Clear(LCD_COLOR_WHITE);

	/* 选择LCD第二层 */
	LCD_SelectLayer(1);

	/* 第二层清屏，显示全黑 */
	LCD_Clear(LCD_COLOR_TRANSPARENT);

	/* 配置第一和第二层的透明度,最小值为0，最大值为255*/
	LCD_SetTransparency(0, 255);
	LCD_SetTransparency(1, 0);

	/* 选择LCD第一层 */
	LCD_SelectLayer(0);

	/* 清屏，显示全白 */
	LCD_Clear(LCD_COLOR_WHITE);
	/*设置字体颜色及字体的背景颜色(此处的背景不是指LCD的背景层！注意区分)*/
	LCD_SetColors(LCD_COLOR_BLACK, LCD_COLOR_WHITE);
	/*选择字体*/
	LCD_SetFont(&Font16);
	
  drawLineFrame();
}

void guiTask()
{
	uint32_t varTmp = 0;
	SingleNum singleNum = {0};

	switch (guiState)
	{
	case NORMAL:
		if (buttons.SETTING.clickEvt)
		{
			buttons.SETTING.clickEvt = 0;
			guiState = SETTINGS;
		}
		break;
	case SETTINGS:
		if (buttons.CONFIRM.clickEvt)
		{
			buttons.CONFIRM.clickEvt = 0;
			motionParamUpd();
			LCD_Clear(LCD_COLOR_WHITE);
			drawLineFrame();
			guiState = NORMAL;
		}

		judgeCoordinate();
		judgeVar();
		judgeSingleNum();

		switch (varType)
		{
		case ACC:
		  if (pos.x >= A_X && pos.x <= A_X +4 * SIZE_X)
			{
				sepData(xMotPar.panPar->acc * 1000, &singleNum);
				addSubDeal(&singleNum);
				mergeData(&varTmp, &singleNum);
				xMotPar.panPar->acc = (float)varTmp / 1000;
			}
			else
			{
				sepData(yMotPar.panPar->acc * 1000, &singleNum);
				addSubDeal(&singleNum);
				mergeData(&varTmp, &singleNum);
				yMotPar.panPar->acc = (float)varTmp / 1000;
			}				
			break;
		case VEL:
		  if (pos.x >= A_X && pos.x <= A_X +4 * SIZE_X)
			{
				sepData(xMotPar.panPar->speed * 1000, &singleNum);
				addSubDeal(&singleNum);
				mergeData(&varTmp, &singleNum);
				xMotPar.panPar->speed = (float)varTmp / 1000;
			}
			else
			{
				sepData(yMotPar.panPar->speed * 1000, &singleNum);
				addSubDeal(&singleNum);
				mergeData(&varTmp, &singleNum);
				yMotPar.panPar->speed = (float)varTmp / 1000;
			}
			break;
		case POS:
		  if (pos.x >= A_X && pos.x <= A_X +4 * SIZE_X)
			{
				sepData(xMotPar.panPar->dist * 1000, &singleNum);
				addSubDeal(&singleNum);
				mergeData(&varTmp, &singleNum);
				xMotPar.panPar->dist = (float)varTmp / 1000;
			}				
			else
			{
				sepData(yMotPar.panPar->dist * 1000, &singleNum);
				addSubDeal(&singleNum);
				mergeData(&varTmp, &singleNum);
				yMotPar.panPar->dist = (float)varTmp / 1000;
			}				
			break;
		case REAL_POS:
		  if (pos.x >= A_X && pos.x <= A_X +4 * SIZE_X)
			{
				sepData(xMotPar.panPar->realDistance * 1000, &singleNum);
				addSubDeal(&singleNum);
				mergeData(&varTmp, &singleNum);	
				xMotPar.panPar->realDistance = (float)varTmp / 1000;
			}				
			else
			{
				sepData(yMotPar.panPar->realDistance * 1000, &singleNum);
				addSubDeal(&singleNum);
				mergeData(&varTmp, &singleNum);	
				yMotPar.panPar->realDistance = (float)varTmp / 1000;
			}				
			break;
		default:
			break;
		}

		LCD_DrawCharCH(pos.x + 1, pos.y, 11, 16, ' ' , 1);
		break;
	default:
		break;
	}

	/**
	 * -------------> X
	 * |
	 * |
	 * |
	 * Y
	 */
	LCD_SetTextColor(LCD_COLOR_BLACK);
	// X轴
	LCD_DisplayChar(X_XAXIS_TEST, Y_XAXIS_TEST, 'X');
	LCD_DrawCharCH(X_XAXIS_TEST + 16, Y_XAXIS_TEST, 16, 16, ZHOU_I, 0);
	// Y轴
	LCD_DisplayChar(X_YAXIS_TEST, Y_YAXIS_TEST, 'Y');
	LCD_DrawCharCH(X_YAXIS_TEST + 16, Y_YAXIS_TEST, 16, 16, ZHOU_I, 0);

	// 加速度
	LCD_DrawCharCH(X_S_PAR_TEXT, Y_S_PAR_TEXT + 34, 16, 16, JIA_I, 0);
	LCD_DrawCharCH(X_S_PAR_TEXT + 16, Y_S_PAR_TEXT + 34, 16, 16, SU_I, 0);
	LCD_DrawCharCH(X_S_PAR_TEXT + 32, Y_S_PAR_TEXT + 34, 16, 16, DU_I, 0);
	// 速度
	LCD_DrawCharCH(X_S_PAR_TEXT, Y_S_PAR_TEXT + 34 * 2, 16, 16, SU_I, 0);
	LCD_DrawCharCH(X_S_PAR_TEXT + 16, Y_S_PAR_TEXT + 34 * 2, 16, 16, DU_I, 0);

	// 理论距离
	LCD_DrawCharCH(X_S_PAR_TEXT, Y_S_PAR_TEXT + 34 * 3, 16, 16, LI_I, 0);
	LCD_DrawCharCH(X_S_PAR_TEXT + 16, Y_S_PAR_TEXT + 34 * 3, 16, 16, LUN_I, 0);
	LCD_DrawCharCH(X_S_PAR_TEXT + 32, Y_S_PAR_TEXT + 34 * 3, 16, 16, JU_I, 0);
	LCD_DrawCharCH(X_S_PAR_TEXT + 48, Y_S_PAR_TEXT + 34 * 3, 16, 16, JULI_L_I, 0);
	// 实际距离
	LCD_DrawCharCH(X_S_PAR_TEXT + 0, Y_S_PAR_TEXT + 34 * 4, 16, 16, SHI_I, 0);
	LCD_DrawCharCH(X_S_PAR_TEXT + 16, Y_S_PAR_TEXT + 34 * 4, 16, 16, JI_I, 0);
	LCD_DrawCharCH(X_S_PAR_TEXT + 32, Y_S_PAR_TEXT + 34 * 4, 16, 16, JU_I, 0);
	LCD_DrawCharCH(X_S_PAR_TEXT + 48, Y_S_PAR_TEXT + 34 * 4, 16, 16, JULI_L_I, 0);
	// 当量
	LCD_DrawCharCH(X_S_PAR_TEXT + 0, Y_S_PAR_TEXT + 34 * 5, 16, 16, DANG_I, 0);
	LCD_DrawCharCH(X_S_PAR_TEXT + 16 + 16, Y_S_PAR_TEXT + 34 * 5, 16, 16, LIANG_I, 0);

	// 模式
	LCD_DrawCharCH(X_S_PAR_TEXT + 0, Y_S_PAR_TEXT + 34 * 6, 16, 16, MOUSHI_M_I, 0);
	LCD_DrawCharCH(X_S_PAR_TEXT + 16, Y_S_PAR_TEXT + 34 * 6, 16, 16, MOUSHI_S_I, 0);

	sprintf(accBuff, "%5dmm", (int)(xMotPar.panPar->acc * 1000));
	sprintf(velBuff, "%5dmm", (int)(xMotPar.panPar->speed * 1000));
	sprintf(posBuff, "%5dmm", (int)(xMotPar.panPar->dist * 1000));
	sprintf(realPosBuff, "%5dmm", (int)(xMotPar.panPar->realDistance * 1000));
	sprintf(equalValBuff, "%.2f", xMotPar.equalVal);

	LCD_DisplayStringAt(A_X, A_Y, (uint8_t *)accBuff, LEFT_MODE);
	LCD_DisplayStringAt(A_X, A_Y + 34 * 1, (uint8_t *)velBuff, LEFT_MODE);
	LCD_DisplayStringAt(A_X, A_Y + 34 * 2, (uint8_t *)posBuff, LEFT_MODE);
	LCD_DisplayStringAt(A_X, A_Y + 34 * 3, (uint8_t *)realPosBuff, LEFT_MODE);
	LCD_DisplayStringAt(A_X, A_Y + 34 * 4, (uint8_t *)equalValBuff, LEFT_MODE);

	sprintf(accBuff, "%5dmm", (int)(yMotPar.panPar->acc * 1000));
	sprintf(velBuff, "%5dmm", (int)(yMotPar.panPar->speed * 1000));
	sprintf(posBuff, "%5dmm", (int)(yMotPar.panPar->dist * 1000));
	sprintf(realPosBuff, "%5dmm", (int)(yMotPar.panPar->realDistance * 1000));
	sprintf(equalValBuff, "%.2f", yMotPar.equalVal);

	LCD_DisplayStringAt(331, A_Y + 34 * 0, (uint8_t *)accBuff, LEFT_MODE);
	LCD_DisplayStringAt(331, A_Y + 34 * 1, (uint8_t *)velBuff, LEFT_MODE);
	LCD_DisplayStringAt(331, A_Y + 34 * 2, (uint8_t *)posBuff, LEFT_MODE);
	LCD_DisplayStringAt(331, A_Y + 34 * 3, (uint8_t *)realPosBuff, LEFT_MODE);
	LCD_DisplayStringAt(331, A_Y + 34 * 4, (uint8_t *)equalValBuff, LEFT_MODE);

	LCD_SetTextColor(LCD_COLOR_RED);
	if (mode == BACK_HOME)
	{
		for (uint8_t i = 0; i < 4; i++)
		{
			LCD_DrawCharCH(X_Y3LINE - 32 + i * 16, A_Y + 34 * 5, 16, 16, HUIYUANDIANZHONG_I + i, 0);
		}
	}
	else if (mode == MANU)
	{
		LCD_DrawCharCH(X_Y3LINE - 32, A_Y + 34 * 5, 16, 16, SHOU_I, 0);
		LCD_DrawCharCH(X_Y3LINE - 32 + 16, A_Y + 34 * 5, 16, 16, DONG_I, 0);
		LCD_DisplayStringAt(X_Y3LINE - 32 + 32, A_Y + 34 * 5, (uint8_t *)"        ", LEFT_MODE);
	}
	else if (mode == AUTO)
	{
		LCD_DrawCharCH(X_Y3LINE - 32, A_Y + 34 * 5, 16, 16, ZI_I, 0);
		LCD_DrawCharCH(X_Y3LINE - 32 + 16, A_Y + 34 * 5, 16, 16, DONG_I, 0);
		LCD_DisplayStringAt(X_Y3LINE - 32 + 32, A_Y + 34 * 5, (uint8_t *)"        ", LEFT_MODE);
	}
}
