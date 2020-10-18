#ifndef __WS2812FX_H__
#define __WS2812FX_H__

#define WS2812FX_VERSION 0x00000001

/*
  移植该库需要建立在移植好Adafruit_NeoPixel库的基础上来实现
  1.keil MDK编译器不支持二进制表示，而该库用到了二进制表示，需要用到binary.h文件
  2.为了减少库文件本身内容的修改，定义了一些方便移植的宏。
  3.当前版本移植匆忙未完全测试
 */

#include <stdint.h>
#include <stdbool.h>
#include "stm32f10x.h"
#include "binary.h"
#include "Adafruit_NeoPixel.h"


/* 为方便移植定义的宏 */
typedef char __FlashStringHelper;
#define millis GetTick
#define PROGMEM 
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define constrain(amt,low,high) ((amt)<=(low)?(low):((amt)>=(high)?(high):(amt)))
typedef bool boolean;
#define FSH(x) (__FlashStringHelper*)(x)

//#include <Adafruit_NeoPixel.h>
#define DIR_BIT     (u8)B00000001           // segrt->aux_param2 direction bit


#define DEFAULT_BRIGHTNESS (u8)30
#define DEFAULT_MODE       (u8)0
#define DEFAULT_SPEED      (u16)1000
#define DEFAULT_COLOR      (u32)0xFFFFFF
#define DEFAULT_COLORS     {RED, GREEN, BLUE}
#define COLORS(...)        (const u32[]){__VA_ARGS__}

#if defined(ESP8266) || defined(ESP32)
  //#pragma message("Compiling for ESP")
  #define SPEED_MIN     (u16)2
#else
  //#pragma message("Compiling for Arduino")
  #define SPEED_MIN     (u16)10
#endif
#define SPEED_MAX       (u16)65535

#define BRIGHTNESS_MIN  (u8)0
#define BRIGHTNESS_MAX  (u8)255

/* each segment uses 36 bytes of SRAM memory, so if you're compile fails
  because of insufficient flash memory, decreasing MAX_NUM_SEGMENTS may help */
#define MAX_NUM_SEGMENTS            10
#define MAX_NUM_ACTIVE_SEGMENTS     10
#define INACTIVE_SEGMENT            255 /* max uint_8 */
#define MAX_NUM_COLORS              3   /* number of colors per segment */
#define MAX_CUSTOM_MODES            8

// some common colors
#define RED        (u32)0xFF0000
#define GREEN      (u32)0x00FF00
#define BLUE       (u32)0x0000FF
#define WHITE      (u32)0xFFFFFF
#define BLACK      (u32)0x000000
#define YELLOW     (u32)0xFFFF00
#define CYAN       (u32)0x00FFFF
#define MAGENTA    (u32)0xFF00FF
#define PURPLE     (u32)0x400080
#define ORANGE     (u32)0xFF3000
#define PINK       (u32)0xFF1493
#define GRAY       (u32)0x101010
#define ULTRAWHITE (u32)0xFFFFFFFF
#define DIM(c)     (u32)((c >> 2) & 0x3f3f3f3f) // color at 25% intensity
#define DARK(c)    (u32)((c >> 4) & 0x0f0f0f0f) // color at  6% intensity


// segment options
// bit    7: reverse animation
// bits 4-6: fade rate (0-7)
// bit    3: gamma correction
// bits 1-2: size
// bits   0: TBD
#define NO_OPTIONS      (u8)B00000000
#define REVERSE         (u8)B10000000
#define IS_REVERSE      ((_seg->options & REVERSE) == REVERSE)
#define FADE_XFAST      (u8)B00010000
#define FADE_FAST       (u8)B00100000
#define FADE_MEDIUM     (u8)B00110000
#define FADE_SLOW       (u8)B01000000
#define FADE_XSLOW      (u8)B01010000
#define FADE_XXSLOW     (u8)B01100000
#define FADE_GLACIAL    (u8)B01110000
#define FADE_RATE       ((_seg->options >> 4) & 7)
#define GAMMA           (u8)B00001000
#define IS_GAMMA        ((_seg->options & GAMMA) == GAMMA)
#define SIZE_SMALL      (u8)B00000000
#define SIZE_MEDIUM     (u8)B00000010
#define SIZE_LARGE      (u8)B00000100
#define SIZE_XLARGE     (u8)B00000110
#define SIZE_OPTION     ((_seg->options >> 1) & 3)

