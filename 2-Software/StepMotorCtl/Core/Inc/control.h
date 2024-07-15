#ifndef _CONTROL_H
#define _CONTROL_H

typedef enum
{
    BACK_HOME = 0U,
    MANU,
    AUTO
} Mode;

void ctlInit(void);

void ctlFixedUpd(void);

#endif /* _CONTROL_H */
