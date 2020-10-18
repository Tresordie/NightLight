#include "ws2812fx.h"
#include "stm32f10x_it.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// create GLOBAL names to allow WS2812FX to compile with sketches and other libs
// that store strings in PROGMEM (get rid of the "section type conflict with __c"
// errors once and for all. Amen.)
//const char name_0[] PROGMEM = "Static";
//const char name_1[] PROGMEM = "Blink";
//const char name_2[] PROGMEM = "Breath";
//const char name_3[] PROGMEM = "Color Wipe";
//const char name_4[] PROGMEM = "Color Wipe Inverse";
//const char name_5[] PROGMEM = "Color Wipe Reverse";
//const char name_6[] PROGMEM = "Color Wipe Reverse Inverse";
//const char name_7[] PROGMEM = "Color Wipe Random";
//const char name_8[] PROGMEM = "Random Color";
//const char name_9[] PROGMEM = "Single Dynamic";
//const char name_10[] PROGMEM = "Multi Dynamic";
//const char name_11[] PROGMEM = "Rainbow";
//const char name_12[] PROGMEM = "Rainbow Cycle";
//const char name_13[] PROGMEM = "Scan";
//const char name_14[] PROGMEM = "Dual Scan";
//const char name_15[] PROGMEM = "Fade";
//const char name_16[] PROGMEM = "Theater Chase";
//const char name_17[] PROGMEM = "Theater Chase Rainbow";
//const char name_18[] PROGMEM = "Running Lights";
//const char name_19[] PROGMEM = "Twinkle";
//const char name_20[] PROGMEM = "Twinkle Random";
//const char name_21[] PROGMEM = "Twinkle Fade";
//const char name_22[] PROGMEM = "Twinkle Fade Random";
//const char name_23[] PROGMEM = "Sparkle";
//const char name_24[] PROGMEM = "Flash Sparkle";
//const char name_25[] PROGMEM = "Hyper Sparkle";
//const char name_26[] PROGMEM = "Strobe";
//const char name_27[] PROGMEM = "Strobe Rainbow";
//const char name_28[] PROGMEM = "Multi Strobe";
//const char name_29[] PROGMEM = "Blink Rainbow";
//const char name_30[] PROGMEM = "Chase White";
//const char name_31[] PROGMEM = "Chase Color";
//const char name_32[] PROGMEM = "Chase Random";
//const char name_33[] PROGMEM = "Chase Rainbow";
//const char name_34[] PROGMEM = "Chase Flash";
//const char name_35[] PROGMEM = "Chase Flash Random";
//const char name_36[] PROGMEM = "Chase Rainbow White";
//const char name_37[] PROGMEM = "Chase Blackout";
//const char name_38[] PROGMEM = "Chase Blackout Rainbow";
//const char name_39[] PROGMEM = "Color Sweep Random";
//const char name_40[] PROGMEM = "Running Color";
//const char name_41[] PROGMEM = "Running Red Blue";
//const char name_42[] PROGMEM = "Running Random";
//const char name_43[] PROGMEM = "Larson Scanner";
//const char name_44[] PROGMEM = "Comet";
//const char name_45[] PROGMEM = "Fireworks";
//const char name_46[] PROGMEM = "Fireworks Random";
//const char name_47[] PROGMEM = "Merry Christmas";
//const char name_48[] PROGMEM = "Fire Flicker";
//const char name_49[] PROGMEM = "Fire Flicker (soft)";
//const char name_50[] PROGMEM = "Fire Flicker (intense)";
//const char name_51[] PROGMEM = "Circus Combustus";
//const char name_52[] PROGMEM = "Halloween";
//const char name_53[] PROGMEM = "Bicolor Chase";
//const char name_54[] PROGMEM = "Tricolor Chase";
//const char name_55[] PROGMEM = "Custom 0"; // custom modes need to go at the end
//const char name_56[] PROGMEM = "Custom 1";
//const char name_57[] PROGMEM = "Custom 2";
//const char name_58[] PROGMEM = "Custom 3";
//const char name_59[] PROGMEM = "Custom 4";
//const char name_60[] PROGMEM = "Custom 5";
//const char name_61[] PROGMEM = "Custom 6";
//const char name_62[] PROGMEM = "Custom 7";

//static const __FlashStringHelper* _names[] = {
//  FSH(name_0),
//  FSH(name_1),
//  FSH(name_2),
//  FSH(name_3),
//  FSH(name_4),
//  FSH(name_5),
//  FSH(name_6),
//  FSH(name_7),
//  FSH(name_8),
//  FSH(name_9),
//  FSH(name_10),
//  FSH(name_11),
//  FSH(name_12),
//  FSH(name_13),
//  FSH(name_14),
//  FSH(name_15),
//  FSH(name_16),
//  FSH(name_17),
//  FSH(name_18),
//  FSH(name_19),
//  FSH(name_20),
//  FSH(name_21),
//  FSH(name_22),
//  FSH(name_23),
//  FSH(name_24),
//  FSH(name_25),
//  FSH(name_26),
//  FSH(name_27),
//  FSH(name_28),
//  FSH(name_29),
//  FSH(name_30),
//  FSH(name_31),
//  FSH(name_32),
//  FSH(name_33),
//  FSH(name_34),
//  FSH(name_35),
//  FSH(name_36),
//  FSH(name_37),
//  FSH(name_38),
//  FSH(name_39),
//  FSH(name_40),
//  FSH(name_41),
//  FSH(name_42),
//  FSH(name_43),
//  FSH(name_44),
//  FSH(name_45),
//  FSH(name_46),
//  FSH(name_47),
//  FSH(name_48),
//  FSH(name_49),
//  FSH(name_50),
//  FSH(name_51),
//  FSH(name_52),
//  FSH(name_53),
//  FSH(name_54),
//  FSH(name_55),
//  FSH(name_56),
//  FSH(name_57),
//  FSH(name_58),
//  FSH(name_59),
//  FSH(name_60),
//  FSH(name_61),
//  FSH(name_62)
//};


/*----variable from Adafruit library(in order to separate FXLib from Adafruit)----*/




/*-------------------------------variable-------------------------------*/
static u16 _rand16seed;
static void (*customShow)(void) = NULL;
static bool _running, _triggered;
static Segment* _segments;                 // array of segments (20 bytes per element)
static Segment_runtime* _segment_runtimes; // array of segment runtimes (16 bytes per element)
static u8* _active_segments;          // array of active segments (1 bytes per element)
static u8 _segments_len = 0;          // size of _segments array
static u8 _active_segments_len = 0;   // size of _segments_runtime and _active_segments arrays
static u8 _num_segments = 0;          // number of configured segments in the _segments array
static Segment* _seg;                      // currently active segment (20 bytes)
static Segment_runtime* _seg_rt;           // currently active segment runtime (16 bytes)
static u16 _seg_len;                  // num LEDs in the currently active segment

// define static array of member function pointers.
// function pointers MUST be in the same order as the corresponding name in the _name array.
static mode_ptr _modes[MODE_COUNT] = {
    &WS2812FX_mode_static,
    &WS2812FX_mode_blink,
    &WS2812FX_mode_breath,
    &WS2812FX_mode_color_wipe,
    &WS2812FX_mode_color_wipe_inv,
    &WS2812FX_mode_color_wipe_rev,
    &WS2812FX_mode_color_wipe_rev_inv,
    &WS2812FX_mode_color_wipe_random,
    &WS2812FX_mode_random_color,
    &WS2812FX_mode_single_dynamic,
    &WS2812FX_mode_multi_dynamic,
    &WS2812FX_mode_rainbow,
    &WS2812FX_mode_rainbow_cycle,
    &WS2812FX_mode_scan,
    &WS2812FX_mode_dual_scan,
    &WS2812FX_mode_fade,
    &WS2812FX_mode_theater_chase,
    &WS2812FX_mode_theater_chase_rainbow,
    &WS2812FX_mode_running_lights,
    &WS2812FX_mode_twinkle,
    &WS2812FX_mode_twinkle_random,
    &WS2812FX_mode_twinkle_fade,
    &WS2812FX_mode_twinkle_fade_random,
    &WS2812FX_mode_sparkle,
    &WS2812FX_mode_flash_sparkle,
    &WS2812FX_mode_hyper_sparkle,
    &WS2812FX_mode_strobe,
    &WS2812FX_mode_strobe_rainbow,
    &WS2812FX_mode_multi_strobe,
    &WS2812FX_mode_blink_rainbow,
    &WS2812FX_mode_chase_white,
    &WS2812FX_mode_chase_color,
    &WS2812FX_mode_chase_random,
    &WS2812FX_mode_chase_rainbow,
    &WS2812FX_mode_chase_flash,
    &WS2812FX_mode_chase_flash_random,
    &WS2812FX_mode_chase_rainbow_white,
    &WS2812FX_mode_chase_blackout,
    &WS2812FX_mode_chase_blackout_rainbow,
    &WS2812FX_mode_color_sweep_random,
    &WS2812FX_mode_running_color,
    &WS2812FX_mode_running_red_blue,
    &WS2812FX_mode_running_random,
    &WS2812FX_mode_larson_scanner,
    &WS2812FX_mode_comet,
    &WS2812FX_mode_fireworks,
    &WS2812FX_mode_fireworks_random,
    &WS2812FX_mode_merry_christmas,
    &WS2812FX_mode_fire_flicker,
    &WS2812FX_mode_fire_flicker_soft,
    &WS2812FX_mode_fire_flicker_intense,
    &WS2812FX_mode_circus_combustus,
    &WS2812FX_mode_halloween,
    &WS2812FX_mode_bicolor_chase,
    &WS2812FX_mode_tricolor_chase,
    &WS2812FX_mode_matrix,
    &WS2812FX_mode_rainbowLarson,
    &WS2812FX_mode_dualLarson,
    &WS2812FX_mode_fillerup,
    &WS2812FX_mode_icu,
    &WS2812FX_mode_multiComet,
    &WS2812FX_mode_heartbeat,
    &WS2812FX_mode_oscillate,
    &WS2812FX_mode_rain,
    &WS2812FX_mode_rainbowFireworks,
    &WS2812FX_mode_randomChase,
    &WS2812FX_mode_triFade,
    &WS2812FX_mode_twinkleFox,
    &WS2812FX_mode_vuMeter,
    &WS2812FX_mode_popcorn,
    &WS2812FX_mode_blockDissolve,
};


void WS2812FX_init(u16 num_leds, u8 type) {
    Adafruit_NeoPixel_init(num_leds, type);     //该函数要放在起始位置，放在后面可能会导致内存申请失败
    _running = false;
    _segments_len = MAX_NUM_SEGMENTS;
    _active_segments_len = MAX_NUM_ACTIVE_SEGMENTS;

    // create all the segment arrays and init to zeros
    _segments = (Segment *)malloc(_segments_len * sizeof(Segment));
    _active_segments = (u8 *)malloc(_active_segments_len * sizeof(u8));
    _segment_runtimes = (Segment_runtime *)malloc(_active_segments_len * sizeof(u8));

    WS2812FX_resetSegments();
    WS2812FX_setSegment_colorOptions(0, 0, num_leds - 1, DEFAULT_MODE, DEFAULT_COLOR, DEFAULT_SPEED, NO_OPTIONS);

    WS2812FX_resetSegmentRuntimes();

    Adafruit_NeoPixel_setBrightness(DEFAULT_BRIGHTNESS + 1);
}

