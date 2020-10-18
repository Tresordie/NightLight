#include "buzzer.h"
#include "bsp_sw_delay.h"


void Buzzer_Init(void)
{
    GPIO_InitTypeDef GPIO_mode;

    RCC_APB2PeriphClockCmd(BUZZER_PORT_RCC, ENABLE);

    GPIO_mode.GPIO_Pin = BUZZER_PIN;
    GPIO_mode.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_mode.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(BUZZER_PORT, &GPIO_mode);
}


void Sound(unsigned int frq)
{
    u32 n;

    if(frq != 1000)             //如果频率不为1000则按频率输出，否则只延时
    {
        n = 500000 / ((u32)frq);
        BUZZER_PIN_HIGH;
        bsp_sw_delay_us(n);
        BUZZER_PIN_LOW;
        bsp_sw_delay_us(n);
    }else{
        bsp_sw_delay_us(1000);
    }
}


void Play1(void)
{
    //红尘情歌
    uc16 tone[] = {247,262,294,330,349,392,440,294,523,587,659,698,784,1000};
    u8 music[]={5,5,6,8,7,6,5,6,13,13,5,5,6,8,7,6,5,3,13,13,2,2,3,5,3,5,6,3,2,1,6,6,5,6,5,3,6,5,13,13,
                  5,5,6,8,7,6,5,6,13,13,5,5,6,8,7,6,5,3,13,13,2,2,3,5,3,5,6,3,2,1,6,6,5,6,5,3,6,1,
              13,8,9,10,10,9,8,10,9,8,6,13,6,8,9,9,8,6,9,8,6,5,13,2,3,5,5,3,5,5,6,8,7,6,6,10,9,9,8,6,5,6,8};
    u8 time[] = {2,4,2,2,2,2,2,8,4, 4,2,4,2,2,2,2,2,8,4, 4, 2,4,2,4,2,2,4,2,2,8,2,4,2,2,2,2,2,8,4 ,4,
                2,4,2,2,2,2,2,8,4, 4,2,4,2,2,2,2,2,8,4,4,2,4,2,4,2,2,4,2,2,8,2,4,2,2,2,2,2,8,
                4,2,2,2,4,2,2,2,2,2,8,4,2,2,2,4,2,2,2,2,2,8,4,2,2,2,4,2,2,5,2,6,2,4,2,2,2,4,2,4,2,2,12};

    u32 yanshi;
    u16 i,e;

    yanshi = 10;
    for(i = 0; i < sizeof(music)/sizeof(music[0]); i++){
        for(e = 0; e < ((u16)time[i]) * tone[music[i]] / yanshi; e++){
            Sound((u32)tone[music[i]]);
        }
    }
}


void Play2(void)
{
    //生日快乐
	u16  tone[]  = {
				 523, 587, 659, 698, 784, 880, 988, 1046 };
                 /*   	         低5  低6  低7   中1  中2  中3  中4  中5
		  *               0    1    2    3    4    5    6    7   
		  *这里最容易出错的地方是我把数组中的第一个元素当成了1而不是0*/
		u16  music[] = {
				 0,0,1,0,3,2,
				 0,0,1,0,4,3,
				 0,0,7,5,3,2,1,
				 6,6,5,3,4,3
		};

        u16  time[]  = {
				 2,2,4,4,4,8,
				 2,2,4,4,4,8,
				 2,2,4,4,4,4,4,
				 2,2,4,4,4,8	
		};


    u32 yanshi;
    u16 i,e;

    yanshi = 10;
    for(i = 0; i < sizeof(music)/sizeof(music[0]); i++){
        for(e = 0; e < ((u16)time[i]) * tone[music[i]] / yanshi; e++){
            Sound((u32)tone[music[i]]);
        }
    }
}

