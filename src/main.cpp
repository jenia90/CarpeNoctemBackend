#include "defs.h"
#include <NeoPixelBus.h>
#include <IRremote.h>

#define NUM_OF_AREAS 5

// Define areas
const Area areas[NUM_OF_AREAS] = {
    {5, 5, "Area 1", AREA_ONE_CODE, new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(5, 5)},
    {5, 6, "Area 2", AREA_TWO_CODE, new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(5, 6)},
    {5, 7, "Area 3", AREA_THREE_CODE, new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(5, 7)},
    {5, 8, "Area 4", AREA_FOUR_CODE, new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(5, 8)},
    {5, 9, "Area 5", AREA_FIVE_CODE, new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(5, 9)},
};

// IR setup
const int IR_PIN = 12;
IRrecv recv(IR_PIN);
decode_results irData;

void setup()
{
    // Initialize serial for debugging
    Serial.begin(9600);
    Serial.println("Initializing...");
    Serial.flush();

    // Enable IR sensor
    Serial.println("Starting IR sensor...");
    recv.enableIRIn();
    Serial.println("IR enabled.");

    // Initialize led strips
    for(int i = 0; i < NUM_OF_AREAS; i++)
    {
        areas[i].strip->Begin();
        areas[i].strip->Show();
    }

    Serial.println();
    Serial.println("Running...");
}

void loop()
{
    if(recv.decode(&irData))
    {
        // Skip if button pressed longer than needed.
        if(irData.value != BLANK_CODE)
        {
            if(decodeRemoteCommands(&irData.value) == FAIL_CODE)
            {
                Serial.print("Code 0x");
                Serial.print(irData.value, HEX);
                Serial.println(" is unknown.");
            }
            // Serial.println(irData.value, HEX);
            Serial.flush();
        }
        recv.resume();
    }
}

/**
 * Activates areas based on the keys pressed.
 */
int decodeRemoteCommands(const unsigned long *data)
{
    switch (*data)
    {
        case ALL_AREAS_CODE: // Power Button
            Serial.println("Activating all areas...");
            for(int i = 0; i < NUM_OF_AREAS; i++)
                activateArea(&areas[i]);
            return SUCCESS_CODE;

        case AREA_ONE_CODE: // "1" Button
            if((areas[1].strip)->IsDirty());
            Serial.println("Activating area 1...");
            activateArea(&areas[0]);
            return SUCCESS_CODE;

        case AREA_TWO_CODE: // "2" Button
            Serial.println("Activating area 2...");
            activateArea(&areas[1]);
            return SUCCESS_CODE;

        case AREA_THREE_CODE: // "3" Button
            Serial.println("Activating area 3...");
            activateArea(&areas[2]);
            return SUCCESS_CODE;

        case AREA_FOUR_CODE: // "4" Button
            Serial.println("Activating area 4...");
            activateArea(&areas[3]);
            return SUCCESS_CODE;

        case AREA_FIVE_CODE: // "5" Button
            Serial.println("Activating area 5...");
            activateArea(&areas[4]);
            return SUCCESS_CODE;
        
        default:
            return FAIL_CODE;
    }

    return FAIL_CODE;
}

/**
 * Set all pixels in an area to white color
 */
int activateArea(const Area *area)
{
    Serial.print("Setting ");
    Serial.print(area->name);
    Serial.println(" to white");
    return setArea(area, RgbColor(255));
}

/**
 * Sets all pixels in an area to black
 */
int deactivateArea(const Area *area)
{
    return setArea(area, RgbColor(0));
}

/**
 * Set all pixels in an area to specific color
 */
int setArea(const Area *area, RgbColor color)
{
    area->strip->ClearTo(color);
    area->strip->Show();
    return SUCCESS_CODE;
}