// void WS2812FX::timer() {
//     for (int j=0; j < 1000; j++) {
//       u16 delay = (this->*_modes[_seg->mode])();
//     }
// }

void WS2812FX_service(void) {
    if(_running || _triggered) {
        unsigned long now = millis();   // Be aware, millis() rolls over every 49 days
        bool doShow = false;
        for(u8 i=0; i < _active_segments_len; i++) {
            if(_active_segments[i] != INACTIVE_SEGMENT) {
                _seg     = &_segments[_active_segments[i]];
                _seg_len = (u16)(_seg->stop - _seg->start + 1);
                _seg_rt  = &_segment_runtimes[i];
                CLR_FRAME_CYCLE;
                if(now > _seg_rt->next_time || _triggered) {
                    SET_FRAME;
                    doShow = true;
                    u16 delay = (*_modes[_seg->mode])();
                    _seg_rt->next_time = now + max(delay, SPEED_MIN);
                    _seg_rt->counter_mode_call++;
                }
            }
        }
        if(doShow) {
        //      delay(1); // for ESP32 (see https://forums.adafruit.com/viewtopic.php?f=47&t=117327)
            WS2812FX_show();
        }
        _triggered = false;
    }
}

u8* WS2812FX_getPixels(void) {
    Adafruit_NeoPixel_getPixels();
}

// overload setPixelColor() functions so we can use gamma correction
// (see https://learn.adafruit.com/led-tricks-gamma-correction/the-issue)
void WS2812FX_setPixelColor(u16 n, u32 c) {
    u8 w = (c >> 24) & 0xFF;
    u8 r = (c >> 16) & 0xFF;
    u8 g = (c >>  8) & 0xFF;
    u8 b =  c        & 0xFF;
    WS2812FX_setPixelColor_rgbw(n, r, g, b, w);
}

void WS2812FX_setPixelColor_rgb(u16 n, u8 r, u8 g, u8 b) {
    WS2812FX_setPixelColor_rgbw(n, r, g, b, 0);
}

void WS2812FX_setPixelColor_rgbw(u16 n, u8 r, u8 g, u8 b, u8 w) {
    if(IS_GAMMA) {
        Adafruit_NeoPixel_setPixelColor_rgbw(n, Adafruit_NeoPixel_gamma8(r), Adafruit_NeoPixel_gamma8(g), Adafruit_NeoPixel_gamma8(b), Adafruit_NeoPixel_gamma8(w));
    } else {
        Adafruit_NeoPixel_setPixelColor_rgbw(n, r, g, b, w);
    }
}

void WS2812FX_copyPixels(u16 dest, u16 src, u16 count) {
    u8 *pixels = Adafruit_NeoPixel_getPixels();
    u8 bytesPerPixel = WS2812FX_getNumBytesPerPixel(); // 3=RGB, 4=RGBW

    memmove(pixels + (dest * bytesPerPixel), pixels + (src * bytesPerPixel), count * bytesPerPixel);
}

// change the underlying Adafruit_NeoPixel pixels pointer (use with care)
//void WS2812FX_setPixels(u16 num_leds, u8* ptr) {
//  free(Adafruit_NeoPixel::pixels); // free existing data (if any)
//  Adafruit_NeoPixel::pixels = ptr;
//  Adafruit_NeoPixel::numLEDs = num_leds;
//  Adafruit_NeoPixel::numBytes = num_leds * ((wOffset == rOffset) ? 3 : 4);
//}

// overload show() functions so we can use custom show()
void WS2812FX_show(void) {
    customShow == NULL ? Adafruit_NeoPixel_show() : customShow();
}

void WS2812FX_start() {
    WS2812FX_resetSegmentRuntimes();
    _running = true;
}

void WS2812FX_stop() {
    _running = false;
    WS2812FX_strip_off();
}

void WS2812FX_pause() {
    _running = false;
}

void WS2812FX_resume() {
    _running = true;
}

void WS2812FX_trigger() {
    _triggered = true;
}

void WS2812FX_setMode(u8 m) {
    WS2812FX_setMode_seg(0, m);
}

void WS2812FX_setMode_seg(u8 seg, u8 m) {
    WS2812FX_resetSegmentRuntime(seg);
    _segments[seg].mode = constrain(m, 0, MODE_COUNT - 1);
}

void WS2812FX_setOptions(u8 seg, u8 o) {
    _segments[seg].options = o;
}

void WS2812FX_setSpeed(u16 s) {
    WS2812FX_setSpeed_seg(0, s);
}

void WS2812FX_setSpeed_seg(u8 seg, u16 s) {
    _segments[seg].speed = constrain(s, SPEED_MIN, SPEED_MAX);
}

void WS2812FX_increaseSpeed(u8 s) {
    u16 newSpeed = constrain(_seg->speed + s, SPEED_MIN, SPEED_MAX);
    WS2812FX_setSpeed(newSpeed);
}

void WS2812FX_decreaseSpeed(u8 s) {
    u16 newSpeed = constrain(_seg->speed - s, SPEED_MIN, SPEED_MAX);
    WS2812FX_setSpeed(newSpeed);
}

void WS2812FX_setColor_rgb(u8 r, u8 g, u8 b) {
    WS2812FX_setColor(((u32)r << 16) | ((u32)g << 8) | b);
}

void WS2812FX_setColor_rgbw(u8 r, u8 g, u8 b, u8 w) {
    WS2812FX_setColor((((u32)w << 24)| ((u32)r << 16) | ((u32)g << 8)| ((u32)b)));
}

void WS2812FX_setColor(u32 c) {
    WS2812FX_setColor_seg(0, c);
}

void WS2812FX_setColor_seg(u8 seg, u32 c) {
    _segments[seg].colors[0] = c;
}

void WS2812FX_setColors(u8 seg, u32* c) {
    for(u8 i=0; i<MAX_NUM_COLORS; i++) {
        _segments[seg].colors[i] = c[i];
    }
}

