#ifndef _AXIS_H
#define _AXIS_H

#include <stdint.h>

#define REDUCE_RADIO	1		// 减速比	

typedef struct
{
    /* data */
    uint32_t acc;       // 加速度 (r/(min*min)) Q31格式
    uint32_t speed;     // 速度 (r/min) Q31格式
    uint32_t pos;       // 位置 (-Pi~Pi) Q31格式
    uint32_t truns;     // 圈数 Q31格式 
} Axis;


#endif /* _AXIS_H */

