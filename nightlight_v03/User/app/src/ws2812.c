#include "ws2812.h"
#include "bsp_sw_delay.h"

/* 
 * WS2812 DIN data 1 bit corresponding to u16 data used for set TIM_OC CCR value
 */
volatile u16 PixelBuffer[PIXEL_NUM_PER_STRIPE][ONERGB_BUFFER_SIZE] = {0};         // WS2812 RGB value, 1 bit --> u16(set TIM compare value)


void WS2812_Show(void)
{
    DMA_SetCurrDataCounter(DMA1_Channel2, PIXEL_NUM_PER_STRIPE * ONERGB_BUFFER_SIZE);
    TIM_Cmd(TIM1, ENABLE);
    DMA_Cmd(DMA1_Channel2, ENABLE);
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC2));
    TIM_Cmd(TIM1, DISABLE);
    DMA_Cmd(DMA1_Channel2, DISABLE);
    DMA_ClearFlag(DMA1_FLAG_TC2);
}


void WS2812_ClearAll(void)
{
    u16 pixelIndex = 0;                     // pixel numbers
    u8  pixelRGBIndex = 0;                  // one pixel rgb data be 24 u16 size

    for(pixelIndex = 0; pixelIndex < PIXEL_NUM_PER_STRIPE; pixelIndex++){
        for(pixelRGBIndex = 0; pixelRGBIndex < ONERGB_BUFFER_SIZE; pixelRGBIndex++){
            PixelBuffer[pixelIndex][pixelRGBIndex] = WS2812_LOW;
        }
    }
    WS2812_Show();
}


void WS2812_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = WS2812_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(WS2812_GPIO_PORT, &GPIO_InitStructure );

    /* 24MHz freq, timer counter = 30 ==> period = 1.25us */
    TIM_TimeBaseStructure.TIM_Period = 29;
    TIM_TimeBaseStructure.TIM_Prescaler = (u16)(SystemCoreClock/ 24000000) - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure );

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_Cmd(TIM1, DISABLE);
    TIM_DMACmd(TIM1, TIM_DMA_CC1, ENABLE);

    /* DMA1 Channel2 --> TIM1_CH1 */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(TIM1->CCR1));
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)PixelBuffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = PIXEL_NUM_PER_STRIPE * ONERGB_BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel2, ENABLE);

    WS2812_ClearAll();
    bsp_sw_delay_ms(100);
}


void WS2812_InitTimer(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef        NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* 1s update interrupt */
    TIM_TimeBaseStructure.TIM_Period = 9999;
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM4, DISABLE);
}


u32 WS2812_RGBU32Convert(rgbval_t *rgb_val)
{
    return ((rgb_val->green << 16)|(rgb_val->red << 8)|rgb_val->blue);
}


void WS2812_SetPixelColor(u16 pixelNum, u32 GRBColor)
{
    u8 i = 0;

    if(pixelNum < PIXEL_NUM_PER_STRIPE){
        for(i = 0; i < ONERGB_BUFFER_SIZE; i++){
            PixelBuffer[pixelNum][i] = (((GRBColor << i) & 0x800000)? WS2812_HIGH : WS2812_LOW);
        }
    }
}


