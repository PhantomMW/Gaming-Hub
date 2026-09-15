#ifndef LED_MATRIX_INTERFACE_H
#define LED_MATRIX_INTERFACE_H

#include "STD_Types.h"

void LED_MATRIX_Init(void);
void LED_MATRIX_SendData(u16 Copy_u16Data);
void LED_MATRIX_Clear(void);
void LED_MATRIX_SetPixel(u8 Copy_u8Row, u8 Copy_u8Column, u8 Copy_u8State);
void LED_MATRIX_SetFrame(const u8 *Copy_pu8Frame);
void LED_MATRIX_Refresh(void);
void LED_MATRIX_ShowMenu(u8 Copy_u8Selection);
void LED_MATRIX_ShowCorrect(void);
void LED_MATRIX_ShowWrong(void);
void LED_MATRIX_ShowMemory(void);
void LED_MATRIX_ShowSnake(void);
void LED_MATRIX_ShowMusic(u8 Copy_u8Level);
void LED_MATRIX_Test(void);

#endif
