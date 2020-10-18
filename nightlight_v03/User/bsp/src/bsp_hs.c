#include "bsp_hs.h"
#include "bsp_sw_delay.h"

void bsp_InitHSEXTI(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_HS, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = HS_PIN;
    GPIO_Init(HS_PORT, &GPIO_InitStructure);

    // HS exti config
    GPIO_EXTILineConfig(HS_EXTI_PORTSRC, HS_EXTI_PINSRC);
    EXTI_InitStructure.EXTI_Line = HS_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = HS_EXTI_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void bsp_InitHS(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_HS, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = HS_PIN;
    GPIO_Init(HS_PORT, &GPIO_InitStructure);
}


u8 IsHumanDetected(void)
{
    u8 ret = 0;

    // DOUT output High level when detect human
    if(GPIO_ReadInputDataBit(HS_PORT, HS_PIN)) {
        ret = 1;
    }else {
        ret = 0;
    }

    return ret;
}


void bsp_InitLS(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_LS, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = LS_PIN;
    GPIO_Init(LS_PORT, &GPIO_InitStructure);
}

u8 IsAmbientDark(void)
{
    u8 ret = 0;

    // GL5528 resistance is 1Mohm when dark
    if(GPIO_ReadInputDataBit(LS_PORT, LS_PIN)) {
        ret = 1;
    }else {
        ret = 0;
    }

    return ret;
}


