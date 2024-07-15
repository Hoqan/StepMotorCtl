/**
  ******************************************************************************
  * @file    bsp_touch_gtxx.c
  * @brief   该文件仅仅用来读取GTxxx触摸芯片的ID号,用以区分不同的RGB屏幕
  ******************************************************************************
  */ 
#include <stdio.h>
#include <string.h>
#include "./touch/bsp_touch_gtxx.h"
#include "./touch/bsp_i2c_touch.h"
#include "./lcd/bsp_lcd.h"


/* 触摸IC类型默认为5寸屏的ic */
TOUCH_IC touchIC = GT9157;		

const TOUCH_PARAM_TypeDef touch_param[TOUCH_TYPE_NUM] = 
{
  /* GT9157,5寸屏 */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8047,
  },
  /* GT911,7寸屏 */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8047,
  },
  /* GT5688,4.3寸屏 */
  {
    .max_width = 480,
    .max_height = 272,
    .config_reg_addr = 0x8050,
  },
	 /* GT917S,5寸屏 */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8050,
  },
  /* GT615,7寸屏 */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8047, //该起始地址与GT911的一样
  },

#if TOUCH_GT1151QM_LCD_4_3
  /* GT1151QM,4.3寸屏 */
  {
    .max_width = 480,
    .max_height = 272,
    .config_reg_addr = 0x8050,
  },
#elif TOUCH_GT1151QM_LCD_5
  /* GT1151QM,5寸屏 */
  {
    .max_width = 800,
    .max_height = 480,
    .config_reg_addr = 0x8050,
  },
#endif
};				

static int8_t GTP_I2C_Test(void);


/**
  * @brief   使用IIC进行数据传输
  * @param
  *		@arg i2c_msg:数据传输结构体
  *		@arg num:数据传输结构体的个数
  * @retval  正常完成的传输结构个数，若不正常，返回0xff
  */
static int I2C_Transfer( struct i2c_msg *msgs,int num)
{
	int im = 0;
	int ret = 0;

	GTP_DEBUG_FUNC();

	for (im = 0; ret == 0 && im != num; im++)
	{
		if ((msgs[im].flags&I2C_M_RD))																//根据flag判断是读数据还是写数据
		{
			ret = I2C_ReadBytes(msgs[im].addr, msgs[im].buf, msgs[im].len);		//IIC读取数据
		} else
		{
			ret = I2C_WriteBytes(msgs[im].addr,  msgs[im].buf, msgs[im].len);	//IIC写入数据
		}
	}
	if(ret)
		return ret;

	return im;   													//正常完成的传输结构个数
}

/**
  * @brief   从IIC设备中读取数据
  * @param
  *		@arg client_addr:设备地址
  *		@arg  buf[0~1]: 读取数据寄存器的起始地址
  *		@arg buf[2~len-1]: 存储读出来数据的缓冲buffer
  *		@arg len:    GTP_ADDR_LENGTH + read bytes count（寄存器地址长度+读取的数据字节数）
  * @retval  i2c_msgs传输结构体的个数，2为成功，其它为失败
  */
static int32_t GTP_I2C_Read(uint8_t client_addr, uint8_t *buf, int32_t len)
{
    struct i2c_msg msgs[2];
    int32_t ret=-1;
    int32_t retries = 0;

    GTP_DEBUG_FUNC();
    /*一个读数据的过程可以分为两个传输过程:
     * 1. IIC  写入 要读取的寄存器地址
     * 2. IIC  读取  数据
     * */

    msgs[0].flags = !I2C_M_RD;					//写入
    msgs[0].addr  = client_addr;					//IIC设备地址
    msgs[0].len   = GTP_ADDR_LENGTH;	//寄存器地址为2字节(即写入两字节的数据)
    msgs[0].buf   = &buf[0];						//buf[0~1]存储的是要读取的寄存器地址
    
    msgs[1].flags = I2C_M_RD;					//读取
    msgs[1].addr  = client_addr;					//IIC设备地址
    msgs[1].len   = len - GTP_ADDR_LENGTH;	//要读取的数据长度
    msgs[1].buf   = &buf[GTP_ADDR_LENGTH];	//buf[GTP_ADDR_LENGTH]之后的缓冲区存储读出的数据

    while(retries < 5)
    {
        ret = I2C_Transfer( msgs, 2);					//调用IIC数据传输过程函数，有2个传输过程
        if(ret == 2)break;
        retries++;
    }
    if((retries >= 5))
    {
        GTP_ERROR("I2C Read: 0x%04X, %d bytes failed, errcode: %d! Process reset.", (((uint16_t)(buf[0] << 8)) | buf[1]), len-2, ret);
    }
    return ret;
}