// segment runtime options (aux_param2)
#define FRAME           ((u8)B10000000)
#define SET_FRAME       (_seg_rt->aux_param2 |=  FRAME)
#define CLR_FRAME       (_seg_rt->aux_param2 &= ~FRAME)
#define CYCLE           ((u8)B01000000)
#define SET_CYCLE       (_seg_rt->aux_param2 |=  CYCLE)
#define CLR_CYCLE       (_seg_rt->aux_param2 &= ~CYCLE)
#define CLR_FRAME_CYCLE (_seg_rt->aux_param2 &= ~(FRAME | CYCLE))

#define MODE_COUNT      71 //(sizeof(_names)/sizeof(_names[0]))

#define FX_MODE_STATIC                   0
#define FX_MODE_BLINK                    1
#define FX_MODE_BREATH                   2
#define FX_MODE_COLOR_WIPE               3
#define FX_MODE_COLOR_WIPE_INV           4 
#define FX_MODE_COLOR_WIPE_REV           5
#define FX_MODE_COLOR_WIPE_REV_INV       6
#define FX_MODE_COLOR_WIPE_RANDOM        7
#define FX_MODE_RANDOM_COLOR             8
#define FX_MODE_SINGLE_DYNAMIC           9
#define FX_MODE_MULTI_DYNAMIC           10
#define FX_MODE_RAINBOW                 11
#define FX_MODE_RAINBOW_CYCLE           12
#define FX_MODE_SCAN                    13
#define FX_MODE_DUAL_SCAN               14
#define FX_MODE_FADE                    15
#define FX_MODE_THEATER_CHASE           16
#define FX_MODE_THEATER_CHASE_RAINBOW   17
#define FX_MODE_RUNNING_LIGHTS          18
#define FX_MODE_TWINKLE                 19
#define FX_MODE_TWINKLE_RANDOM          20
#define FX_MODE_TWINKLE_FADE            21
#define FX_MODE_TWINKLE_FADE_RANDOM     22
#define FX_MODE_SPARKLE                 23
#define FX_MODE_FLASH_SPARKLE           24
#define FX_MODE_HYPER_SPARKLE           25
#define FX_MODE_STROBE                  26
#define FX_MODE_STROBE_RAINBOW          27
#define FX_MODE_MULTI_STROBE            28
#define FX_MODE_BLINK_RAINBOW           29
#define FX_MODE_CHASE_WHITE             30
#define FX_MODE_CHASE_COLOR             31
#define FX_MODE_CHASE_RANDOM            32
#define FX_MODE_CHASE_RAINBOW           33
#define FX_MODE_CHASE_FLASH             34
#define FX_MODE_CHASE_FLASH_RANDOM      35
#define FX_MODE_CHASE_RAINBOW_WHITE     36
#define FX_MODE_CHASE_BLACKOUT          37
#define FX_MODE_CHASE_BLACKOUT_RAINBOW  38
#define FX_MODE_COLOR_SWEEP_RANDOM      39
#define FX_MODE_RUNNING_COLOR           40
#define FX_MODE_RUNNING_RED_BLUE        41
#define FX_MODE_RUNNING_RANDOM          42
#define FX_MODE_LARSON_SCANNER          43
#define FX_MODE_COMET                   44
#define FX_MODE_FIREWORKS               45
#define FX_MODE_FIREWORKS_RANDOM        46
#define FX_MODE_MERRY_CHRISTMAS         47
#define FX_MODE_FIRE_FLICKER            48
#define FX_MODE_FIRE_FLICKER_SOFT       49
#define FX_MODE_FIRE_FLICKER_INTENSE    50
#define FX_MODE_CIRCUS_COMBUSTUS        51
#define FX_MODE_HALLOWEEN               52
#define FX_MODE_BICOLOR_CHASE           53
#define FX_MODE_TRICOLOR_CHASE          54
//#define FX_MODE_CUSTOM                  55  // keep this for backward compatiblity
#define FX_MODE_MATRIX                  55  // custom modes need to go at the end
#define FX_MODE_RAINBOWLARSON           56
#define FX_MODE_DUALLARSON              57
#define FX_MODE_FILLERUP                58
#define FX_MODE_ICU                     59
#define FX_MODE_MULTICOMET              60
#define FX_MODE_HEARTBEAT               61
#define FX_MODE_OSCILLATE               62
#define FX_MODE_RAIN                    63
#define FX_MODE_RAINBOWFIREWORKS        64
#define FX_MODE_RANDOMCHASE             65
#define FX_MODE_TRIFADE                 66
#define FX_MODE_TWINKLEFOX              67
#define FX_MODE_VUMETER                 68
#define FX_MODE_POPCORN                 69
#define FX_MODE_BLOCKDISSOLVE           70

