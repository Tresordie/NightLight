#ifndef _MAIN_H_
#define _MAIN_H_

#define         FW_VERSION              0
#define         FW_SUB1_VERSION         1
#define         FW_SUB2_VERSION         0


#if 0
    #define     __DEBUG
    #ifdef      __DEBUG
    #define     DEBUG(format, ...)      printf("FILE: "__FILE__", LINE: %d "format"\r\n", __LINE__, ##__VA_ARGS__);
    #endif
#endif


#if 1
    #define     __DEBUG
    #ifdef      __DEBUG
    #define     DEBUG(format, ...)      printf(""format"\r\n", ##__VA_ARGS__);
    #endif
#endif


typedef enum {
    NIGHT_MODE = 1,
    PLAY_MODE,
    ALLPLAY_MODE,
    MUSIC_MODE,
    MAX_MODE,
}Lightmode_e;

#endif