/**
  * @brief   向IIC设备写入数据
  * @param
  *		@arg client_addr:设备地址
  *		@arg  buf[0~1]: 要写入的数据寄存器的起始地址
  *		@arg buf[2~len-1]: 要写入的数据
  *		@arg len:    GTP_ADDR_LENGTH + write bytes count（寄存器地址长度+写入的数据字节数）
  * @retval  i2c_msgs传输结构体的个数，1为成功，其它为失败
  */
//static int32_t GTP_I2C_Write(uint8_t client_addr,uint8_t *buf,int32_t len)
//{
//    struct i2c_msg msg;
//    int32_t ret = -1;
//    int32_t retries = 0;

//    GTP_DEBUG_FUNC();
//    /*一个写数据的过程只需要一个传输过程:
//     * 1. IIC连续 写入 数据寄存器地址及数据
//     * */
//    msg.flags = !I2C_M_RD;			//写入
//    msg.addr  = client_addr;			//从设备地址
//    msg.len   = len;							//长度直接等于(寄存器地址长度+写入的数据字节数)
//    msg.buf   = buf;						//直接连续写入缓冲区中的数据(包括了寄存器地址)

//    while(retries < 5)
//    {
//        ret = I2C_Transfer(&msg, 1);	//调用IIC数据传输过程函数，1个传输过程
//        if (ret == 1)break;
//        retries++;
//    }
//    if((retries >= 5))
//    {

//        GTP_ERROR("I2C Write: 0x%04X, %d bytes failed, errcode: %d! Process reset.", (((uint16_t)(buf[0] << 8)) | buf[1]), len-2, ret);

//    }
//    return ret;
//}





static int32_t GTP_Get_Info(void)
{
    uint8_t opr_buf[6] = {0};
    int32_t ret = 0;

    uint16_t abs_x_max = GTP_MAX_WIDTH;
    uint16_t abs_y_max = GTP_MAX_HEIGHT;
    uint8_t int_trigger_type = GTP_INT_TRIGGER;
        
    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+1) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+1) & 0xFF);
    
    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 6);
    if (ret < 0)
    {
        return FAIL;
    }
    
    abs_x_max = (opr_buf[3] << 8) + opr_buf[2];
    abs_y_max = (opr_buf[5] << 8) + opr_buf[4];
    
    opr_buf[0] = (uint8_t)((GTP_REG_CONFIG_DATA+6) >> 8);
    opr_buf[1] = (uint8_t)((GTP_REG_CONFIG_DATA+6) & 0xFF);
    
    ret = GTP_I2C_Read(GTP_ADDRESS, opr_buf, 3);
    if (ret < 0)
    {
        return FAIL;
    }
    int_trigger_type = opr_buf[2] & 0x03;
    
    GTP_INFO("X_MAX = %d, Y_MAX = %d, TRIGGER = 0x%02x",
            abs_x_max,abs_y_max,int_trigger_type);
    
    return SUCCESS;    
}


/* 初始化触摸芯片,然后读取触摸芯片ID以判断不同尺寸类型的屏幕
   保存当前的液晶屏类型到变量 cur_lcd */
  
int32_t GTP_Init_ReadID(void)
{
    int32_t ret = -1;

    GTP_DEBUG_FUNC();


    I2C_Touch_Init();

    ret = GTP_I2C_Test();
    if (ret < 0)
    {
        GTP_ERROR("I2C communication ERROR!");
				return ret;
    } 
		
		//获取触摸IC的型号
    GTP_Read_Version(); 
    
    GTP_Get_Info();

    return 0;
}


