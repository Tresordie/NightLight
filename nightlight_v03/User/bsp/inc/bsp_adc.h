#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__

#include "stm32f10x.h"

void bsp_InitBatteryADC(void);
void ADCReadBatteryVoltage(float *vbat);
void bsp_InitMultiADC(void);
void ADCReadSpecifiedChannel(u8 ch, float *volt);

#endif

