#ifndef __BSP_H__
#define __BSP_H__

#define ENABLE_INT()    __set_PRIMASK(0)
#define DISABLE_INT()   __set_PRIMASK(1)

#define BSP_Printf      printf
//#define BSP_Printf(...)

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bsp_gpio.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_nvic.h"
#include "bsp_sw_delay.h"
#include "bsp_pwmout.h"
#include "bsp_timbase.h"
#include "bsp_uart.h"
#include "bsp_hs.h"
#include "bsp_adc.h"


void bsp_Init(void);

#endif

