#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"

/* 供外部调用的函数声明 */
void bsp_InitLed(void);
void bsp_LedOn(u8 _no);
void bsp_LedOff(u8 _no);
void bsp_LedToggle(u8 _no);
u8 bsp_IsLedOn(u8 _no);

#endif