/*******************************************************
Function:
    Read chip version.
Input:
    client:  i2c device
    version: buffer to keep ic firmware version
Output:
    read operation return.
        2: succeed, otherwise: failed
*******************************************************/
int32_t GTP_Read_Version(void)
{
    int32_t ret = -1;
    uint8_t buf[8] = {GTP_REG_VERSION >> 8, GTP_REG_VERSION & 0xff};    //寄存器地址

    GTP_DEBUG_FUNC();

    ret = GTP_I2C_Read(GTP_ADDRESS, buf, sizeof(buf));
    if (ret < 0)
    {
        GTP_ERROR("GTP read version failed");
        return ret;
    }

#ifdef LCD_TOUCH_IC_GT1151QM
    //GT1151QM芯片
    if (buf[2]=='1' && buf[3]=='1' && buf[4]=='5')
    {
        GTP_INFO("IC Version: %c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[7], buf[6]);
        touchIC = GT1151QM;
        
        #if TOUCH_GT1151QM_LCD_4_3
        cur_lcd = INCH_4_3;     //设置当前的液晶屏类型
        #elif TOUCH_GT1151QM_LCD_5
        cur_lcd = INCH_5;       //设置当前的液晶屏类型
        #endif
    }
    else
    {
        GTP_INFO("Unknown IC Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);
    }
#else

    if (buf[4] == '1')
    {				
				//GT911芯片
				if(buf[2] == '9' && buf[3] == '1' && buf[4] == '1')
        {
          GTP_INFO("IC1 Version: %c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[7], buf[6]);

					touchIC = GT911;
          /* 设置当前的液晶屏类型 */
          cur_lcd = INCH_7;
        }
        //GT9157芯片
        else
           GTP_INFO("Unknown IC Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);
		}
		else if (buf[4] == '5')
		{
			if( buf[2] == '9' && buf[3] == '1' && buf[4] == '5' && buf[5] == '7')
        {
          GTP_INFO("IC2 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

					touchIC = GT9157;
          /* 设置当前的液晶屏类型 */
          cur_lcd = INCH_5;
        }
        //GT615芯片
        else if( buf[2] == '6' && buf[3] == '1' && buf[4] == '5')
        {
          GTP_INFO("IC3 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

					touchIC = GT615;
          /* 设置当前的液晶屏类型 */
          cur_lcd = INCH_7;
        }
        else
           GTP_INFO("Unknown IC Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

    }    
    else if (buf[4] == '8')
    {	
				//GT5688芯片
				if(buf[2] == '5' && buf[3] == '6' && buf[4] == '8' && buf[5] == '8')
        {
          GTP_INFO("IC4 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

					touchIC = GT5688;
          /* 设置当前的液晶屏类型 */
          cur_lcd = INCH_4_3;
        }
        else
           GTP_INFO("Unknown IC Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

    }
		else if(buf[4] == '7')
    {
			  //GT917S芯片
         GTP_INFO("IC5 Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);
				
				if(buf[2] == '9' && buf[3] == '1' && buf[4] == '7' && buf[5] == 'S')
				{	
					touchIC = GT917S; 
					/* 设置当前的液晶屏类型 */
          cur_lcd = INCH_5;	
				}					
    }
    else 
       GTP_INFO("Unknown IC Version: %c%c%c%c_%02x%02x", buf[2], buf[3], buf[4], buf[5], buf[7], buf[6]);

#endif

    return ret;
}

/*******************************************************
Function:
    I2c test Function.
Input:
    client:i2c client.
Output:
    Executive outcomes.
        2: succeed, otherwise failed.
*******************************************************/
static int8_t GTP_I2C_Test( void)
{
    uint8_t test[3] = {GTP_REG_CONFIG_DATA >> 8, GTP_REG_CONFIG_DATA & 0xff};
    uint8_t retry = 0;
    int8_t ret = -1;

    GTP_DEBUG_FUNC();
  
    while(retry++ < 5)
    {
        ret = GTP_I2C_Read(GTP_ADDRESS, test, 3);
        if (ret > 0)
        {
            return ret;
        }
        GTP_ERROR("GTP i2c test failed time %d.",retry);
    }
    return ret;
}

