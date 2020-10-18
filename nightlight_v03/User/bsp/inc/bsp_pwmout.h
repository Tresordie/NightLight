#ifndef __BSP_PWMOUT_H__
#define __BSP_PWMOUT_H__

#include "stm32f10x.h"

#define PWM_BLUE_PORT           GPIOA
#define PWM_BLUE_PIN            GPIO_Pin_6

#define PWM_GREEN_PORT          GPIOA
#define PWM_GREEN_PIN           GPIO_Pin_7

#define PWM_RED_PORT            GPIOB
#define PWM_RED_PIN             GPIO_Pin_0

#define PWM_TIMPRESCALER        8
#define PWM_TIMPERIOD           255


#define PWM_DEFREDPULSE         0
#define PWM_DEFGREENPULSE       0
#define PWM_DEFBLUEPULSE        0

void bsp_InitPWMOut(void);

#endif

