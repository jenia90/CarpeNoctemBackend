#ifndef DEFS_H
#define DEFS_H

#include <Arduino.h>
#include <NeoPixelBus.h>


#define SUCCESS_CODE 0
#define FAIL_CODE -1


typedef struct {
    const uint16_t PixelCount;
    const uint8_t Pin;
    const String name;
    const unsigned long irCode;
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> *strip;
    bool state;
} Area; 

#define ALL_AREAS_CODE 0xFD00FF
#define AREA_ONE_CODE 0xFD08F7
#define AREA_TWO_CODE 0xFD8877
#define AREA_THREE_CODE 0xFD48B7
#define AREA_FOUR_CODE 0xFD28D7
#define AREA_FIVE_CODE 0xFDA857
#define BLANK_CODE 0xFFFFFFFF


int decodeRemoteCommands(const unsigned long *data);
int activateArea(Area *area);
int deactivateArea(Area *area);
int setArea(Area *area, RgbColor color);

#endif