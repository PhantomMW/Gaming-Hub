#ifndef IR_INTERFACE_H
#define IR_INTERFACE_H

#include "STD_Types.h"

#define IR_KEY_NONE       0U
#define IR_KEY_UP         1U
#define IR_KEY_DOWN       2U
#define IR_KEY_SELECT     3U

#define IR_CODE_PREV      0x00FF44BBUL
#define IR_CODE_NEXT      0x00FF40BFUL
#define IR_CODE_SELECT    0x00FF43BCUL

void IR_Init(void);
u8 IR_GetKey(void);

#endif
