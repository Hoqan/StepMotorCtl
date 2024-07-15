#ifndef _TOUCH_GTXX_H
#define _TOUCH_GTXX_H
#include "stm32f4xx.h"


/* 表示读数据 */ 
#define I2C_M_RD		0x0001	
 /*
 * 存储I2C通讯的信息
 * @addr：  从设备的I2C设备地址	
 * @flags: 控制标志
 * @len：  读写数据的长度
 * @buf：  存储读写数据的指针
 **/
struct i2c_msg {
	uint8_t addr;		/*从设备的I2C设备地址 */
	uint16_t flags;	/*控制标志*/
	uint16_t len;		/*读写数据的长度			*/
	uint8_t *buf;		/*存储读写数据的指针	*/
};

/** 
  * @brief 触摸屏参数
  */
typedef struct
{
  /*根据触摸屏类型配置*/
  uint16_t max_width;  //触点最大值,高
  uint16_t max_height;  //触点最大值，宽

  uint16_t config_reg_addr;  	//不同类型的触摸ic配置寄存器地址不同

}TOUCH_PARAM_TypeDef;

typedef enum 
{
	GT9157=0,
	GT911=1,
  GT5688=2,
	GT917S=3,
  GT615=4,
  GT1151QM=5,
  
  TOUCH_TYPE_NUM /* TOUCH芯片类型总数 */
}TOUCH_IC;

extern TOUCH_IC touchIC;
extern const TOUCH_PARAM_TypeDef touch_param[];

// STEP_3(optional): Specify your special config info if needed
#define GTP_MAX_HEIGHT   touch_param[touchIC].max_height
#define GTP_MAX_WIDTH    touch_param[touchIC].max_width

#define GTP_INT_TRIGGER  0
#define GTP_MAX_TOUCH         5


//***************************PART3:OTHER define*********************************
#define GTP_DRIVER_VERSION          "V2.2<2014/01/14>"
#define GTP_I2C_NAME                "Goodix-TS"
#define GT91XX_CONFIG_PROC_FILE     "gt9xx_config"
#define GTP_POLL_TIME         10    
#define GTP_ADDR_LENGTH       2
#define GTP_CONFIG_MIN_LENGTH 186
#define GTP_CONFIG_MAX_LENGTH 240
#define FAIL                  0
#define SUCCESS               1
#define SWITCH_OFF            0
#define SWITCH_ON             1

//******************** For GT9XXF Start **********************//
#define GTP_REG_BAK_REF                 0x99D0
#define GTP_REG_MAIN_CLK                0x8020
#define GTP_REG_CHIP_TYPE               0x8000
#define GTP_REG_HAVE_KEY                0x804E
#define GTP_REG_MATRIX_DRVNUM           0x8069     
#define GTP_REG_MATRIX_SENNUM           0x806A
#define GTP_REG_COMMAND				0x8040

#define GTP_COMMAND_READSTATUS	    0
#define GTP_COMMAND_DIFFERENCE	    1
#define GTP_COMMAND_SOFTRESET		    2
#define GTP_COMMAND_UPDATE	    		3
#define GTP_COMMAND_CALCULATE	    4
#define GTP_COMMAND_TURNOFF	    	5




#define GTP_FL_FW_BURN              0x00
#define GTP_FL_ESD_RECOVERY         0x01
#define GTP_FL_READ_REPAIR          0x02

#define GTP_BAK_REF_SEND                0
#define GTP_BAK_REF_STORE               1
#define CFG_LOC_DRVA_NUM                29
#define CFG_LOC_DRVB_NUM                30
#define CFG_LOC_SENS_NUM                31

#define GTP_CHK_FW_MAX                  40
#define GTP_CHK_FS_MNT_MAX              300
#define GTP_BAK_REF_PATH                "/data/gtp_ref.bin"
#define GTP_MAIN_CLK_PATH               "/data/gtp_clk.bin"
#define GTP_RQST_CONFIG                 0x01
#define GTP_RQST_BAK_REF                0x02
#define GTP_RQST_RESET                  0x03
#define GTP_RQST_MAIN_CLOCK             0x04
#define GTP_RQST_RESPONDED              0x00
#define GTP_RQST_IDLE                   0xFF

//******************** For GT9XXF End **********************//
// Registers define
#define GTP_READ_COOR_ADDR    0x814E
#define GTP_REG_SLEEP         0x8040
#define GTP_REG_SENSOR_ID     0x814A
#define GTP_REG_CONFIG_DATA   touch_param[touchIC].config_reg_addr
#define GTP_REG_VERSION       0x8140

#define RESOLUTION_LOC        3
#define TRIGGER_LOC           8

#define CFG_GROUP_LEN(p_cfg_grp)  (sizeof(p_cfg_grp) / sizeof(p_cfg_grp[0]))
	

//***************************PART1:ON/OFF define*******************************


// Log define
#define GTP_INFO(fmt,arg...)           printf("<<-GTP-INFO->> "fmt"\n",##arg)
#define GTP_ERROR(fmt,arg...)          printf("<<-GTP-ERROR->> "fmt"\n",##arg)
#define GTP_DEBUG(fmt,arg...)          do{\
                                         if(GTP_DEBUG_ON)\
                                         printf("<<-GTP-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
																					}while(0)

#define GTP_DEBUG_FUNC_ON   	0

#define GTP_DEBUG_FUNC()               do{\
                                         if(GTP_DEBUG_FUNC_ON)\
                                         printf("<<-GTP-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)


//*****************************End of Part III********************************
int32_t GTP_Read_Version(void);
int32_t GTP_Init_ReadID(void);

#endif /* _GOODIX_GT9XX_H_ */
