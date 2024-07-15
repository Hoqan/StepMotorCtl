#include "oled.h"
#include "oledfont.h"

void Delay(uint32_t time)
{
	for (int i = 0; i < time; i++)
	{
		for (int j = 0; j < 1; j++) ;
	}
}

/***********************Initial code*********************/
void Initial_LY096BG30()
{
	Write_IIC_Command(0xAE);   //display off
	Write_IIC_Command(0x20);	//Set Memory Addressing Mode	
	Write_IIC_Command(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	Write_IIC_Command(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	Write_IIC_Command(0xc8);	//Set COM Output Scan Direction
	Write_IIC_Command(0x00);//---set low column address
	Write_IIC_Command(0x10);//---set high column address
	Write_IIC_Command(0x40);//--set start line address
	Write_IIC_Command(0x81);//--set contrast control register
	Write_IIC_Command(0x7f);
	Write_IIC_Command(0xa1);//--set segment re-map 0 to 127
	Write_IIC_Command(0xa6);//--set normal display
	Write_IIC_Command(0xa8);//--set multiplex ratio(1 to 64)
	Write_IIC_Command(0x3F);//
	Write_IIC_Command(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	Write_IIC_Command(0xd3);//-set display offset
	Write_IIC_Command(0x00);//-not offset
	Write_IIC_Command(0xd5);//--set display clock divide ratio/oscillator frequency
	Write_IIC_Command(0xf0);//--set divide ratio
	Write_IIC_Command(0xd9);//--set pre-charge period
	Write_IIC_Command(0x22); //
	Write_IIC_Command(0xda);//--set com pins hardware configuration
	Write_IIC_Command(0x12);
	Write_IIC_Command(0xdb);//--set vcomh
	Write_IIC_Command(0x20);//0x20,0.77xVcc
	Write_IIC_Command(0x8d);//--set DC-DC enable
	Write_IIC_Command(0x14);//
	Write_IIC_Command(0xaf);//--turn on oled panel 
}

/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{
   SCL_SET;
   //Delay_us(1);		
   SDA_SET;
   Delay(15);
   SDA_RESET;
   Delay(30);
   SCL_RESET;
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
   SDA_RESET;
   Delay(15);
   SCL_SET;
   Delay(30);
   SDA_SET;
}
/**********************************************
// IIC Write byte
**********************************************/
uint8_t Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	uint8_t Ack_Bit = 1;                    //应答信号
	for(i=0;i<8;i++)		
	{
		if(IIC_Byte & 0x80)		//1?0?
		SDA_SET;
		else
		SDA_RESET;
		//Delay_us(1);
		SCL_SET;
		Delay(30);
		SCL_RESET;
		//Delay_us(1);
		IIC_Byte<<=1;			//loop
	}
	 SDA_SET;		                //释放IIC SDA总线为主器件接收从器件产生应答信号	
	Delay(30);
	SCL_SET;                     //第9个时钟周期
	Delay(30);
	// Ack_Bit = SDA;		            //读取应答信号
	SCL_RESET;
	return Ack_Bit;	
}  
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
   Write_IIC_Byte(0x00);			//write command
   Write_IIC_Byte(IIC_Command); 
   IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			
   Write_IIC_Byte(0x40);			//write data
   Write_IIC_Byte(IIC_Data);
   IIC_Stop(); 
}

/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)		//
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)		//
{
	unsigned char j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	Write_IIC_Command(0xb0+y);
	Write_IIC_Command(((x&0xf0)>>4)|0x10);
	Write_IIC_Command((x&0x0f)|0x01); 
}

/*
	清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
*/
void OLED_Clear(void)  
{  
	unsigned char i,n;		    
	for(i=0;i<8;i++)  
	{  
		Write_IIC_Command (0xb0+i);    //设置页地址（0~7）
		Write_IIC_Command (0x00);      //设置显示位置—列低地址
		Write_IIC_Command (0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)Write_IIC_Data(0); 
	} //更新显示
}

/*
	在指定位置显示一个字符,包括部分字符
	x:0~127
	y:0~63
	mode:0,反白显示;1,正常显示				 
	size:选择字体 16/12 
*/
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//得到偏移后的值			
	
	if(x>Max_Column-1){x=0;y=y+2;}
	if(SIZE ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
		Write_IIC_Data(F8X16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		Write_IIC_Data(F8X16[c*16+i+8]);
	}
	else
	{	
		OLED_Set_Pos(x,y+1);
		for(i=0;i<6;i++)
			Write_IIC_Data(F6x8[c][i]);
	}
}

void OLED_ShowData(unsigned char x,unsigned char y,unsigned char dat)
{      	
	unsigned char d=0,i=0;	
	d=dat;		
	
	if(x>Max_Column-1){x=0;y=y+2;}
	if(SIZE ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
		Write_IIC_Data(F8X16[d*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		Write_IIC_Data(F8X16[d*16+i+8]);
	}
	else
	{	
		OLED_Set_Pos(x,y+1);
		for(i=0;i<6;i++)
			Write_IIC_Data(F6x8[d][i]);
	}
}


/*
	显示一个字符号串
*/
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{
		OLED_ShowChar(x,y,chr[j]);
		if(SIZE==16)x+=8;
		else x+=6;
		
		if(x>120){x=0;y+=1;}
			j++;
	}
}

//void OLED_ShowData(unsigned char x,unsigned char y,unsigned char *dat, unsigned char len)
//{
//	unsigned char j=0;
//	while (len--)
//	{
//		OLED_ShowChar(x,y,dat[j] + ' ');
//		if(SIZE==16)x+=8;
//		else x+=6;
//		
//		if(x>120){x=0;y+=2;}
//			j++;
//	}
//}
