#ifndef	__OLED_H
#define	__OLED_H

#include "gpio.h"

#define high 1
#define low 0 
/*************Pin Define***************/
#define SCL_SET HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_SET)
#define SCL_RESET HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_RESET)
#define SDA_SET HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_SET)
#define SDA_RESET HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_RESET)

#define SIZE 8
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

void Initial_LY096BG30(void);
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
void Picture_1(void);
void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
uint8_t Write_IIC_Byte(unsigned char IIC_Byte);
void OLED_Clear(void);
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr);
void OLED_Set_Pos(unsigned char x, unsigned char y);

#endif