void WS2812FX_setBrightness(u8 b) {
    b = constrain(b, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
    Adafruit_NeoPixel_setBrightness(b);
    WS2812FX_show();
}

void WS2812FX_increaseBrightness(u8 s) {
    s = constrain(Adafruit_NeoPixel_getBrightness() + s, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
    WS2812FX_setBrightness(s);
}

void WS2812FX_decreaseBrightness(u8 s) {
    s = constrain(Adafruit_NeoPixel_getBrightness() - s, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
    WS2812FX_setBrightness(s);
}

//void WS2812FX::setLength(u16 b) {
//  resetSegmentRuntimes();
//  if (b < 1) b = 1;

//  // Decrease numLEDs to maximum available memory
//  do {
//      Adafruit_NeoPixel::updateLength(b);
//      b--;
//  } while(!Adafruit_NeoPixel::numLEDs && b > 1);

//  _segments[0].start = 0;
//  _segments[0].stop = Adafruit_NeoPixel::numLEDs - 1;
//}

//void WS2812FX_increaseLength(u16 s) {
//  u16 seglen = _segments[0].stop - _segments[0].start + 1;
//  setLength(seglen + s);
//}

//void WS2812FX_decreaseLength(u16 s) {
//  u16 seglen = _segments[0].stop - _segments[0].start + 1;
//  fill(BLACK, _segments[0].start, seglen);
//  show();

//  if (s < seglen) setLength(seglen - s);
//}

boolean WS2812FX_isRunning() {
    return _running;
}

boolean WS2812FX_isTriggered() {
    return _triggered;
}

boolean WS2812FX_isFrame() {
    return WS2812FX_isFrame_seg(0);
}

boolean WS2812FX_isFrame_seg(u8 seg) {
    u8* ptr = (u8*)memchr(_active_segments, seg, _active_segments_len);
    if(ptr == NULL) return false; // segment not active
    return (_segment_runtimes[ptr - _active_segments].aux_param2 & FRAME);
}

boolean WS2812FX_isCycle() {
    return WS2812FX_isCycle_seg(0);
}

boolean WS2812FX_isCycle_seg(u8 seg) {
    u8* ptr = (u8*)memchr(_active_segments, seg, _active_segments_len);
    if(ptr == NULL) return false; // segment not active
    return (_segment_runtimes[ptr - _active_segments].aux_param2 & CYCLE);
}

void WS2812FX_setCycle() {
    SET_CYCLE;
}

u8 WS2812FX_getMode(void) {
    return WS2812FX_getMode_seg(0);
}

u8 WS2812FX_getMode_seg(u8 seg) {
    return _segments[seg].mode;
}

u16 WS2812FX_getSpeed(void) {
    return WS2812FX_getSpeed_seg(0);
}

u16 WS2812FX_getSpeed_seg(u8 seg) {
    return _segments[seg].speed;
}

u8 WS2812FX_getOptions(u8 seg) {
    return _segments[seg].options;
}

u16 WS2812FX_getLength(void) {
    return Adafruit_NeoPixel_numPixels();
}

u16 WS2812FX_getNumBytes(void) {
    return Adafruit_NeoPixel_getNumBytes();
}

u8 WS2812FX_getNumBytesPerPixel(void) {
    return Adafruit_NeoPixel_getNumBytesPerPixel();
}

u8 WS2812FX_getModeCount(void) {
    return MODE_COUNT;
}

u8 WS2812FX_getNumSegments(void) {
    return _num_segments;
}

void WS2812FX_setNumSegments(u8 n) {
    _num_segments = n;
}

u32 WS2812FX_getColor(void) {
    return WS2812FX_getColor_seg(0);
}

u32 WS2812FX_getColor_seg(u8 seg) {
    return _segments[seg].colors[0];
}

u32* WS2812FX_getColors(u8 seg) {
    return _segments[seg].colors;
}

Segment* WS2812FX_getSegment(void) {
    return _seg;
}

Segment* WS2812FX_getSegment_seg(u8 seg) {
    return &_segments[seg];
}

Segment* WS2812FX_getSegments(void) {
    return _segments;
}

Segment_runtime* WS2812FX_getSegmentRuntime(void) {
    return _seg_rt;
}

Segment_runtime* WS2812FX_getSegmentRuntime_seg(u8 seg) {
    u8* ptr = (u8*)memchr(_active_segments, seg, _active_segments_len);
    if(ptr == NULL) return NULL; // segment not active
    return &_segment_runtimes[ptr - _active_segments];
}

Segment_runtime* WS2812FX_getSegmentRuntimes(void) {
    return _segment_runtimes;
}

u8* WS2812FX_getActiveSegments(void) {
    return _active_segments;
}

//const __FlashStringHelper* WS2812FX_getModeName(u8 m) {
//  if(m < MODE_COUNT) {
//    return _names[m];
//  } else {
//    return "";//F("");
//  }
//}

void WS2812FX_setIdleSegment(u8 n, u16 start, u16 stop, u8 mode, u32 color, u16 speed, u8 options) {
    u32 colors[] = {color, 0, 0};
    WS2812FX_setIdleSegment_colors(n, start, stop, mode, colors, speed, options);
}

void WS2812FX_setIdleSegment_colors(u8 n, u16 start, u16 stop, u8 mode, u32 colors[], u16 speed, u8 options) {
    WS2812FX_setSegment_colorsOptions(n, start, stop, mode, colors, speed, options);
    if(n < _active_segments_len) WS2812FX_removeActiveSegment(n);;
}

void WS2812FX_setSegment_colorReverse(u8 n, u16 start, u16 stop, u8 mode, u32 color, u16 speed, bool reverse) {
    u32 colors[] = {color, 0, 0};
    WS2812FX_setSegment_colorsOptions(n, start, stop, mode, colors, speed, (u8)(reverse ? REVERSE : NO_OPTIONS));
}

void WS2812FX_setSegment_colorsReverse(u8 n, u16 start, u16 stop, u8 mode, u32 colors[], u16 speed, bool reverse) {
    WS2812FX_setSegment_colorsOptions(n, start, stop, mode, colors, speed, (u8)(reverse ? REVERSE : NO_OPTIONS));
}

void WS2812FX_setSegment_colorOptions(u8 n, u16 start, u16 stop, u8 mode, u32 color, u16 speed, u8 options) {
    u32 colors[] = {color, 0, 0};
    WS2812FX_setSegment_colorsOptions(n, start, stop, mode, colors, speed, options);
}

void WS2812FX_setSegment_colorsOptions(u8 n, u16 start, u16 stop, u8 mode, u32 colors[], u16 speed, u8 options) {
    if(n < _segments_len) {
        if(n + 1 > _num_segments) _num_segments = n + 1;
        _segments[n].start = start;
        _segments[n].stop = stop;
        _segments[n].mode = mode;
        _segments[n].speed = speed;
        _segments[n].options = options;

        WS2812FX_setColors(n, (u32*)colors);

        if(n < _active_segments_len) WS2812FX_addActiveSegment(n);
    }
}

void WS2812FX_addActiveSegment(u8 seg) {
    u8* ptr = (u8*)memchr(_active_segments, seg, _active_segments_len);
    if(ptr != NULL) return; // segment already active
    for(u8 i = 0; i < _active_segments_len; i++) {
        if(_active_segments[i] == INACTIVE_SEGMENT) {
            _active_segments[i] = seg;
            WS2812FX_resetSegmentRuntime(seg);
            break;
        }
    }
}

void WS2812FX_removeActiveSegment(u8 seg) {
    for(u8 i = 0; i < _active_segments_len; i++) {
        if(_active_segments[i] == seg) {
            _active_segments[i] = INACTIVE_SEGMENT;
        }
    }
}

void WS2812FX_swapActiveSegment(u8 oldSeg, u8 newSeg) {
    u8* ptr = (u8*)memchr(_active_segments, newSeg, _active_segments_len);
    if(ptr != NULL) return; // if newSeg is already active, don't swap
    for(u8 i = 0; i < _active_segments_len; i++) {
        if(_active_segments[i] == oldSeg) {
            _active_segments[i] = newSeg;

            // reset all runtime parameters EXCEPT next_time,
            // allowing the current animation frame to complete
            Segment_runtime seg_rt = _segment_runtimes[i];
            seg_rt.counter_mode_step = 0;
            seg_rt.counter_mode_call = 0;
            seg_rt.aux_param = 0;
            seg_rt.aux_param2 = 0;
            seg_rt.aux_param3 = 0;
            break;
        }
    }
}

boolean WS2812FX_isActiveSegment(u8 seg) {
    u8* ptr = (u8*)memchr(_active_segments, seg, _active_segments_len);
    if(ptr != NULL) return true;
    return false;
}

void WS2812FX_resetSegments() {
    WS2812FX_resetSegmentRuntimes();
    memset(_segments, 0, _segments_len * sizeof(Segment));
    memset(_active_segments, INACTIVE_SEGMENT, _active_segments_len);
    _num_segments = 0;
}

void WS2812FX_resetSegmentRuntimes() {
    memset(_segment_runtimes, 0, _active_segments_len * sizeof(Segment_runtime));
}

void WS2812FX_resetSegmentRuntime(u8 seg) {
    u8* ptr = (u8*)memchr(_active_segments, seg, _active_segments_len);
    if(ptr == NULL) return; // segment not active
    memset(&_segment_runtimes[ptr - _active_segments], 0, sizeof(Segment_runtime));
}

/*
 * Turns everything off. Doh.
 */
void WS2812FX_strip_off() {
    Adafruit_NeoPixel_clear();
    WS2812FX_show();
}

/*
 * Put a value 0 to 255 in to get a color value.
 * The colours are a transition r -> g -> b -> back to r
 * Inspired by the Adafruit examples.
 */
u32 WS2812FX_color_wheel(u8 pos) {
    pos = 255 - pos;
    if(pos < 85) {
        return ((u32)(255 - pos * 3) << 16) | ((u32)(0) << 8) | (pos * 3);
    }else if(pos < 170) {
        pos -= 85;
        return ((u32)(0) << 16) | ((u32)(pos * 3) << 8) | (255 - pos * 3);
    }else {
        pos -= 170;
        return ((u32)(pos * 3) << 16) | ((u32)(255 - pos * 3) << 8) | (0);
    }
}

/*
 * Returns a new, random wheel index with a minimum distance of 42 from pos.
 */
u8 WS2812FX_get_random_wheel_index(u8 pos) {
    u8 r = 0;
    u8 x = 0;
    u8 y = 0;
    u8 d = 0;

    while(d < 42) {
        r = WS2812FX_random8();
        x = abs(pos - r);
        y = 255 - x;
        d = min(x, y);
    }

    return r;
}

// fast 8-bit random number generator shamelessly borrowed from FastLED
u8 WS2812FX_random8() {
    _rand16seed = (_rand16seed * 2053) + 13849;
    return (u8)((_rand16seed + (_rand16seed >> 8)) & 0xFF);
}

// note random8(lim) generates numbers in the range 0 to (lim -1)
u8 WS2812FX_random8_lim(u8 lim) {
    u8 r = WS2812FX_random8();
    r = ((u16)r * lim) >> 8;
    return r;
}

u16 WS2812FX_random16() {
    return (u16)WS2812FX_random8() * 256 + WS2812FX_random8();
}

// note random16(lim) generates numbers in the range 0 to (lim - 1)
u16 WS2812FX_random16_lim(u16 lim) {
    u16 r = WS2812FX_random16();
    r = ((u32)r * lim) >> 16;
    return r;
}

// Return the sum of all LED intensities (can be used for
// rudimentary power calculations)
//u32 WS2812FX_intensitySum() {
//  u8 *pixels = getPixels();
//  u32 sum = 0;
//  for(u16 i=0; i <numBytes; i++) {
//    sum+= pixels[i];
//  }
//  return sum;
//}

// Return the sum of each color's intensity. Note, the order of
// intensities in the returned array depends on the type of WS2812
// LEDs you have. NEO_GRB LEDs will return an array with entries
// in a different order then NEO_RGB LEDs.
//u32* WS2812FX_intensitySums() {
//  static u32 intensities[] = { 0, 0, 0, 0 };
//  memset(intensities, 0, sizeof(intensities));

//  u8 *pixels = getPixels();
//  u8 bytesPerPixel = getNumBytesPerPixel(); // 3=RGB, 4=RGBW
//  for(u16 i=0; i <numBytes; i += bytesPerPixel) {
//    intensities[0] += pixels[i];
//    intensities[1] += pixels[i + 1];
//    intensities[2] += pixels[i + 2];
//    if(bytesPerPixel == 4) intensities[3] += pixels[i + 3]; // for RGBW LEDs
//  }
//  return intensities;
//}


/* #####################################################
#
#  Mode Functions
#
##################################################### */

/*
 * No blinking. Just plain old static light.
 */
u16 WS2812FX_mode_static(void) {
    Adafruit_NeoPixel_fill(_seg->colors[0], _seg->start, _seg_len);
    SET_CYCLE;
    return _seg->speed;
}


/*
 * Blink/strobe function
 * Alternate between color1 and color2
 * if(strobe == true) then create a strobe effect
 */
u16 WS2812FX_blink(u32 color1, u32 color2, bool strobe) {
    if(_seg_rt->counter_mode_call & 1) {
        u32 color = (IS_REVERSE) ? color1 : color2; // off
        Adafruit_NeoPixel_fill(color, _seg->start, _seg_len);
        SET_CYCLE;
        return strobe ? _seg->speed - 20 : (_seg->speed / 2);
    }else {
        u32 color = (IS_REVERSE) ? color2 : color1; // on
        Adafruit_NeoPixel_fill(color, _seg->start, _seg_len);
        return strobe ? 20 : (_seg->speed / 2);
    }
}


/*
 * Normal blinking. 50% on/off time.
 */
u16 WS2812FX_mode_blink(void) {
    return WS2812FX_blink(_seg->colors[0], _seg->colors[1], false);
}


/*
 * Classic Blink effect. Cycling through the rainbow.
 */
u16 WS2812FX_mode_blink_rainbow(void) {
    return WS2812FX_blink(WS2812FX_color_wheel(_seg_rt->counter_mode_call & 0xFF), _seg->colors[1], false);
}


/*
 * Classic Strobe effect.
 */
u16 WS2812FX_mode_strobe(void) {
    return WS2812FX_blink(_seg->colors[0], _seg->colors[1], true);
}


/*
 * Classic Strobe effect. Cycling through the rainbow.
 */
u16 WS2812FX_mode_strobe_rainbow(void) {
    return WS2812FX_blink(WS2812FX_color_wheel(_seg_rt->counter_mode_call & 0xFF), _seg->colors[1], true);
}


/*
 * Color wipe function
 * LEDs are turned on (color1) in sequence, then turned off (color2) in sequence.
 * if (bool rev == true) then LEDs are turned off in reverse order
 */
u16 WS2812FX_color_wipe(u32 color1, u32 color2, bool rev) {
    if(_seg_rt->counter_mode_step < _seg_len) {
        u32 led_offset = _seg_rt->counter_mode_step;
        if(IS_REVERSE) {
            WS2812FX_setPixelColor(_seg->stop - led_offset, color1);
        }else {
            WS2812FX_setPixelColor(_seg->start + led_offset, color1);
        }
    }else {
        u32 led_offset = _seg_rt->counter_mode_step - _seg_len;
        if((IS_REVERSE && !rev) || (!IS_REVERSE && rev)) {
            WS2812FX_setPixelColor(_seg->stop - led_offset, color2);
        }else {
            WS2812FX_setPixelColor(_seg->start + led_offset, color2);
        }
    }

    _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % (_seg_len * 2);

    if(_seg_rt->counter_mode_step == 0) SET_CYCLE;

    return (_seg->speed / (_seg_len * 2));
}

/*
 * Lights all LEDs one after another.
 */
u16 WS2812FX_mode_color_wipe(void) {
    return WS2812FX_color_wipe(_seg->colors[0], _seg->colors[1], false);
}

u16 WS2812FX_mode_color_wipe_inv(void) {
    return WS2812FX_color_wipe(_seg->colors[1], _seg->colors[0], false);
}

u16 WS2812FX_mode_color_wipe_rev(void) {
    return WS2812FX_color_wipe(_seg->colors[0], _seg->colors[1], true);
}

u16 WS2812FX_mode_color_wipe_rev_inv(void) {
    return WS2812FX_color_wipe(_seg->colors[1], _seg->colors[0], true);
}


/*
 * Turns all LEDs after each other to a random color.
 * Then starts over with another color.
 */
u16 WS2812FX_mode_color_wipe_random(void) {
    if(_seg_rt->counter_mode_step % _seg_len == 0) { // aux_param will store our random color wheel index
        _seg_rt->aux_param = WS2812FX_get_random_wheel_index(_seg_rt->aux_param);
    }
    u32 color = WS2812FX_color_wheel(_seg_rt->aux_param);
    return WS2812FX_color_wipe(color, color, false) * 2;
}


/*
 * Random color introduced alternating from start and end of strip.
 */
u16 WS2812FX_mode_color_sweep_random(void) {
  if(_seg_rt->counter_mode_step % _seg_len == 0) { // aux_param will store our random color wheel index
    _seg_rt->aux_param = WS2812FX_get_random_wheel_index(_seg_rt->aux_param);
  }
  u32 color = WS2812FX_color_wheel(_seg_rt->aux_param);
  return WS2812FX_color_wipe(color, color, true) * 2;
}


/*
 * Lights all LEDs in one random color up. Then switches them
 * to the next random color.
 */
u16 WS2812FX_mode_random_color(void) {
    _seg_rt->aux_param = WS2812FX_get_random_wheel_index(_seg_rt->aux_param); // aux_param will store our random color wheel index
    u32 color = WS2812FX_color_wheel(_seg_rt->aux_param);
    Adafruit_NeoPixel_fill(color, _seg->start, _seg_len);
    SET_CYCLE;
    return _seg->speed;
}


/*
 * Lights every LED in a random color. Changes one random LED after the other
 * to another random color.
 */
u16 WS2812FX_mode_single_dynamic(void) {
    if(_seg_rt->counter_mode_call == 0) {
        for(u16 i=_seg->start; i <= _seg->stop; i++) {
            WS2812FX_setPixelColor(i, WS2812FX_color_wheel(WS2812FX_random8()));
        }
    }

    WS2812FX_setPixelColor(_seg->start + WS2812FX_random16_lim(_seg_len), WS2812FX_color_wheel(WS2812FX_random8()));
    SET_CYCLE;
    return _seg->speed;
}


/*
 * Lights every LED in a random color. Changes all LED at the same time
 * to new random colors.
 */
u16 WS2812FX_mode_multi_dynamic(void) {
    for(u16 i=_seg->start; i <= _seg->stop; i++) {
        WS2812FX_setPixelColor(i, WS2812FX_color_wheel(WS2812FX_random8()));
    }
    SET_CYCLE;
    return _seg->speed;
}


/*
 * Does the "standby-breathing" of well known i-Devices. Fixed Speed.
 * Use mode "fade" if you like to have something similar with a different speed.
 */
u16 WS2812FX_mode_breath(void) {
    int lum = _seg_rt->counter_mode_step;
    if(lum > 255) lum = 511 - lum; // lum = 15 -> 255 -> 15

    u16 delay;
    if(lum == 15) delay = 970; // 970 pause before each breath
    else if(lum <=  25) delay = 38; // 19
    else if(lum <=  50) delay = 36; // 18
    else if(lum <=  75) delay = 28; // 14
    else if(lum <= 100) delay = 20; // 10
    else if(lum <= 125) delay = 14; // 7
    else if(lum <= 150) delay = 11; // 5
    else delay = 10; // 4

    u32 color =  WS2812FX_color_blend(_seg->colors[1], _seg->colors[0], lum);
    Adafruit_NeoPixel_fill(color, _seg->start, _seg_len);

    _seg_rt->counter_mode_step += 2;
    if(_seg_rt->counter_mode_step > (512-15)) {
        _seg_rt->counter_mode_step = 15;
        SET_CYCLE;
    }
    return delay;
}


/*
 * Fades the LEDs between two colors
 */
u16 WS2812FX_mode_fade(void) {
    int lum = _seg_rt->counter_mode_step;
    if(lum > 255) lum = 511 - lum; // lum = 0 -> 255 -> 0

    u32 color = WS2812FX_color_blend(_seg->colors[1], _seg->colors[0], lum);
    Adafruit_NeoPixel_fill(color, _seg->start, _seg_len);

    _seg_rt->counter_mode_step += 4;
    if(_seg_rt->counter_mode_step > 511) {
        _seg_rt->counter_mode_step = 0;
        SET_CYCLE;
    }
    return (_seg->speed / 128);
}


/*
 * scan function - runs a block of pixels back and forth.
 */
u16 WS2812FX_scan(u32 color1, u32 color2, bool dual) {
    int8_t dir = _seg_rt->aux_param ? -1 : 1;
    u8 size = 1 << SIZE_OPTION;

    Adafruit_NeoPixel_fill(color2, _seg->start, _seg_len);

    for(u8 i = 0; i < size; i++) {
        if(IS_REVERSE || dual) {
            WS2812FX_setPixelColor(_seg->stop - _seg_rt->counter_mode_step - i, color1);
        }
        if(!IS_REVERSE || dual) {
            WS2812FX_setPixelColor(_seg->start + _seg_rt->counter_mode_step + i, color1);
        }
    }

    _seg_rt->counter_mode_step += dir;
    if(_seg_rt->counter_mode_step == 0) {
        _seg_rt->aux_param = 0;
        SET_CYCLE;
    }
    if(_seg_rt->counter_mode_step >= (u16)(_seg_len - size)) _seg_rt->aux_param = 1;

    return (_seg->speed / (_seg_len * 2));
}


/*
 * Runs a block of pixels back and forth.
 */
u16 WS2812FX_mode_scan(void) {
    return WS2812FX_scan(_seg->colors[0], _seg->colors[1], false);
}


/*
 * Runs two blocks of pixels back and forth in opposite directions.
 */
u16 WS2812FX_mode_dual_scan(void) {
    return WS2812FX_scan(_seg->colors[0], _seg->colors[1], true);
}


/*
 * Cycles all LEDs at once through a rainbow.
 */
u16 WS2812FX_mode_rainbow(void) {
    u32 color = WS2812FX_color_wheel(_seg_rt->counter_mode_step);
    Adafruit_NeoPixel_fill(color, _seg->start, _seg_len);

    _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) & 0xFF;

    if(_seg_rt->counter_mode_step == 0)  SET_CYCLE;

    return (_seg->speed / 256);
}


/*
 * Cycles a rainbow over the entire string of LEDs.
 */
u16 WS2812FX_mode_rainbow_cycle(void) {
    for(u16 i=0; i < _seg_len; i++) {
        u32 color = WS2812FX_color_wheel(((i * 256 / _seg_len) + _seg_rt->counter_mode_step) & 0xFF);
        WS2812FX_setPixelColor(_seg->start + i, color);
    }

    _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) & 0xFF;

    if(_seg_rt->counter_mode_step == 0) SET_CYCLE;

    return (_seg->speed / 256);
}


/*
 * Tricolor chase function
 */
u16 WS2812FX_tricolor_chase(u32 color1, u32 color2, u32 color3) {
    u8 sizeCnt = 1 << SIZE_OPTION;
    u8 sizeCnt2 = sizeCnt + sizeCnt;
    u8 sizeCnt3 = sizeCnt2 + sizeCnt;
    u16 index = _seg_rt->counter_mode_step % sizeCnt3;
    for(u16 i=0; i < _seg_len; i++, index++) {
        index = index % sizeCnt3;

        u32 color = color3;
        if(index < sizeCnt) color = color1;
        else if(index < sizeCnt2) color = color2;

        if(IS_REVERSE) {
            WS2812FX_setPixelColor(_seg->start + i, color);
        } else {
            WS2812FX_setPixelColor(_seg->stop - i, color);
        }
    }

    _seg_rt->counter_mode_step++;
    if(_seg_rt->counter_mode_step % _seg_len == 0) SET_CYCLE;

    return (_seg->speed / _seg_len);
}


/*
 * Tricolor chase mode
 */
u16 WS2812FX_mode_tricolor_chase(void) {
    return WS2812FX_tricolor_chase(_seg->colors[0], _seg->colors[1], _seg->colors[2]);
}


/*
 * Alternating white/red/black pixels running.
 */
u16 WS2812FX_mode_circus_combustus(void) {
    return WS2812FX_tricolor_chase(RED, WHITE, BLACK);
}


/*
 * Theatre-style crawling lights.
 * Inspired by the Adafruit examples.
 */
u16 WS2812FX_mode_theater_chase(void) {
    return WS2812FX_tricolor_chase(_seg->colors[0], _seg->colors[1], _seg->colors[1]);
}


/*
 * Theatre-style crawling lights with rainbow effect.
 * Inspired by the Adafruit examples.
 */
u16 WS2812FX_mode_theater_chase_rainbow(void) {
    _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) & 0xFF;
    u32 color = WS2812FX_color_wheel(_seg_rt->counter_mode_step);
    return WS2812FX_tricolor_chase(color, _seg->colors[1], _seg->colors[1]);
}


