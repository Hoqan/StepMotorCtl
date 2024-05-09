#include <stdio.h>
#include "gui.h"
#include "button.h"
#include "./sdram/bsp_sdram.h"
#include <stdlib.h>
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_touch_gtxx.h"
#include "motion_param.h"

extern uint8_t mode;

#define ACC_Y 1
#define VEL_Y 2
#define POS_Y 3
#define REAL_POS_Y 4

#define A_X 60

#define SIZE_X 6

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
 Pos pos = {A_X, 1};
 VarType varType;
 SingleNumType singNumType;
static char accBuff[30];
static char velBuff[30];
static char posBuff[30];
static char realPosBuff[30];
static char modeBuff[30];
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
	*data = singNum->a * 10000 + singNum->b * 1000
					+ singNum->c * 100 + singNum->d * 10
					+ singNum->e;
}

static void judgeCoordinate()
{
	if (button.FORWARD)
	{
		pos.y--;
		if (pos.y < 1)
		{
			pos.y = 4;
		}
	}
	else if (button.BACK)
	{
		pos.y++;
		if (pos.y > 4)
		{
			pos.y = 1;
		}
	}
	else if (button.LEFT)
	{
		pos.x -= SIZE_X;
		if (pos.x < A_X)
		{
			pos.x = A_X + 4*SIZE_X;
//			pos.y--;
//			if (pos.y < 1)
//			{
//				pos.y = 3;
//			}
		}
	}
	else if (button.RIGHT)
  {	
	  pos.x += SIZE_X;
	  if (pos.x > A_X + 4*SIZE_X)
		{
			pos.x = A_X;
//			pos.y++;
//			if (pos.y > 3)
//			{
//				pos.y = 1;
//			}
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
	if (pos.x == A_X)
	{
		singNumType = A;
	}
	else if (pos.x == A_X + SIZE_X)
	{
		singNumType = B;
	}
	else if (pos.x == A_X + 2*SIZE_X)
	{
		singNumType = C;
	}
	else if (pos.x == A_X + 3*SIZE_X)
	{
		singNumType = D;
	}
	else if (pos.x == A_X + 4*SIZE_X)
	{
		singNumType = E;
	}
}

static void constrain(int8_t *num)
{
	*num = ((*num > 9) ? 9 : (*num < 0) ? 0 : *num);
}

static void addSubDeal(SingleNum *singleNum)
{
		switch (singNumType)
		{
			case A:
				if (button.ADD)
					singleNum->a++;
				else if (button.SUB)
					singleNum->a--;
				constrain(&singleNum->a);
				break;
			case B:
				if (button.ADD)
					singleNum->b++;
				else if (button.SUB)
					singleNum->b--;
				constrain(&singleNum->b);
				break;
			case C:
				if (button.ADD)
					singleNum->c++;
				else if (button.SUB)
					singleNum->c--;
				constrain(&singleNum->c);
				break;
			case D:
				if (button.ADD)
					singleNum->d++;
				else if (button.SUB)
					singleNum->d--;
				constrain(&singleNum->d);
				break;
			case E:
				if (button.ADD)
					singleNum->e++;
				else if (button.SUB)
					singleNum->e--;
				constrain(&singleNum->e);
				break;
			default:
				break;
		}
}

void guiInit(void)
{
		/* 必须先初始化触摸屏, 读取触摸芯片ID以判断不同尺寸类型的屏幕 */
    GTP_Init_ReadID();
	
    /* LCD 端口初始化 */ 
    LCD_Init();
    /* LCD 第一层初始化 */ 
    LCD_LayerInit(0, LCD_FB_START_ADDRESS,ARGB8888);
		/* LCD 第二层初始化 */ 
    LCD_LayerInit(1, LCD_FB_START_ADDRESS+(LCD_GetXSize()*LCD_GetYSize()*4),ARGB8888);
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
		LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_WHITE);
		/*选择字体*/
		LCD_SetFont(&Font16);		
}

void guiTask()
{
	uint32_t varTmp = 0;
	SingleNum singleNum = {0};
	
	switch (guiState)
	{
		case NORMAL:
			if (button.SETTING)
			{
				guiState = SETTINGS;
			}
			break;
		case SETTINGS:
			if (button.CONFIRM)
			{
				// TODO
//				OLED_Set_Pos(pos.x, pos.y);
//				Write_IIC_Data(0x00);
				motionParamUpd();  // 更新参数
				guiState = NORMAL;
			}
			
			judgeCoordinate();
			judgeVar();
			judgeSingleNum();
			
			switch (varType)
			{
				case ACC:
					sepData(acc * 1000, &singleNum);
					addSubDeal(&singleNum);
					mergeData(&varTmp, &singleNum);
					acc = (float)varTmp / 1000;
					break;
				case VEL:
					sepData(speed * 1000, &singleNum);
					addSubDeal(&singleNum);
					mergeData(&varTmp, &singleNum);
					speed = (float)varTmp / 1000;
					break;
				case POS:
					sepData(distance * 1000, &singleNum);
					addSubDeal(&singleNum);
					mergeData(&varTmp, &singleNum);
					distance = (float)varTmp / 1000;
					break;
				case REAL_POS:
					sepData(realDistance * 1000, &singleNum);
					addSubDeal(&singleNum);
					mergeData(&varTmp, &singleNum);
					realDistance = (float)varTmp / 1000;
					break;
				default:
					break;
			}
			
//			OLED_Set_Pos(pos.x, pos.y);
//			Write_IIC_Data(0xff);
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
	
	// X轴
	LCD_DisplayChar(80, 0, 'X');
	LCD_DrawCharCH(80 + 16, 0, 16, 16, ZHOU_I, 0);
	// Y轴
	LCD_DisplayChar(375, 0, 'Y');
	LCD_DrawCharCH(375 + 16, 0, 16, 16, ZHOU_I, 0);
	
	// 加速度
	LCD_DrawCharCH(0,16,16,16,JIA_I,0);
	LCD_DrawCharCH(16,16,16,16,SU_I,0);
	LCD_DrawCharCH(32,16,16,16,DU_I,0);
	// 速度
	LCD_DrawCharCH(0,32,16,16,SU_I,0);
	LCD_DrawCharCH(16,32,16,16,DU_I,0);
	
	// 理论距离
	LCD_DrawCharCH(0,64,16,16,LI_I,0);
	LCD_DrawCharCH(16,64,16,16,LUN_I,0);
	LCD_DrawCharCH(32,64,16,16,JU_I,0);
	LCD_DrawCharCH(48,64,16,16,JULI_L_I,0);
	// 实际距离
	LCD_DrawCharCH(0,80,16,16,SHI_I,0);
	LCD_DrawCharCH(16,80,16,16,JI_I,0);
	LCD_DrawCharCH(32,80,16,16,JU_I,0);
	LCD_DrawCharCH(48,80,16,16,JULI_L_I,0);
	// 当量
	LCD_DrawCharCH(144,64,16,16,DANG_I,0);
	LCD_DrawCharCH(144 + 16,64,16,16,LIANG_I,0);
	
	// 模式
	LCD_DrawCharCH(0,112,16,16,MOUSHI_M_I,0);
	LCD_DrawCharCH(16,112,16,16,MOUSHI_S_I,0);
//	LCD_DisplayStringLineEx(0,5,16,16,(uint8_t* )"F429 16*16 ",0);
//	sprintf(accBuff, "acc:     %5d ", (int)(acc*1000));
//	sprintf(velBuff, "speed:   %5d ", (int)(speed*1000));
//	sprintf(posBuff, "distant: %5d ", (int)(distance*1000));
//	sprintf(realPosBuff, "realdist:%5d ", (int)(realDistance*1000));
//	sprintf(modeBuff, "mode:    %5d", mode);
//	sprintf(equalValBuff, "dangliang:  %.2f", equalVal);

//	LCD_DisplayStringLine(0, (uint8_t *)accBuff);
//	LCD_DisplayStringLine(1, (uint8_t *)velBuff);
//	LCD_DisplayStringLine(2, (uint8_t *)posBuff);
//	LCD_DisplayStringLine(3, (uint8_t *)realPosBuff);
//	LCD_DisplayStringLine(4, (uint8_t *)modeBuff);
//	LCD_DisplayStringLine(5, (uint8_t *)equalValBuff);
	// OLED_Clear();
}