// Used for HeartBeat mode
#define BEATS_PER_MINUTE                50
#define MS_PER_BEAT                     (60000 / BEATS_PER_MINUTE)
#define SECOND_BEAT                     (MS_PER_BEAT / 3)

// Used for TriFade mode
#define TRIFADE_BLACK                   (u8)B10000000 // same as REVERSE macro

// Used for Popcorn mode
#define MAX_NUM_POPCORN                 10
#define GRAVITY                         0.1


typedef u16 (*mode_ptr)(void);

// segment parameters
typedef struct Segment{ // 20 bytes
    u16 start;
    u16 stop;
    u16 speed;
    u8  mode;
    u8  options;
    u32 colors[MAX_NUM_COLORS];
}Segment;

// segment runtime parameters
typedef struct Segment_runtime{ // 16 bytes
    unsigned long next_time;
    u32 counter_mode_step;
    u32 counter_mode_call;
    u8 aux_param;   // auxilary param (usually stores a color_wheel index)
    u8 aux_param2;  // auxilary param (usually stores bitwise options)
    u16 aux_param3; // auxilary param (usually stores a segment index)
}Segment_runtime;


struct Matrix {
    int8_t   numPages;
    int8_t   numRows;
    int8_t   numCols;
    int32_t* colors;
};

typedef struct Oscillator {
    int16_t pos;
    int8_t  size;
    int8_t  dir;
    int8_t  speed;
}oscillator;

typedef struct Kernel {
  float position;
  float velocity;
  int32_t color;
}kernel;


//   void timer(void),
void WS2812FX_init(u16 num_leds, u8 type);
void WS2812FX_service(void);
void WS2812FX_start(void);
void WS2812FX_stop(void);
void WS2812FX_pause(void);
void WS2812FX_resume(void);
void WS2812FX_strip_off(void);
void WS2812FX_fade_out(void);
void WS2812FX_fade_out_targetColor(u32);
void WS2812FX_setMode(u8 m);
void WS2812FX_setMode_seg(u8 seg, u8 m);
void WS2812FX_setOptions(u8 seg, u8 o);
void WS2812FX_setCustomMode(u16 (*p)());
void WS2812FX_setCustomShow(void (*p)());
void WS2812FX_setSpeed(u16 s);
void WS2812FX_setSpeed_seg(u8 seg, u16 s);
void WS2812FX_increaseSpeed(u8 s);
void WS2812FX_decreaseSpeed(u8 s);
void WS2812FX_setColor_rgb(u8 r, u8 g, u8 b);
void WS2812FX_setColor_rgbw(u8 r, u8 g, u8 b, u8 w);
void WS2812FX_setColor(u32 c);
void WS2812FX_setColor_seg(u8 seg, u32 c);
void WS2812FX_setColors(u8 seg, u32* c);
void WS2812FX_setBrightness(u8 b);
void WS2812FX_increaseBrightness(u8 s);
void WS2812FX_decreaseBrightness(u8 s);
void WS2812FX_setLength(u16 b);
void WS2812FX_increaseLength(u16 s);
void WS2812FX_decreaseLength(u16 s);
void WS2812FX_trigger(void);
void WS2812FX_setCycle(void);
void WS2812FX_setNumSegments(u8 n);
void WS2812FX_setSegment_colorReverse(u8 n, u16 start, u16 stop, u8 mode, u32 color,          u16 speed, bool reverse);
void WS2812FX_setSegment_colorOptions(u8 n, u16 start, u16 stop, u8 mode, u32 color,          u16 speed, u8 options);
void WS2812FX_setSegment_colorsReverse(u8 n, u16 start, u16 stop, u8 mode, u32 colors[], u16 speed, bool reverse);
void WS2812FX_setSegment_colorsOptions(u8 n, u16 start, u16 stop, u8 mode, u32 colors[], u16 speed, u8 options);
void WS2812FX_setIdleSegment(u8 n, u16 start, u16 stop, u8 mode, u32 color,          u16 speed, u8 options);
void WS2812FX_setIdleSegment_colors(u8 n, u16 start, u16 stop, u8 mode, u32 colors[], u16 speed, u8 options);
void WS2812FX_addActiveSegment(u8 seg);
void WS2812FX_removeActiveSegment(u8 seg);
void WS2812FX_swapActiveSegment(u8 oldSeg, u8 newSeg);
void WS2812FX_resetSegments(void);
void WS2812FX_resetSegmentRuntimes(void);
void WS2812FX_resetSegmentRuntime(u8);
void WS2812FX_setPixelColor(u16 n, u32 c);
void WS2812FX_setPixelColor_rgb(u16 n, u8 r, u8 g, u8 b);
void WS2812FX_setPixelColor_rgbw(u16 n, u8 r, u8 g, u8 b, u8 w);
void WS2812FX_copyPixels(u16 d, u16 s, u16 c);
void WS2812FX_setPixels(u16, u8*);
u8* WS2812FX_getPixels(void);

