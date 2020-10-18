#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#include "stm32f10x.h"

#define RCC_KEY             (RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO)

#define KEY1_PORT           GPIOB
#define KEY1_PIN            GPIO_Pin_5
#define KEY1_EXTI_PORTSRC   GPIO_PortSourceGPIOB
#define KEY1_EXTI_PINSRC    GPIO_PinSource5
#define KEY1_EXTI_LINE      EXTI_Line5

#define KEY2_PORT           GPIOB
#define KEY2_PIN            GPIO_Pin_6
#define KEY2_EXTI_PORTSRC   GPIO_PortSourceGPIOB
#define KEY2_EXTI_PINSRC    GPIO_PinSource6
#define KEY2_EXTI_LINE      EXTI_Line6

#define KEY_EXTI_IRQ        EXTI9_5_IRQn


void bsp_InitKey(void);
void bsp_InitKeyEXTI(void);
void bsp_InitKey1EXTI(void);
void bsp_InitKey2EXTI(void);
uint8_t bsp_IsKey1Pressed(void);
uint8_t bsp_IsKey2Pressed(void);
uint8_t bsp_IsKey3Pressed(void);

#endif

