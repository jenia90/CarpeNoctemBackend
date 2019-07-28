#ifndef DEFS_H
#define DEFS_H

#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>


#define SUCCESS_CODE 0
#define FAIL_CODE -1


#define NUM_OF_AREAS 5
const int IR_PIN = 12;

// Area struct definition
typedef struct 
{
    const uint16_t PixelCount;
    const uint8_t Pin;
    const String name;
    NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod> *strip;
    bool state;
} Area; 

// Color definitions
#define WHITE_COLOR RgbColor(255)
#define BLACK_COLOR RgbColor(0)

// Animation definitions
const uint16_t ANIM_TIME = 200;
typedef struct 
{
    RgbColor InitColor;
    RgbColor FinColor;
} AnimState;

// IR codes definitions
#define ALL_AREAS_CODE 0xFD00FF
#define COLOR_ONE_CODE 0xFD08F7
#define COLOR_TWO_CODE 0xFD8877
#define COLOR_THREE_CODE 0xFD48B7
#define COLOR_FOUR_CODE 0xFD28D7
#define COLOR_FIVE_CODE 0xFDA857
#define COLOR_SIX_CODE 0xFD6897
#define COLOR_SEVEN_CODE 0xFD18E7
#define COLOR_EIGHT_CODE 0xFD9867
#define COLOR_NINE_CODE 0xFD58A7

#define BLANK_CODE 0xFFFFFFFF

// Function declarations
int decodeRemoteCommands(const unsigned long *data);
// int activateArea(uint16_t index, Area *area);
// int deactivateArea(uint16_t index, Area *area);
int setArea(RgbColor *color);
// void blendAnimUpdate(const AnimationParam &param);
// void fadeAreaColor(uint16_t index, RgbColor initColor, RgbColor finColor);

#endif