void WS2812FX_show(void);


boolean WS2812FX_isRunning(void);
boolean WS2812FX_isTriggered(void);
boolean WS2812FX_isFrame(void);
boolean WS2812FX_isFrame_seg(u8);
boolean WS2812FX_isCycle(void);
boolean WS2812FX_isCycle_seg(u8);
boolean WS2812FX_isActiveSegment(u8 seg);


u8 WS2812FX_random8(void);
u8 WS2812FX_random8_lim(u8);
u8 WS2812FX_getMode(void);
u8 WS2812FX_getMode_seg(u8);
u8 WS2812FX_getModeCount(void);
//u8 WS2812FX_setCustomMode(const __FlashStringHelper* name, u16 (*p)());
//u8 WS2812FX_setCustomMode(u8 i, const __FlashStringHelper* name, u16 (*p)());
u8 WS2812FX_getNumSegments(void);
u8 WS2812FX_get_random_wheel_index(u8);
u8 WS2812FX_getOptions(u8);
u8 WS2812FX_getNumBytesPerPixel(void);


u16 WS2812FX_random16(void);
u16 WS2812FX_random16_lim(u16);
u16 WS2812FX_getSpeed(void);
u16 WS2812FX_getSpeed_seg(u8);
u16 WS2812FX_getLength(void);
u16 WS2812FX_getNumBytes(void);


u32 WS2812FX_color_wheel(u8);
u32 WS2812FX_getColor(void);
u32 WS2812FX_getColor_seg(u8);
u32 WS2812FX_intensitySum(void);


u32* getColors(u8);
u32* intensitySums(void);
u8*  getActiveSegments(void);
const __FlashStringHelper* getModeName(u8 m);

Segment* getSegment(void);
Segment* getSegment_seg(u8);
Segment* getSegments(void);

Segment_runtime* getSegmentRuntime(void);
Segment_runtime* getSegmentRuntime_seg(u8);
Segment_runtime* getSegmentRuntimes(void);


// mode helper functions
u16 WS2812FX_blink(u32, u32, bool strobe);
u16 WS2812FX_color_wipe(u32, u32, bool);
u16 WS2812FX_twinkle(u32, u32);
u16 WS2812FX_twinkle_fade(u32);
u16 WS2812FX_sparkle(u32, u32);
u16 WS2812FX_chase(u32, u32, u32);
u16 WS2812FX_chase_flash(u32, u32);
u16 WS2812FX_running(u32, u32);
u16 WS2812FX_fireworks(u32);
u16 WS2812FX_fire_flicker(int);
u16 WS2812FX_tricolor_chase(u32, u32, u32);
u16 WS2812FX_scan(u32, u32, bool);


u32 WS2812FX_color_blend(u32, u32, u8);


