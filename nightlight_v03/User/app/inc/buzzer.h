#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "stm32f10x.h"

#define BUZZER_PORT             GPIOB
#define BUZZER_PIN              GPIO_Pin_7
#define BUZZER_PORT_RCC         RCC_APB2Periph_GPIOB

#define BUZZER_PIN_HIGH         GPIO_SetBits(BUZZER_PORT, BUZZER_PIN)
#define BUZZER_PIN_LOW          GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN)


void Buzzer_Init(void);
void Sound(unsigned int frq);
void Play1(void);
void Play2(void);

#endif

