#ifndef __BSP_HS_H__
#define __BSP_HS_H__

#include "stm32f10x.h"

#define RCC_HS             (RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO)

#define HS_PORT             GPIOA
#define HS_PIN              GPIO_Pin_5
#define HS_EXTI_PORTSRC     GPIO_PortSourceGPIOA
#define HS_EXTI_PINSRC      GPIO_PinSource5
#define HS_EXTI_LINE        EXTI_Line5
#define HS_EXTI_IRQ         EXTI9_5_IRQn


#define RCC_LS             (RCC_APB2Periph_GPIOA)

#define LS_PORT             GPIOA
#define LS_PIN              GPIO_Pin_3


void bsp_InitHS(void);
void bsp_InitHSEXTI(void);
u8 IsHumanDetected(void);
void bsp_InitLS(void);
u8 IsAmbientDark(void);

#endif
