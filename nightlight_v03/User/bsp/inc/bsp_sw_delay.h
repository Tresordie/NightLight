#ifndef __BSP_SW_DELAY_H__
#define __BSP_SW_DELAY_H__

#include "stm32f10x.h"

void bsp_sw_delay_us(uint32_t nTimer);
void bsp_sw_delay_ms(uint32_t nTimer);
//void SysTick_Init(u8 SYSCLK);
//void bsp_systickdelay_us(u32 nus);
//void bsp_systickdelay_ms(u16 nms);

#endif


