#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "bsp.h"
#include "main.h"
#include "buzzer.h"
#include "Adafruit_NeoPixel.h"
#include "ws2812fx.h"


/*------- FX Library -------*/
#define TIMER_MS    5000

u32 last_change = 0;
u32 now = 0;

#define PIXEL_NUM_PER_STRIPE    30

#define GRBCOLOR_NONE           (0x00000000)
#define GRBCOLOR_RED            (0x00004F00)
#define GRBCOLOR_GREEN          (0x004F0000)
#define GRBCOLOR_BLUE           (0x0000004F)
#define GRBCOLOR_REDGREEN       (0x004F4F00)
#define GRBCOLOR_REDBLUE        (0x00004F4F)
#define GRBCOLOR_GREENBLUE      (0x004F004F)
#define GRBCOLOR_WHITE          (0x004F4F4F)
#define GRBNIGHT_WHITE          (0x001F1F1F)

volatile u8 fx_mode = FX_MODE_STATIC;

u32 matrix_leds[][6][5] = {
    { // page 0
      {RED,     RED,    RED,    RED,    RED},            // row 0
      {WHITE,   WHITE,  WHITE,  WHITE,  WHITE},          // row 1
      {BLUE,    BLUE,   BLUE,   BLUE,   BLUE},           // row 2
      {PURPLE,  PURPLE, PURPLE, PURPLE, PURPLE},         // row 3
      {GRAY,    GRAY,   GRAY,   GRAY,   GRAY},           // row 4
      {ORANGE,  ORANGE, ORANGE, ORANGE, ORANGE}          // row 5
    },
    { // page 1
      {YELLOW,  YELLOW, YELLOW, YELLOW, YELLOW},         // row 0
      {PINK,    PINK,   PINK,   PINK,   PINK},           // row 1
      {GREEN,   GREEN,  GREEN,  GREEN,  GREEN},          // row 2
      {YELLOW,  YELLOW, YELLOW, YELLOW, YELLOW},         // row 3
      {MAGENTA, MAGENTA,MAGENTA,MAGENTA,MAGENTA},        // row 4
      {PINK,    PINK,   PINK,   PINK,   PINK}            // row 5
    },
};


/** default RGB mode and effect definition */
volatile Lightmode_e   current_mode   = NIGHT_MODE;
volatile u8 Flag_DetectHuman = 0;
volatile u8 SongNum = 1;

int main(void)
{
    int16_t i;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    
    /* FX library init */
    SysTick_Init();
    WS2812FX_init(PIXEL_NUM_PER_STRIPE, NEO_RGB);
    WS2812FX_setSpeed(1000);
    configMatrix(2, 6, 5, matrix_leds);
    WS2812FX_setMode(FX_MODE_STATIC);
    WS2812FX_start();

    Buzzer_Init();
    bsp_Init();
    printf("board support package init ok.\r\n");

    while(1)
    {
        if(NIGHT_MODE == current_mode) {
            if(IsAmbientDark() && IsHumanDetected()) {
                bsp_sw_delay_ms(5000);
                if(IsHumanDetected()) {
                    Flag_DetectHuman = 1;
                    TIM_Cmd(TIM4, ENABLE);
                    WS2812FX_setMode(FX_MODE_CIRCUS_COMBUSTUS);               
                    WS2812FX_start();
                    WS2812FX_service();
                }
            }else if(IsAmbientDark() && Flag_DetectHuman) {
                WS2812FX_start();
                WS2812FX_service();
            }else {
                WS2812FX_stop();
            }
        }
        else if(PLAY_MODE == current_mode) {       
            WS2812FX_setMode(fx_mode);
            WS2812FX_start();
            WS2812FX_service();
        }else if(ALLPLAY_MODE == current_mode) {
            now = millis();
            WS2812FX_service();
            if(now - last_change > TIMER_MS) {
                WS2812FX_setMode((WS2812FX_getMode() + 1) % WS2812FX_getModeCount());
                last_change = now;
            }
        }else if(MUSIC_MODE == current_mode) {
            WS2812FX_setMode(FX_MODE_RANDOM_COLOR);
            WS2812FX_start();
            WS2812FX_service();
            if(1 == SongNum)    Play1();
            else                Play2();
        }
    }
}

