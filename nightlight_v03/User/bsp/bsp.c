#include "bsp.h"


void bsp_Init(void)
{
    bsp_InitLed();
    bsp_InitLS();
    bsp_InitHS();
    bsp_InitKeyEXTI();
    bsp_InitBatteryADC();
    bsp_InitTIMBase();
    bsp_InitUart1();
}