void WS2812_SetPixelRGB(u16 pixelNum, rgbval_t *rgb_val)
{
    u8 i = 0;

    if(pixelNum < PIXEL_NUM_PER_STRIPE){
        for(i = 0; i < ONERGB_BUFFER_SIZE; i++){
            PixelBuffer[pixelNum][i] = (((WS2812_RGBU32Convert(rgb_val) << i) & 0x800000)? WS2812_HIGH : WS2812_LOW);
        }
    }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// RGB total value is 255
u32 WS2812_Wheel(u8 wheelPos)
{
    rgbval_t rgbwheel = {0};

    wheelPos = 255 - wheelPos;

    if(wheelPos < 85){
        rgbwheel.red   = 255 - wheelPos * 3;
        rgbwheel.green = 0;
        rgbwheel.blue  = wheelPos * 3;

        return WS2812_RGBU32Convert(&rgbwheel);
    }

    if(wheelPos < 170){
        wheelPos -= 85;
        rgbwheel.red   = 0;
        rgbwheel.green = wheelPos * 3;
        rgbwheel.blue  = 255 - wheelPos * 3;

        return WS2812_RGBU32Convert(&rgbwheel);
    }

    wheelPos -= 170;
    rgbwheel.red   = wheelPos * 3;
    rgbwheel.green = 255 - wheelPos * 3;
    rgbwheel.blue  = 0;
    return WS2812_RGBU32Convert(&rgbwheel);
}


// Fill the dots one after the other with a color
void WS2812_ColorWipe(u32 color, u8 wait_time)
{
    u16 i = 0;

    for(i = 0; i < PIXEL_NUM_PER_STRIPE; i++){
        WS2812_SetPixelColor(i, color);
        WS2812_Show();
        bsp_sw_delay_ms(wait_time);
    }
}


void WS2812_Rainbow(u8 wait_time)
{
    u16 i, j;

    for(j = 0; j < 256; j++){
        for(i = 0; i < PIXEL_NUM_PER_STRIPE; i++) {
            WS2812_SetPixelColor(i, WS2812_Wheel((i+j) & 255));
        }
        WS2812_Show();
        bsp_sw_delay_ms(wait_time);
    }
}


void WS2812_RainbowCycle(u8 wait_time)
{
    u16 i, j;

    // 5 cycles of all colors on wheel
    for(j = 0; j < 256 * 5; j++){
        for(i = 0; i < PIXEL_NUM_PER_STRIPE; i++){
            WS2812_SetPixelColor(i, WS2812_Wheel(((i * 256 / PIXEL_NUM_PER_STRIPE) + j) & 255));
        }
        WS2812_Show();
        bsp_sw_delay_ms(wait_time);
    }
}


void WS2812_TheaterChase(u32 color, u8 wait_time)
{
    u16 i;
    int j, q;

    //do 10 cycles of chasing
    for(j = 0; j < 10; j++){
        for (q = 0; q < 3; q++){
            for(i = 0; i < PIXEL_NUM_PER_STRIPE; i = i + 3){
                WS2812_SetPixelColor(i + q, color);         //turn every third pixel on
            }
            WS2812_Show();
            bsp_sw_delay_ms(wait_time);

            for(i = 0; i < PIXEL_NUM_PER_STRIPE; i = i + 3){
            WS2812_SetPixelColor(i + q, 0);                 //turn every third pixel off
            }
        }
    }
}


void WS2812_TheaterChaseRainbow(u8 wait_time)
{
    u16 i;
    int j, q;

    // cycle all 256 colors in the wheel
    for(j = 0; j < 256; j++){
        for (q = 0; q < 3; q++){
            for (i = 0; i < PIXEL_NUM_PER_STRIPE; i = i + 3){
                WS2812_SetPixelColor(i + q, WS2812_Wheel((i + j) % 255));   //turn every third pixel on
            }
            WS2812_Show();
            bsp_sw_delay_ms(wait_time);

            for(i = 0; i < PIXEL_NUM_PER_STRIPE; i = i + 3){
                WS2812_SetPixelColor(i + q, 0);                             //turn every third pixel off
            }
        }
    }
}


static float min(float a, float b, float c)
{
    float m;

    m = a < b ? a : b;
    return (m < c ? m : c);
}

static float max(float a, float b, float c)
{
    float m;

    m = a > b ? a : b;
    return (m > c ? m : c);
}
  
void rgb2hsv(u8 r, u8 g, u8 b, float *h, float *s, float *v)
{
    float red, green ,blue;
    float cmax, cmin, delta;

    red = (float)r / 255;
    green = (float)g / 255;
    blue = (float)b / 255;

    cmax = max(red, green, blue);
    cmin = min(red, green, blue);
    delta = cmax - cmin;

    /* H */
    if(delta == 0)
    {
        *h = 0;
    }
    else
    {
        if(cmax == red)
        {
            if(green >= blue)
            {
                *h = 60 * ((green - blue) / delta);
            }
            else
            {
                *h = 60 * ((green - blue) / delta) + 360;
            }
        }
        else if(cmax == green)
        {
            *h = 60 * ((blue - red) / delta + 2);
        }
        else if(cmax == blue) 
        {
            *h = 60 * ((red - green) / delta + 4);
        }
    }

    /* S */
    if(cmax == 0)
    {
        *s = 0;
    }
    else
    {
        *s = delta / cmax;
    }

    /* V */
    *v = cmax;
}


void hsv2rgb(float h, float s, float v, u8 *r, u8 *g, u8 *b)
{
    int hi = ((int)h / 60) % 6;
    float f = h * 1.0 / 60 - hi;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1- (1 - f) * s);
    switch (hi){
        case 0:
            *r = 255 * v;
            *g = 255 * t;
            *b = 255 * p;
            break;
        case 1:
            *r = 255 * q;
            *g = 255 * v;
            *b = 255 * p;
            break;
        case 2:
            *r = 255 * p;
            *g = 255 * v;
            *b = 255 * t;
            break;
        case 3:
            *r = 255 * p;
            *g = 255 * q;
            *b = 255 * v;
            break;
        case 4:
            *r = 255 * t;
            *g = 255 * p;
            *b = 255 * v;
            break;
        case 5:
            *r = 255 * v;
            *g = 255 * p;
            *b = 255 * q;
            break;
    }
}



