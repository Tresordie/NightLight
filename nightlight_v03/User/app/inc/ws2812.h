#ifndef __WS2812_H__
#define __WS2812_H__

#include "stm32f10x.h"

#define WS2812_GPIO_PORT        GPIOA
#define WS2812_GPIO_PIN         GPIO_Pin_8

#define WS2812_TIM              TIM1
#define WS2812_HIGH             17
#define WS2812_LOW              9

#define PIXEL_NUM_PER_STRIPE    30
#define ONERGB_BUFFER_SIZE      24

//#define GRBCOLOR_RED            (0x0000FF00)
//#define GRBCOLOR_GREEN          (0x00FF0000)
//#define GRBCOLOR_BLUE           (0x000000FF)
//#define GRBCOLOR_REDGREEN       (0x00FFFF00)
//#define GRBCOLOR_REDBLUE        (0x0000FFFF)
//#define GRBCOLOR_GREENBLUE      (0x00FF00FF)
//#define GRBCOLOR_WHITE          (0x00FFFFFF)

#define GRBCOLOR_RED            (0x00004F00)
#define GRBCOLOR_GREEN          (0x004F0000)
#define GRBCOLOR_BLUE           (0x0000004F)
#define GRBCOLOR_REDGREEN       (0x004F4F00)
#define GRBCOLOR_REDBLUE        (0x00004F4F)
#define GRBCOLOR_GREENBLUE      (0x004F004F)
#define GRBCOLOR_WHITE          (0x004F4F4F)
#define GRBNIGHT_WHITE          (0x002F2F2F)


typedef struct _rgbval_t{
    u8 red;
    u8 green;
    u8 blue;
}rgbval_t;

typedef struct _hsv_t{
    float h;
    float s;
    float v;
}hsv_t;

void WS2812_Show(void);
void WS2812_ClearAll(void);
void WS2812_Init(void);
void WS2812_InitTimer(void);
u32 WS2812_RGBU32Convert(rgbval_t *rgb_val);
void WS2812_SetPixelColor(u16 pixelNum, u32 GRBColor);
void WS2812_SetPixelRGB(u16 pixelNum, rgbval_t *rgb_val);
u32 WS2812_Wheel(u8 wheelPos);
void WS2812_ColorWipe(u32 color, u8 wait_time);
void WS2812_Rainbow(u8 wait_time);
void WS2812_RainbowCycle(u8 wait_time);
void WS2812_TheaterChase(u32 color, u8 wait_time);
void WS2812_TheaterChaseRainbow(u8 wait_time);
void rgb2hsv(u8 r, u8 g, u8 b, float *h, float *s, float *v);
void hsv2rgb(float h, float s, float v, u8 *r, u8 *g, u8 *b);

#endif