/*
 * Running lights effect with smooth sine transition.
 */
u16 WS2812FX_mode_running_lights(void) {
    u8 size = 1 << SIZE_OPTION;
    u8 sineIncr = max(1, (256 / _seg_len) * size);
    for(u16 i=0; i < _seg_len; i++) {
        int lum = (int)Adafruit_NeoPixel_sine8(((i + _seg_rt->counter_mode_step) * sineIncr));
        u32 color = WS2812FX_color_blend(_seg->colors[0], _seg->colors[1], lum);
        if(IS_REVERSE) {
            WS2812FX_setPixelColor(_seg->start + i, color);
        } else {
            WS2812FX_setPixelColor(_seg->stop - i,  color);
        }
    }
    _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % 256;
    if(_seg_rt->counter_mode_step == 0) SET_CYCLE;
    return (_seg->speed / _seg_len);
}


/*
 * twinkle function
 */
u16 WS2812FX_twinkle(u32 color1, u32 color2) {
    if(_seg_rt->counter_mode_step == 0) {
        Adafruit_NeoPixel_fill(color2, _seg->start, _seg_len);
        u16 min_leds = (_seg_len / 4) + 1; // make sure, at least one LED is on
        _seg_rt->counter_mode_step = WS2812FX_random16_lim(min_leds * 2);//random(min_leds, min_leds * 2);
        SET_CYCLE;
    }

    WS2812FX_setPixelColor(_seg->start + WS2812FX_random16_lim(_seg_len), color1);

    _seg_rt->counter_mode_step--;
    return (_seg->speed / _seg_len);
}

/*
 * Blink several LEDs on, reset, repeat.
 * Inspired by www.tweaking4all.com/hardware/arduino/arduino-led-strip-effects/
 */
u16 WS2812FX_mode_twinkle(void) {
    return WS2812FX_twinkle(_seg->colors[0], _seg->colors[1]);
}

