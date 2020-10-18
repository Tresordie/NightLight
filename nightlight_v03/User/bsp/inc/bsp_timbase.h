#ifndef __BSP_TIMBASE_H__
#define __BSP_TIMBASE_H__

#include "stm32f10x.h"

// Default use TIM4 as basic timer
#define BASIC_TIM_PRESCALER     (10000 - 1)
#define BASIC_TIM_PERIOD        (7200 - 1)

void bsp_InitTIMBase(void);

#endif