// builtin modes
u16 WS2812FX_mode_static(void);
u16 WS2812FX_mode_blink(void);
u16 WS2812FX_mode_blink_rainbow(void);
u16 WS2812FX_mode_strobe(void);
u16 WS2812FX_mode_strobe_rainbow(void);
u16 WS2812FX_mode_color_wipe(void);
u16 WS2812FX_mode_color_wipe_inv(void);
u16 WS2812FX_mode_color_wipe_rev(void);
u16 WS2812FX_mode_color_wipe_rev_inv(void);
u16 WS2812FX_mode_color_wipe_random(void);
u16 WS2812FX_mode_color_sweep_random(void);
u16 WS2812FX_mode_random_color(void);
u16 WS2812FX_mode_single_dynamic(void);
u16 WS2812FX_mode_multi_dynamic(void);
u16 WS2812FX_mode_breath(void);
u16 WS2812FX_mode_fade(void);
u16 WS2812FX_mode_scan(void);
u16 WS2812FX_mode_dual_scan(void);
u16 WS2812FX_mode_theater_chase(void);
u16 WS2812FX_mode_theater_chase_rainbow(void);
u16 WS2812FX_mode_rainbow(void);
u16 WS2812FX_mode_rainbow_cycle(void);
u16 WS2812FX_mode_running_lights(void);
u16 WS2812FX_mode_twinkle(void);
u16 WS2812FX_mode_twinkle_random(void);
u16 WS2812FX_mode_twinkle_fade(void);
u16 WS2812FX_mode_twinkle_fade_random(void);
u16 WS2812FX_mode_sparkle(void);
u16 WS2812FX_mode_flash_sparkle(void);
u16 WS2812FX_mode_hyper_sparkle(void);
u16 WS2812FX_mode_multi_strobe(void);
u16 WS2812FX_mode_chase_white(void);
u16 WS2812FX_mode_chase_color(void);
u16 WS2812FX_mode_chase_random(void);
u16 WS2812FX_mode_chase_rainbow(void);
u16 WS2812FX_mode_chase_flash(void);
u16 WS2812FX_mode_chase_flash_random(void);
u16 WS2812FX_mode_chase_rainbow_white(void);
u16 WS2812FX_mode_chase_blackout(void);
u16 WS2812FX_mode_chase_blackout_rainbow(void);
u16 WS2812FX_mode_running_color(void);
u16 WS2812FX_mode_running_red_blue(void);
u16 WS2812FX_mode_running_random(void);
u16 WS2812FX_mode_larson_scanner(void);
u16 WS2812FX_mode_comet(void);
u16 WS2812FX_mode_fireworks(void);
u16 WS2812FX_mode_fireworks_random(void);
u16 WS2812FX_mode_merry_christmas(void);
u16 WS2812FX_mode_halloween(void);
u16 WS2812FX_mode_fire_flicker(void);
u16 WS2812FX_mode_fire_flicker_soft(void);
u16 WS2812FX_mode_fire_flicker_intense(void);
u16 WS2812FX_mode_circus_combustus(void);
u16 WS2812FX_mode_bicolor_chase(void);
u16 WS2812FX_mode_tricolor_chase(void);

void configMatrix(u8 numPages, u8 numRows, u8 numCols, u32* colors);
u16 WS2812FX_mode_matrix(void);

u16 WS2812FX_mode_rainbowLarson(void);
u16 WS2812FX_mode_dualLarson(void);
u16 WS2812FX_mode_fillerup(void);
u16 WS2812FX_mode_icu(void);
u16 WS2812FX_mode_multiComet(void);

void beatIt(Segment* seg, u8 size);
u16 WS2812FX_mode_heartbeat(void);

u8 custom_rand(u8 min, u8 max);
u16 WS2812FX_mode_oscillate(void);
u16 WS2812FX_mode_rain(void);
u16 WS2812FX_mode_rainbowFireworks(void);
u16 WS2812FX_mode_randomChase(void);
u16 WS2812FX_mode_triFade(void);
u16 WS2812FX_mode_twinkleFox(void);
u16 WS2812FX_mode_vuMeter(void);
u16 WS2812FX_mode_popcorn(void);
u16 WS2812FX_mode_blockDissolve(void);



#endif /* __BSP_WS2812FX_H */