/*
 * Blink several LEDs in random colors on, reset, repeat.
 * Inspired by www.tweaking4all.com/hardware/arduino/arduino-led-strip-effects/
 */
u16 WS2812FX_mode_twinkle_random(void) {
    return WS2812FX_twinkle(WS2812FX_color_wheel(WS2812FX_random8()), _seg->colors[1]);
}


/*
 * fade out functions
 */
void WS2812FX_fade_out() {
    /*return*/ WS2812FX_fade_out_targetColor(_seg->colors[1]);
}

void WS2812FX_fade_out_targetColor(u32 targetColor) {
    static const u8 rateMapH[] = {0, 1, 1, 1, 2, 3, 4, 6};
    static const u8 rateMapL[] = {0, 2, 3, 8, 8, 8, 8, 8};

    u8 rate  = FADE_RATE;
    u8 rateH = rateMapH[rate];
    u8 rateL = rateMapL[rate];

    u32 color = targetColor;
    int w2 = (color >> 24) & 0xff;
    int r2 = (color >> 16) & 0xff;
    int g2 = (color >>  8) & 0xff;
    int b2 =  color        & 0xff;

    for(u16 i=_seg->start; i <= _seg->stop; i++) {
        color = Adafruit_NeoPixel_getPixelColor(i); // current color
        if(rate == 0) { // old fade-to-black algorithm
            WS2812FX_setPixelColor(i, (color >> 1) & 0x7F7F7F7F);
        }else { // new fade-to-color algorithm
            int w1 = (color >> 24) & 0xff;
            int r1 = (color >> 16) & 0xff;
            int g1 = (color >>  8) & 0xff;
            int b1 =  color        & 0xff;

            // calculate the color differences between the current and target colors
            int wdelta = w2 - w1;
            int rdelta = r2 - r1;
            int gdelta = g2 - g1;
            int bdelta = b2 - b1;

            // if the current and target colors are almost the same, jump right to the target
            // color, otherwise calculate an intermediate color. (fixes rounding issues)
            wdelta = abs(wdelta) < 3 ? wdelta : (wdelta >> rateH) + (wdelta >> rateL);
            rdelta = abs(rdelta) < 3 ? rdelta : (rdelta >> rateH) + (rdelta >> rateL);
            gdelta = abs(gdelta) < 3 ? gdelta : (gdelta >> rateH) + (gdelta >> rateL);
            bdelta = abs(bdelta) < 3 ? bdelta : (bdelta >> rateH) + (bdelta >> rateL);

            WS2812FX_setPixelColor_rgbw(i, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
        }
    }
}


/*
 * color blend function
 */
u32 WS2812FX_color_blend(u32 color1, u32 color2, u8 blend) {
    if(blend == 0)   return color1;
    if(blend == 255) return color2;

    u8 w1 = (color1 >> 24) & 0xff;
    u8 r1 = (color1 >> 16) & 0xff;
    u8 g1 = (color1 >>  8) & 0xff;
    u8 b1 =  color1        & 0xff;

    u8 w2 = (color2 >> 24) & 0xff;
    u8 r2 = (color2 >> 16) & 0xff;
    u8 g2 = (color2 >>  8) & 0xff;
    u8 b2 =  color2        & 0xff;

    u32 w3 = ((w2 * blend) + (w1 * (255U - blend))) / 256U;
    u32 r3 = ((r2 * blend) + (r1 * (255U - blend))) / 256U;
    u32 g3 = ((g2 * blend) + (g1 * (255U - blend))) / 256U;
    u32 b3 = ((b2 * blend) + (b1 * (255U - blend))) / 256U;

    return ((w3 << 24) | (r3 << 16) | (g3 << 8) | (b3));
}


/*
 * twinkle_fade function
 */
u16 WS2812FX_twinkle_fade(u32 color) {
    WS2812FX_fade_out();

    if(WS2812FX_random8_lim(3) == 0) {
        u8 size = 1 << SIZE_OPTION;
        u16 index = _seg->start + WS2812FX_random16_lim(_seg_len - size);
        Adafruit_NeoPixel_fill(color, index, size);
        SET_CYCLE;
    }
    return (_seg->speed / 8);
}


/*
 * Blink several LEDs on, fading out.
 */
u16 WS2812FX_mode_twinkle_fade(void) {
    return WS2812FX_twinkle_fade(_seg->colors[0]);
}


/*
 * Blink several LEDs in random colors on, fading out.
 */
u16 WS2812FX_mode_twinkle_fade_random(void) {
    return WS2812FX_twinkle_fade(WS2812FX_color_wheel(WS2812FX_random8()));
}

/*
 * Sparkle function
 * color1 = background color
 * color2 = sparkle color
 */
u16 WS2812FX_sparkle(u32 color1, u32 color2) {
    if(_seg_rt->counter_mode_step == 0) {
        Adafruit_NeoPixel_fill(color1, _seg->start, _seg_len);
    }

    u8 size = 1 << SIZE_OPTION;
    Adafruit_NeoPixel_fill(color1, _seg->start + _seg_rt->aux_param3, size);

    _seg_rt->aux_param3 = WS2812FX_random16_lim(_seg_len - size); // aux_param3 stores the random led index
    Adafruit_NeoPixel_fill(color2, _seg->start + _seg_rt->aux_param3, size);

    SET_CYCLE;
    return (_seg->speed / 32);
}


/*
 * Blinks one LED at a time.
 * Inspired by www.tweaking4all.com/hardware/arduino/arduino-led-strip-effects/
 */
u16 WS2812FX_mode_sparkle(void) {
    return WS2812FX_sparkle(_seg->colors[1], _seg->colors[0]);
}


/*
 * Lights all LEDs in the color. Flashes white pixels randomly.
 * Inspired by www.tweaking4all.com/hardware/arduino/arduino-led-strip-effects/
 */
u16 WS2812FX_mode_flash_sparkle(void) {
    return WS2812FX_sparkle(_seg->colors[0], WHITE);
}


/*
 * Like flash sparkle. With more flash.
 * Inspired by www.tweaking4all.com/hardware/arduino/arduino-led-strip-effects/
 */
u16 WS2812FX_mode_hyper_sparkle(void) {
    Adafruit_NeoPixel_fill(_seg->colors[0], _seg->start, _seg_len);

    u8 size = 1 << SIZE_OPTION;
    for(u8 i=0; i<8; i++) {
        Adafruit_NeoPixel_fill(WHITE, _seg->start + WS2812FX_random16_lim(_seg_len - size), size);
    }

    SET_CYCLE;
    return (_seg->speed / 32);
}


/*
 * Strobe effect with different strobe count and pause, controlled by speed.
 */
u16 WS2812FX_mode_multi_strobe(void) {
    Adafruit_NeoPixel_fill(_seg->colors[1], _seg->start, _seg_len);

    u16 delay = 200 + ((9 - (_seg->speed % 10)) * 100);
    u16 count = 2 * ((_seg->speed / 100) + 1);
    if(_seg_rt->counter_mode_step < count) {
        if((_seg_rt->counter_mode_step & 1) == 0) {
            Adafruit_NeoPixel_fill(_seg->colors[0], _seg->start, _seg_len);
            delay = 20;
        }else {
            delay = 50;
        }
    }

    _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % (count + 1);
    if(_seg_rt->counter_mode_step == 0) SET_CYCLE;
    return delay;
}


/*
 * color chase function.
 * color1 = background color
 * color2 and color3 = colors of two adjacent leds
 */
u16 WS2812FX_chase(u32 color1, u32 color2, u32 color3) {
    u8 size = 1 << SIZE_OPTION;
    for(u8 i=0; i<size; i++) {
        u16 a = (_seg_rt->counter_mode_step + i) % _seg_len;
        u16 b = (a + size) % _seg_len;
        u16 c = (b + size) % _seg_len;
        if(IS_REVERSE) {
            WS2812FX_setPixelColor(_seg->stop - a, color1);
            WS2812FX_setPixelColor(_seg->stop - b, color2);
            WS2812FX_setPixelColor(_seg->stop - c, color3);
        }else {
            WS2812FX_setPixelColor(_seg->start + a, color1);
            WS2812FX_setPixelColor(_seg->start + b, color2);
            WS2812FX_setPixelColor(_seg->start + c, color3);
        }
    }

    if(_seg_rt->counter_mode_step + (size * 3) == _seg_len) SET_CYCLE;

    _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % _seg_len;
    return (_seg->speed / _seg_len);
}


/*
 * Bicolor chase mode
 */
u16 WS2812FX_mode_bicolor_chase(void) {
    return WS2812FX_chase(_seg->colors[0], _seg->colors[1], _seg->colors[2]);
}


/*
 * White running on _color.
 */
u16 WS2812FX_mode_chase_color(void) {
    return WS2812FX_chase(_seg->colors[0], WHITE, WHITE);
}


/*
 * Black running on _color.
 */
u16 WS2812FX_mode_chase_blackout(void) {
    return WS2812FX_chase(_seg->colors[0], BLACK, BLACK);
}


/*
 * _color running on white.
 */
u16 WS2812FX_mode_chase_white(void) {
    return WS2812FX_chase(WHITE, _seg->colors[0], _seg->colors[0]);
}


/*
 * White running followed by random color.
 */
u16 WS2812FX_mode_chase_random(void) {
    if(_seg_rt->counter_mode_step == 0) {
        _seg_rt->aux_param = WS2812FX_get_random_wheel_index(_seg_rt->aux_param);
    }
    return WS2812FX_chase(WS2812FX_color_wheel(_seg_rt->aux_param), WHITE, WHITE);
}


/*
 * Rainbow running on white.
 */
u16 WS2812FX_mode_chase_rainbow_white(void) {
    u16 n = _seg_rt->counter_mode_step;
    u16 m = (_seg_rt->counter_mode_step + 1) % _seg_len;
    u32 color2 = WS2812FX_color_wheel(((n * 256 / _seg_len) + (_seg_rt->counter_mode_call & 0xFF)) & 0xFF);
    u32 color3 = WS2812FX_color_wheel(((m * 256 / _seg_len) + (_seg_rt->counter_mode_call & 0xFF)) & 0xFF);

    return WS2812FX_chase(WHITE, color2, color3);
}


/*
 * White running on rainbow.
 */
u16 WS2812FX_mode_chase_rainbow(void) {
    u8 color_sep = 256 / _seg_len;
    u8 color_index = _seg_rt->counter_mode_call & 0xFF;
    u32 color = WS2812FX_color_wheel(((_seg_rt->counter_mode_step * color_sep) + color_index) & 0xFF);

    return WS2812FX_chase(color, WHITE, WHITE);
}


/*
 * Black running on rainbow.
 */
u16 WS2812FX_mode_chase_blackout_rainbow(void) {
    u8 color_sep = 256 / _seg_len;
    u8 color_index = _seg_rt->counter_mode_call & 0xFF;
    u32 color = WS2812FX_color_wheel(((_seg_rt->counter_mode_step * color_sep) + color_index) & 0xFF);

    return WS2812FX_chase(color, BLACK, BLACK);
}

/*
 * running white flashes function.
 * color1 = background color
 * color2 = flash color
 */
u16 WS2812FX_chase_flash(u32 color1, u32 color2) {
    const static u8 flash_count = 4;
    u8 flash_step = _seg_rt->counter_mode_call % ((flash_count * 2) + 1);

    if(flash_step < (flash_count * 2)) {
        u32 color = (flash_step % 2 == 0) ? color2 : color1;
        u16 n = _seg_rt->counter_mode_step;
        u16 m = (_seg_rt->counter_mode_step + 1) % _seg_len;
        if(IS_REVERSE) {
            WS2812FX_setPixelColor(_seg->stop - n, color);
            WS2812FX_setPixelColor(_seg->stop - m, color);
        }else {
            WS2812FX_setPixelColor(_seg->start + n, color);
            WS2812FX_setPixelColor(_seg->start + m, color);
        }
        return 30;
    }else {
        _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % _seg_len;
        if(_seg_rt->counter_mode_step == 0) {
            // update aux_param so mode_chase_flash_random() will select the next color
            _seg_rt->aux_param = WS2812FX_get_random_wheel_index(_seg_rt->aux_param);
            SET_CYCLE;
        }
    }

    return (_seg->speed / _seg_len);
}

/*
 * White flashes running on _color.
 */
u16 WS2812FX_mode_chase_flash(void) {
    return WS2812FX_chase_flash(_seg->colors[0], WHITE);
}


/*
 * White flashes running, followed by random color.
 */
u16 WS2812FX_mode_chase_flash_random(void) {
    return WS2812FX_chase_flash(WS2812FX_color_wheel(_seg_rt->aux_param), WHITE);
}


/*
 * Alternating pixels running function.
 */
u16 WS2812FX_running(u32 color1, u32 color2) {
    u8 size = 2 << SIZE_OPTION;
    u32 color = (_seg_rt->counter_mode_step & size) ? color1 : color2;

    if(IS_REVERSE) {
        WS2812FX_copyPixels(_seg->start, _seg->start + 1, _seg_len - 1);
        WS2812FX_setPixelColor(_seg->stop, color);
    }else {
        WS2812FX_copyPixels(_seg->start + 1, _seg->start, _seg_len - 1);
        WS2812FX_setPixelColor(_seg->start, color);
    }

    _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % _seg_len;
    if(_seg_rt->counter_mode_step == 0) SET_CYCLE;
    return (_seg->speed / _seg_len);
}


/*
 * Alternating color/white pixels running.
 */
u16 WS2812FX_mode_running_color(void) {
    return WS2812FX_running(_seg->colors[0], _seg->colors[1]);
}


/*
 * Alternating red/blue pixels running.
 */
u16 WS2812FX_mode_running_red_blue(void) {
    return WS2812FX_running(RED, BLUE);
}


/*
 * Alternating red/green pixels running.
 */
u16 WS2812FX_mode_merry_christmas(void) {
    return WS2812FX_running(RED, GREEN);
}

/*
 * Alternating orange/purple pixels running.
 */
u16 WS2812FX_mode_halloween(void) {
    return WS2812FX_running(PURPLE, ORANGE);
}


/*
 * Random colored pixels running.
 */
u16 WS2812FX_mode_running_random(void) {
    u8 size = 2 << SIZE_OPTION;
    if((_seg_rt->counter_mode_step) % size == 0) {
        _seg_rt->aux_param = WS2812FX_get_random_wheel_index(_seg_rt->aux_param);
    }

    u32 color = WS2812FX_color_wheel(_seg_rt->aux_param);

    return WS2812FX_running(color, color);
}


/*
 * K.I.T.T.
 */
u16 WS2812FX_mode_larson_scanner(void) {
    WS2812FX_fade_out();

    if(_seg_rt->counter_mode_step < _seg_len) {
        if(IS_REVERSE) {
            WS2812FX_setPixelColor(_seg->stop - _seg_rt->counter_mode_step, _seg->colors[0]);
        }else {
            WS2812FX_setPixelColor(_seg->start + _seg_rt->counter_mode_step, _seg->colors[0]);
        }
    }else {
        u16 index = (_seg_len * 2) - _seg_rt->counter_mode_step - 2;
        if(IS_REVERSE) {
            WS2812FX_setPixelColor(_seg->stop - index, _seg->colors[0]);
        } else {
            WS2812FX_setPixelColor(_seg->start + index, _seg->colors[0]);
        }
    }

    _seg_rt->counter_mode_step++;
    if(_seg_rt->counter_mode_step >= (u16)((_seg_len * 2) - 2)) {
        _seg_rt->counter_mode_step = 0;
        SET_CYCLE;
    }

    return (_seg->speed / (_seg_len * 2));
}


/*
 * Firing comets from one end.
 */
u16 WS2812FX_mode_comet(void) {
    WS2812FX_fade_out();

    if(IS_REVERSE) {
        WS2812FX_setPixelColor(_seg->stop - _seg_rt->counter_mode_step, _seg->colors[0]);
    } else {
        WS2812FX_setPixelColor(_seg->start + _seg_rt->counter_mode_step, _seg->colors[0]);
    }

    _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % _seg_len;
    if(_seg_rt->counter_mode_step == 0) SET_CYCLE;

    return (_seg->speed / _seg_len);
}


/*
 * Fireworks function.
 */
u16 WS2812FX_fireworks(u32 color) {
    WS2812FX_fade_out();

    // for better performance, manipulate the Adafruit_NeoPixels pixels[] array directly
    u8 *pixels = Adafruit_NeoPixel_getPixels();
    u8 bytesPerPixel = Adafruit_NeoPixel_getNumBytesPerPixel(); // 3=RGB, 4=RGBW
    u16 startPixel = _seg->start * bytesPerPixel + bytesPerPixel;
    u16 stopPixel = _seg->stop * bytesPerPixel ;
    for(u16 i = startPixel; i < stopPixel; i++) {
        u16 tmpPixel = (pixels[i - bytesPerPixel] >> 2) + pixels[i] + (pixels[i + bytesPerPixel] >> 2);
        pixels[i] =  tmpPixel > 255 ? 255 : tmpPixel;
    }

    u8 size = 2 << SIZE_OPTION;
    if(!_triggered) {
        for(u16 i=0; i<max(1, _seg_len/20); i++) {
            if(WS2812FX_random8_lim(10) == 0) {
                u16 index = _seg->start + WS2812FX_random16_lim(_seg_len - size);
                Adafruit_NeoPixel_fill(color, index, size);
                SET_CYCLE;
            }
        }
    }else {
        for(u16 i=0; i<max(1, _seg_len/10); i++) {
            u16 index = _seg->start + WS2812FX_random16_lim(_seg_len - size);
            Adafruit_NeoPixel_fill(color, index, size);
            SET_CYCLE;
        }
    }

    return (_seg->speed / _seg_len);
}

/*
 * Firework sparks.
 */
u16 WS2812FX_mode_fireworks(void) {
    u32 color = BLACK;
    do{ // randomly choose a non-BLACK color from the colors array
        color = _seg->colors[WS2812FX_random8_lim(MAX_NUM_COLORS)];
    }while(color == BLACK);
    return WS2812FX_fireworks(color);
}

/*
 * Random colored firework sparks.
 */
u16 WS2812FX_mode_fireworks_random(void) {
  return WS2812FX_fireworks(WS2812FX_color_wheel(WS2812FX_random8()));
}


/*
 * Fire flicker function
 */
u16 WS2812FX_fire_flicker(int rev_intensity) {
    u8 w = (_seg->colors[0] >> 24) & 0xFF;
    u8 r = (_seg->colors[0] >> 16) & 0xFF;
    u8 g = (_seg->colors[0] >>  8) & 0xFF;
    u8 b = (_seg->colors[0]        & 0xFF);
    u8 lum = max(w, max(r, max(g, b))) / rev_intensity;
    for(u16 i=_seg->start; i <= _seg->stop; i++) {
        int flicker = WS2812FX_random8_lim(lum);
        WS2812FX_setPixelColor_rgbw(i, max(r - flicker, 0), max(g - flicker, 0), max(b - flicker, 0), max(w - flicker, 0));
    }

    SET_CYCLE;
    return (_seg->speed / _seg_len);
}

/*
 * Random flickering.
 */
u16 WS2812FX_mode_fire_flicker(void) {
    return WS2812FX_fire_flicker(3);
}

/*
* Random flickering, less intensity.
*/
u16 WS2812FX_mode_fire_flicker_soft(void) {
    return WS2812FX_fire_flicker(6);
}

/*
* Random flickering, more intensity.
*/
u16 WS2812FX_mode_fire_flicker_intense(void) {
    return WS2812FX_fire_flicker(1);
}


// Matrix RGB effect
struct Matrix _matrix;      // global variable, so this custom effect shouldn't be used in more then one segment

void configMatrix(u8 numPages, u8 numRows, u8 numCols, u32* colors) {
    _matrix.numPages = numPages;
    _matrix.numRows = numRows;
    _matrix.numCols = numCols;
    _matrix.colors = colors;
}

u16 WS2812FX_mode_matrix(void) {
    Segment* seg = WS2812FX_getSegment();
    Segment_runtime* segrt = WS2812FX_getSegmentRuntime();
    int seglen = seg->stop - seg->start + 1;

    u16 segIndex = seg->start;
    u8 pageIndex = segrt->aux_param * _matrix.numRows * _matrix.numCols;    // aux_param will store the page index

    for(int rowIndex=0; rowIndex < _matrix.numRows; rowIndex++) {
        u16 matrixIndex = pageIndex + (rowIndex * _matrix.numCols);
        for(int colIndex=0; colIndex < _matrix.numCols; colIndex++) {
            if(segIndex <= seg->stop) {
                WS2812FX_setPixelColor(segIndex, _matrix.colors[matrixIndex + colIndex]);
                segIndex++;
            }
        }
    }

    // increment to the next page
    segrt->aux_param < (_matrix.numPages - 1) ? segrt->aux_param++ : (segrt->aux_param = 0);
    if(segrt->aux_param == 0)       WS2812FX_setCycle();

    return seg->speed;
}


u16 WS2812FX_mode_rainbowLarson(void) {
    Segment* seg = WS2812FX_getSegment();               // get the current segment
    Segment_runtime* segrt = WS2812FX_getSegmentRuntime();
    int seglen = seg->stop - seg->start + 1;

    int8_t dir = ((segrt->aux_param2 & DIR_BIT) == DIR_BIT) ? -1 : 1; // forward?
    segrt->aux_param3 += dir;
    int16_t index = segrt->aux_param3 % seglen;

    WS2812FX_fade_out();

    if(seg->options == NO_OPTIONS) {
        int8_t cnt = segrt->aux_param++;
        WS2812FX_setPixelColor(seg->start + index, WS2812FX_color_wheel((cnt % 8) * 32));
    }else {
        WS2812FX_setPixelColor(seg->start + index, WS2812FX_color_wheel((index * 256) / seglen));
    }

    if(segrt->aux_param3 >= (seg->stop - seg->start) || segrt->aux_param3 <= 0) {
        segrt->aux_param2 ^= DIR_BIT;           // change direction
        WS2812FX_setCycle();
    }

    return (seg->speed / (seglen * 2));
}


u16 WS2812FX_mode_dualLarson(void) {
  Segment* seg = WS2812FX_getSegment();         // get the current segment
  Segment_runtime* segrt = WS2812FX_getSegmentRuntime();
  int seglen = seg->stop - seg->start + 1;

  static int16_t offset = 0;
  int8_t dir = segrt->aux_param ? -1 : 1;
  segrt->aux_param3 += dir;

  WS2812FX_fade_out();

  int16_t segIndex = (segrt->aux_param3 + offset) % seglen;
  WS2812FX_setPixelColor(seg->start + segIndex, seg->colors[0]);
  WS2812FX_setPixelColor(seg->stop  - segIndex, seg->colors[2]);

  if(segrt->aux_param3 >= (seg->stop - seg->start) || segrt->aux_param3 <= 0) {
    segrt->aux_param = !segrt->aux_param;
    if(seg->options & REVERSE) offset = (offset + 1) % seglen;
    if(!segrt->aux_param)       WS2812FX_setCycle();
  }

  return (seg->speed / (seglen * 2));
}


u16 WS2812FX_mode_fillerup(void) {
    Segment* seg = WS2812FX_getSegment();       // get the current segment
    Segment_runtime* segrt = WS2812FX_getSegmentRuntime();
    int seglen = seg->stop - seg->start + 1;
    bool isReverse = (seg->options & REVERSE) == REVERSE;

    // set the forground and background colors
    u32 fgColor = seg->colors[0];
    u32 bgColor = seg->colors[1];
    if(segrt->aux_param) {                      // swap colors
        fgColor = seg->colors[1];
        bgColor = seg->colors[0];
    }

    // run the standard comet effect
    WS2812FX_fade_out_targetColor(bgColor);
    if(isReverse) {
        WS2812FX_setPixelColor(seg->stop  - segrt->counter_mode_step, fgColor);
    }else {
        WS2812FX_setPixelColor(seg->start + segrt->counter_mode_step, fgColor);
    }
    segrt->counter_mode_step = (segrt->counter_mode_step + 1) % seglen;

    // when drop reaches the fill line, incr the fill line
    if(segrt->counter_mode_step >= seglen - segrt->aux_param3 - 1) {
        segrt->aux_param3 += ((seg->options >> 1) & 3) + 1; // increment fill line by SIZE
        segrt->counter_mode_step = 0;
    }

    // repaint the "filled" portion every time
    for(u8 i = 0; i < segrt->aux_param3; i++) {
        if(isReverse) {
            WS2812FX_setPixelColor(seg->start + i, fgColor);
        }else {
            WS2812FX_setPixelColor(seg->stop - i,  fgColor);
        }
    }

    // when "glass" is full, flip the colors and start again
    if(segrt->aux_param3 >= seglen) {
        segrt->aux_param = !segrt->aux_param;
        segrt->aux_param3 = 0;
        WS2812FX_setCycle();
    }

    return (seg->speed / seglen);
}


u16 WS2812FX_mode_icu(void) {
    Segment* seg = WS2812FX_getSegment();       // get the current segment
    Segment_runtime* segrt = WS2812FX_getSegmentRuntime();
    int seglen = seg->stop - seg->start + 1;

    u16 dest = segrt->counter_mode_step & 0xFFFF;

    WS2812FX_setPixelColor(seg->start + dest, seg->colors[0]);
    WS2812FX_setPixelColor(seg->start + dest + seglen/2, seg->colors[0]);

    if(segrt->aux_param3 == dest) {     // pause between eye movements
        if(WS2812FX_random8_lim(6) == 0) {  // blink once in a while
            WS2812FX_setPixelColor(seg->start + dest, BLACK);
            WS2812FX_setPixelColor(seg->start + dest + seglen/2, BLACK);
            return 200;
        }
        segrt->aux_param3 = WS2812FX_random16_lim(seglen/2);
        WS2812FX_setCycle();
        return 1000 + WS2812FX_random16_lim(2000);
    }

    WS2812FX_setPixelColor(seg->start + dest, BLACK);
    WS2812FX_setPixelColor(seg->start + dest + seglen/2, BLACK);

    if(segrt->aux_param3 > segrt->counter_mode_step) {
        segrt->counter_mode_step++;
        dest++;
    }else if (segrt->aux_param3 < segrt->counter_mode_step) {
        segrt->counter_mode_step--;
        dest--;
    }

    WS2812FX_setPixelColor(seg->start + dest, seg->colors[0]);
    WS2812FX_setPixelColor(seg->start + dest + seglen/2, seg->colors[0]);

    return (seg->speed / seglen);
}


u16 WS2812FX_mode_multiComet(void) {
    Segment* seg = WS2812FX_getSegment();     // get the current segment
    int seglen = seg->stop - seg->start + 1;

    bool isReverse = (seg->options & REVERSE) == REVERSE;

    WS2812FX_fade_out();

    static int16_t comets[] = {32767, 32767, 32767, 32767, 32767, 32767};
    static int8_t numComets = sizeof(comets)/sizeof(comets[0]);

    for(u8 i=0; i < numComets; i++) {
        if(comets[i] < seglen) {
            if(isReverse) {
                WS2812FX_setPixelColor(seg->stop - comets[i],  i % 2 ? seg->colors[0] : seg->colors[2]);
            }else {
                WS2812FX_setPixelColor(seg->start + comets[i], i % 2 ? seg->colors[0] : seg->colors[2]);
            }
            comets[i]++;
        }else {
            if(!WS2812FX_random16_lim(seglen)) {
                comets[i] = 0;
                WS2812FX_setCycle();
            }
        }
    }

    return (seg->speed / seglen);
}


u16 WS2812FX_mode_heartbeat(void) {
    static unsigned long lastBeat = 0;
    static bool secondBeatActive = false;

    Segment* seg = WS2812FX_getSegment();             // get the current segment
    int seglen = seg->stop - seg->start + 1;

    // Get and translate the segment's size option
    u8 size = 2 << ((seg->options >> 1) & 0x03);      // 2,4,8,16

    // copy pixels from the middle of the segment to the edges
    u16 bytesPerPixelBlock = size * WS2812FX_getNumBytesPerPixel();
    u16 centerOffset = (seglen / 2) * WS2812FX_getNumBytesPerPixel();
    u16 byteCount = centerOffset - bytesPerPixelBlock;
    memmove(WS2812FX_getPixels(), WS2812FX_getPixels() + bytesPerPixelBlock, byteCount);
    memmove(WS2812FX_getPixels() + centerOffset + bytesPerPixelBlock, WS2812FX_getPixels() + centerOffset, byteCount);

    WS2812FX_fade_out();

    unsigned long beatTimer = millis() - lastBeat;
    if((beatTimer > SECOND_BEAT) && !secondBeatActive) { // time for the second beat?
        beatIt(seg, size);          // create the second beat
        secondBeatActive = true;
    }
    if(beatTimer > MS_PER_BEAT) {   // time to reset the beat timer?
        beatIt(seg, size);          // create the first beat
        secondBeatActive = false;
        lastBeat = millis();
        WS2812FX_setCycle();
    }

    return(seg->speed / 32);
}

// light up ('size' * 2) LEDs in the middle of the segment (starts a beat)
void beatIt(Segment* seg, u8 size) {
    int seglen = seg->stop - seg->start + 1;
    u16 startLed = seg->start + (seglen / 2) - size;
    for(u16 i = startLed; i < startLed + (size * 2); i++) {
        WS2812FX_setPixelColor(i, seg->colors[0]);
    }
}

// random generate 
u8 custom_rand(u8 min, u8 max) {
    srand(millis());
    return ((rand() % (max - min))+ min);
}

u16 WS2812FX_mode_oscillate(void) {
    static oscillator oscillators[3];
    Segment* seg = WS2812FX_getSegment(); // get the current segment
    int seglen = seg->stop - seg->start + 1;

    oscillators[0].pos = seglen/4;
    oscillators[0].size = seglen/8;
    oscillators[0].dir = 1;
    oscillators[0].speed = 1;

    oscillators[1].pos = seglen/4*2;
    oscillators[1].size = seglen/8;
    oscillators[1].dir = -1;
    oscillators[1].speed = 1;

    oscillators[2].pos = seglen/4*3;
    oscillators[2].size = seglen/8;
    oscillators[2].dir = 1;
    oscillators[2].speed = 2;

    for(int8_t i=0; i < sizeof(oscillators)/sizeof(oscillators[0]); i++) {
        oscillators[i].pos += oscillators[i].dir * oscillators[i].speed;
        if((oscillators[i].dir == -1) && (oscillators[i].pos <= 0)) {
            oscillators[i].pos = 0;
            oscillators[i].dir = 1;
            oscillators[i].speed = WS2812FX_random8_lim(3) + 1;
            WS2812FX_setCycle();
        }
        if((oscillators[i].dir == 1) && (oscillators[i].pos >= (seglen - 1))) {
            oscillators[i].pos = seglen - 1;
            oscillators[i].dir = -1;
            oscillators[i].speed = WS2812FX_random8_lim(3) + 1;
            WS2812FX_setCycle();
        }
    }

    for(int16_t i=0; i < seglen; i++) {
        u32 color = BLACK;
        for(int8_t j = 0; j < sizeof(oscillators)/sizeof(oscillators[0]); j++) {
            if(i >= oscillators[j].pos - oscillators[j].size && i <= oscillators[j].pos + oscillators[j].size) {
                color = (color == BLACK) ? seg->colors[j] : WS2812FX_color_blend(color, seg->colors[j], 128);
            }
        }
        WS2812FX_setPixelColor(seg->start + i, color);
    }
    return(seg->speed / 8);
}


u16 WS2812FX_mode_rain(void) {
    Segment* seg = WS2812FX_getSegment();       // get the current segment
    u16 seglen = seg->stop - seg->start + 1;
    u32 rainColor = (WS2812FX_random8() & 1) == 0 ? seg->colors[0] : seg->colors[2];
    if(seg->colors[0] == seg->colors[1])    rainColor = WS2812FX_color_wheel(WS2812FX_random8());

    WS2812FX_fireworks(rainColor);

    // shift everything two pixels
    bool isReverse = (seg->options & REVERSE) != 0;
    if(isReverse) {
        WS2812FX_copyPixels(seg->start + 2, seg->start, seglen - 2);
    }else {
        WS2812FX_copyPixels(seg->start, seg->start + 2, seglen - 2);
    }

    return(seg->speed / seglen);
}


u16 WS2812FX_mode_rainbowFireworks(void) {
    Segment* seg = WS2812FX_getSegment();       // get the current segment
    u16 seglen = seg->stop - seg->start + 1;

    WS2812FX_setBrightness(255);                // force full brightness
    WS2812FX_fade_out();

    for(u16 i=seg->start + 6; i <= seg->stop - 6; i++) {
        u32 color = Adafruit_NeoPixel_getPixelColor(i);

        if(color == 0x7F0000) {
            WS2812FX_setPixelColor(i-1, 0xFF7F00); // orange
            WS2812FX_setPixelColor(i+1, 0xFF7F00);
        }
        if(color == 0x3F0000) {
            WS2812FX_setPixelColor(i-2, 0xFFFF00); // yellow
            WS2812FX_setPixelColor(i+2, 0xFFFF00);
        }
        if(color == 0x1F0000) {
            WS2812FX_setPixelColor(i-3, 0x00FF00); // green
            WS2812FX_setPixelColor(i+3, 0x00FF00);
        }
        if(color == 0x0F0000) {
            WS2812FX_setPixelColor(i-4, 0x0000FF); // blue
            WS2812FX_setPixelColor(i+4, 0x0000FF);
        }
        if(color == 0x070000) {
            WS2812FX_setPixelColor(i-5, 0x4B0082); // indigo
            WS2812FX_setPixelColor(i+5, 0x4B0082);
        }
        if(color == 0x030000) {
            WS2812FX_setPixelColor(i-6, 0x9400D3); // violet
            WS2812FX_setPixelColor(i+6, 0x9400D3);
        }
    }

    // randomly create a red pixel
    for(u16 i = 0; i < max(1, seglen/20); i++) {
        if(WS2812FX_random8_lim(10) == 0) {
            u16 index = seg->start + 6 + WS2812FX_random16_lim(seglen - 12);
            WS2812FX_setPixelColor(index, RED);
            WS2812FX_setCycle();
        }
    }

    return(seg->speed / seglen);
}


u16 WS2812FX_mode_randomChase(void) {
    Segment* seg = WS2812FX_getSegment();
    Segment_runtime* segrt = WS2812FX_getSegmentRuntime();
    int seglen = seg->stop - seg->start + 1;

    WS2812FX_copyPixels(seg->start + 1, seg->start, seglen - 1);
    u32 color = Adafruit_NeoPixel_getPixelColor(seg->start + 1);
    int r = custom_rand(0, 6) != 0 ? (color >> 16 & 0xFF) : custom_rand(0, 256);
    int g = custom_rand(0, 6) != 0 ? (color >> 8  & 0xFF) : custom_rand(0, 256);
    int b = custom_rand(0, 6) != 0 ? (color       & 0xFF) : custom_rand(0, 256);
    WS2812FX_setPixelColor_rgb(seg->start, r, g, b);

    if((segrt->counter_mode_call % seglen) == 0)  WS2812FX_setCycle();
    return seg->speed;
}


u16 WS2812FX_mode_triFade(void) {
    Segment* seg = WS2812FX_getSegment();       // get the current segment
    Segment_runtime* segrt = WS2812FX_getSegmentRuntime();

    u8 options = seg->options;
    bool trifade_black = (options & TRIFADE_BLACK) == TRIFADE_BLACK;

    u16 count = segrt->aux_param3;
    u32 color1 = 0, color2 = 0;

    if(count < 1536) {
        color1 = trifade_black ? BLACK: seg->colors[2];
        color2 = seg->colors[0];
    }
    if(count < 1280) {
        color1 = trifade_black ? seg->colors[2] : seg->colors[1];
        color2 = trifade_black ? BLACK : seg->colors[2];
    }
    if(count < 1024) {
        color1 = trifade_black ? BLACK : seg->colors[0];
        color2 = trifade_black ? seg->colors[2] : seg->colors[1];
    }
    if(count < 768) {
        color1 = trifade_black ? seg->colors[1] : seg->colors[2];
        color2 = trifade_black ? BLACK : seg->colors[0];
    }
    if(count < 512) {
        color1 = trifade_black ? BLACK : seg->colors[1];
        color2 = trifade_black ? seg->colors[1] : seg->colors[2];
    }
    if(count < 256) {
        color1 = seg->colors[0];
        color2 = trifade_black ? BLACK : seg->colors[1];
    }

    u32 color = WS2812FX_color_blend(color1, color2, count % 256);
    for(u16 i = seg->start; i <= seg->stop; i++) {
        WS2812FX_setPixelColor(i, color);
    }

    segrt->aux_param3 += 4;
    if(segrt->aux_param3 >= 1536) {
        segrt->aux_param3 = 0;
        WS2812FX_setCycle();
    }

    return (seg->speed / 128);
}


u16 WS2812FX_mode_twinkleFox(void) {
    u16 mySeed = 0;                                 // reset the random number generator seed
    Segment* seg = WS2812FX_getSegment();           // get the current segment
    Segment_runtime* segrt = WS2812FX_getSegmentRuntime();

    // Get and translate the segment's size option
    u8 size = 1 << ((seg->options >> 1) & 0x03);    // 1,2,4,8

    // Get the segment's colors array values
    u32 color0 = seg->colors[0];
    u32 color1 = seg->colors[1];
    u32 color2 = seg->colors[2];
    u32 blendedColor;

    for (u16 i = seg->start; i <= seg->stop; i+=size) {
        // Use Mark Kriegsman's clever idea of using pseudo-random numbers to determine
        // each LED's initial and increment blend values
        mySeed = (mySeed * 2053) + 13849; // a random, but deterministic, number
        u16 initValue = (mySeed + (mySeed >> 8)) & 0xff; // the LED's initial blend index (0-255)
        mySeed = (mySeed * 2053) + 13849; // another random, but deterministic, number
        u16 incrValue = (((mySeed + (mySeed >> 8)) & 0x07) + 1) * 2; // blend index increment (2,4,6,8,10,12,14,16)

        // We're going to use a sine function to blend colors, instead of Mark's triangle
        // function, simply because a sine lookup table is already built into the
        // Adafruit_NeoPixel lib. Yes, I'm lazy.
        // Use the counter_mode_call var as a clock "tick" counter and calc the blend index
        u8 blendIndex = (initValue + (segrt->counter_mode_call * incrValue)) & 0xff; // 0-255
        // Index into the built-in Adafruit_NeoPixel sine table to lookup the blend amount
        u8 blendAmt = Adafruit_NeoPixel_sine8(blendIndex); // 0-255

        // If colors[0] is BLACK, blend random colors
        if(color0 == BLACK) {
            blendedColor = WS2812FX_color_blend(WS2812FX_color_wheel(initValue), color1, blendAmt);
            // If colors[2] isn't BLACK, choose to blend colors[0]/colors[1] or colors[1]/colors[2]
            // (which color pair to blend is picked randomly)
        }else if((color2 != BLACK) && (initValue < 128) == 0) {
            blendedColor = WS2812FX_color_blend(color2, color1, blendAmt);
            // Otherwise always blend colors[0]/colors[1]
        }else {
            blendedColor = WS2812FX_color_blend(color0, color1, blendAmt);
        }

        // Assign the new color to the number of LEDs specified by the SIZE option
        for(u8 j = 0; j < size; j++) {
            if((i + j) <= seg->stop) {
                WS2812FX_setPixelColor(i + j, blendedColor);
            }
        }
    }
    WS2812FX_setCycle();
    return seg->speed / 32;
}


// set USE_RANDOM_DATA to false in the line below if vuMeterBands[] is populated
// by an external data source. otherwise random data will be used for the effect.
u8 vuMeterBands[2];        // global VU meter band amplitude data (range 0-255)

u16 WS2812FX_mode_vuMeter(void) {
    Segment* seg = WS2812FX_getSegment();
    u16 seglen = seg->stop - seg->start + 1;
    u16 bandSize = seglen / 2;

    for(u8 i = 0; i < 2; i++) {
        int randomData = vuMeterBands[i] + WS2812FX_random8_lim(32) - WS2812FX_random8_lim(32);
        vuMeterBands[i] = (randomData < 0 || randomData > 255) ? 128 : randomData;


        u8 scaledBand = (vuMeterBands[i] * bandSize) / 256;
        for(u16 j = 0; j < bandSize; j++) {
            u16 index = seg->start + (i * bandSize) + j;
            if(j <= scaledBand) {
                if(j < bandSize - 4)        WS2812FX_setPixelColor(index, seg->colors[0]);
                else if(j < bandSize - 2)   WS2812FX_setPixelColor(index, seg->colors[1]);
                else                        WS2812FX_setPixelColor(index, seg->colors[2]);
            }else {
                WS2812FX_setPixelColor(index, BLACK);
            }
        }
    }
    WS2812FX_setCycle();

    return seg->speed;
}


u16 WS2812FX_mode_popcorn(void) {
    Segment* seg = WS2812FX_getSegment();       // get the current segment
    u16 seglen = seg->stop - seg->start + 1;
    u32 popcornColor = seg->colors[0];
    u32 bgColor = seg->colors[1];
    if(popcornColor == bgColor) popcornColor = WS2812FX_color_wheel(WS2812FX_random8());
        bool isReverse = (seg->options & REVERSE) != 0;

    static kernel popcorn[MAX_NUM_POPCORN];
    static float coeff = 0.0f;
    if(coeff == 0.0f) { // calculate the velocity coeff once (the secret sauce)
    coeff = pow((float)seglen, 0.5223324f) * 0.3944296f;
    }

    // reset all LEDs to background color
    for(u16 i=seg->start; i <= seg->stop; i++) {
        WS2812FX_setPixelColor(i, bgColor);
    }

    u16 ledIndex;
    for(int8_t i=0; i < MAX_NUM_POPCORN; i++) {
        bool isActive = popcorn[i].position >= 0.0f;

        if(isActive) { // if kernel is active, update its position
            popcorn[i].position += popcorn[i].velocity;
            popcorn[i].velocity -= GRAVITY;
            ledIndex = isReverse ? seg->stop - popcorn[i].position : seg->start + popcorn[i].position;
            if(ledIndex >= seg->start && ledIndex <= seg->stop) WS2812FX_setPixelColor(ledIndex, popcorn[i].color);
        }else { // if kernel is inactive, randomly pop it
            if(WS2812FX_random8() < 2) { // POP!!!
                popcorn[i].position = 0.0f;
                popcorn[i].velocity = coeff * (custom_rand(66, 100) / 100.0f);
                popcorn[i].color = popcornColor;
                ledIndex = isReverse ? seg->stop : seg->start;
                WS2812FX_setPixelColor(ledIndex, popcorn[i].color);
                WS2812FX_setCycle();
            }
        }
    }

    return(seg->speed / seglen);
}


u16 WS2812FX_mode_blockDissolve(void) {
    Segment* seg = WS2812FX_getSegment();
    Segment_runtime* segrt = WS2812FX_getSegmentRuntime();
    int seglen = seg->stop - seg->start + 1;

    u32 color = seg->colors[segrt->aux_param];
    WS2812FX_setPixelColor(seg->start, color);
    // get the decimated color after setPixelColor() has mangled it
    // in accordance to the brightness setting
    u32 desColor = Adafruit_NeoPixel_getPixelColor(seg->start);

    for(u16 i = 0; i < seglen; i++) {
        int index = seg->start + WS2812FX_random16_lim(seglen);
        if(Adafruit_NeoPixel_getPixelColor(index) != desColor) {
            WS2812FX_setPixelColor(index, color);
            return seg->speed;
        }
    }

    for(u16 i = seg->start; i < seg->stop; i++) {
        WS2812FX_setPixelColor(i, color);
    }

    segrt->aux_param = (segrt->aux_param + 1) % MAX_NUM_COLORS;
    if(segrt->aux_param == 0)     WS2812FX_setCycle();

    return seg->speed;
}



