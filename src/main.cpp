#include "defs.h"
#include <IRremote.h>


// Define areas
Area areas[NUM_OF_AREAS] = {
    {5, 5, "Area 1", AREA_ONE_CODE, new NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>(5, 5), false},
    {5, 6, "Area 2", AREA_TWO_CODE, new NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>(5, 6), false},
    {5, 7, "Area 3", AREA_THREE_CODE, new NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>(5, 7), false},
    {5, 8, "Area 4", AREA_FOUR_CODE, new NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>(5, 8), false},
    {5, 9, "Area 5", AREA_FIVE_CODE, new NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>(5, 9), false}
};

NeoPixelAnimator animations(NUM_OF_AREAS);
AnimState animStates[NUM_OF_AREAS];

// IR setup
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
                Serial.println(" is unknown or there was a problem with the LEDs.");
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
    uint16_t index;
    int (*func)(uint16_t,Area*);

    switch (*data)
    {
        case ALL_AREAS_CODE: // Power Button
        {    
            // We want to use a function to activate/deactivate all areas 
            // if one of the areas is activated then all will be deactivated and vice versa.
            String msg = "";
            if (!areas[0].state && 
                !areas[1].state && 
                !areas[2].state && 
                !areas[3].state && 
                !areas[4].state) 
            {
                func = activateArea;
                msg = "Activating all areas...";
            }
            else 
            {
                func = deactivateArea;
                msg = "Deactivating all areas...";
            }
            Serial.println(msg);

            for(int i = 0; i < NUM_OF_AREAS; i++)
                func(i, &areas[i]);

            return SUCCESS_CODE;
        }
        case AREA_ONE_CODE: // "1" Button
            index = 0;
            break;

        case AREA_TWO_CODE: // "2" Button
            index = 1;
            break;

        case AREA_THREE_CODE: // "3" Button
            index = 2;
            break;

        case AREA_FOUR_CODE: // "4" Button
            index = 3;
            break;

        case AREA_FIVE_CODE: // "5" Button
            index = 4;
            break;
        
        default:
            return FAIL_CODE;
    }

    Area *area = &areas[index];

    if(!(*area).state)
    {
        Serial.println("Activating " + (*area).name + "...");
        func = activateArea;
    }
    else
    {
        Serial.println("Deactivating " + (*area).name + "...");
        func = deactivateArea;
    }

    return func(index, area);
}

/**
 * Set all pixels in an area to white color
 */
int activateArea(uint16_t index, Area *area)
{
    Serial.println("Setting " + area->name + " to white");
    (*area).state = true;
    // fadeAreaColor(index, BLACK_COLOR, WHITE_COLOR);
    return setArea(area, WHITE_COLOR);
    // return SUCCESS_CODE;
}

/**
 * Sets all pixels in an area to black
 */
int deactivateArea(uint16_t index, Area *area)
{
    Serial.println("Setting " + area->name + " to black");
    (*area).state = false;
    // fadeAreaColor(index, WHITE_COLOR, BLACK_COLOR);
    return setArea(area, BLACK_COLOR);
    // return SUCCESS_CODE;
}

/**
 * Set all pixels in an area to specific color
 */
int setArea(Area *area, RgbColor color)
{
    area->strip->ClearTo(color);
    area->strip->Show();
    return SUCCESS_CODE;
}

/**
 * Animates the color fade using linear blend animation
 */
void blendAnimUpdate(const AnimationParam &param)
{
    RgbColor updatedColor = RgbColor::LinearBlend(
        animStates[param.index].InitColor, 
        animStates[param.index].FinColor, 
        param.progress
    );

    areas[param.index].strip->ClearTo(updatedColor);
}

/**
 * Starts the animation for color fade-in/fade-out
 */
void fadeAreaColor(uint16_t index, RgbColor initColor, RgbColor finColor)
{
    animStates[index] = {initColor, finColor};
    animations.StartAnimation(index, ANIM_TIME, blendAnimUpdate);
}