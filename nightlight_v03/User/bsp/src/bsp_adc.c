#include "bsp_adc.h"
#include "bsp_sw_delay.h"

// PA7 ---> ADC12_CH7 ---> Battety ADC
void bsp_InitBatteryADC(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef  ADC_InitStructure;

    // enable ADC system clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

    //clock for ADC (max 14MHz --> 72/6=12MHz)
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    // input of ADC (it doesn't seem to be needed, as default GPIO state is floating input)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1);

    // define ADC config
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // if need to work in continuous sampling mode, set enable
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure); // set config of ADC1

    // enable ADC
    ADC_Cmd(ADC1, ENABLE); //enable ADC1

    // ADC calibration(optional, but recommended at power on)
    ADC_ResetCalibration(ADC1); // Reset previous calibration
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);	// Start new calibration (ADC must be off at that time)
    while(ADC_GetCalibrationStatus(ADC1));
}


void ADCReadBatteryVoltage(float *vbat)
{
    u8  i = 0;
    u32 val_bat = 0;

    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_28Cycles5); // define regular conversion config
    for(i = 0; i < 8; i++) {
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
        val_bat += ADC_GetConversionValue(ADC1);
        bsp_sw_delay_ms(30); // need some time to discharge and prepare for next time sampling
    }

    val_bat = val_bat >> 3;

     *vbat = ((float)val_bat * 3.3 / 2048);

    printf("Battery voltage = %fV.\r\n", ((float)val_bat * 3.3 / 2048));
}


/**
 *   Multi channel ADC configuration
 */
void bsp_InitMultiADC(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef  ADC_InitStructure;

    // enable ADC system clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

    //clock for ADC (max 14MHz --> 72/6=12MHz)
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    // input of ADC (it doesn't seem to be needed, as default GPIO state is floating input)
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7 | GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC1);

    // define ADC config
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // if need to work in continuous sampling mode, set enable
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure); // set config of ADC1

    // enable ADC
    ADC_Cmd(ADC1, ENABLE); //enable ADC1

    // ADC calibration(optional, but recommended at power on)
    ADC_ResetCalibration(ADC1); // Reset previous calibration
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);	// Start new calibration (ADC must be off at that time)
    while(ADC_GetCalibrationStatus(ADC1));
}


void ADCReadSpecifiedChannel(u8 ch, float *volt)
{
    u8  i = 0;
    u32 val_bat = 0;

    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_28Cycles5); // define regular conversion config
    for(i = 0; i < 8; i++) {
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
        val_bat += ADC_GetConversionValue(ADC1);
        bsp_sw_delay_ms(50); // need some time to discharge and prepare for next time sampling
    }

    val_bat = val_bat >> 3;

     *volt = ((float)val_bat * 3.3 / 4096);
}


