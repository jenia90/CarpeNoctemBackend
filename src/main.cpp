#include "defs.h"
#include <IRremote.h>


// Define areas
Area pixels = {20, 5, "Area 1", new NeoPixelBus<NeoRgbFeature, Neo800KbpsMethod>(20, 5), false};
RgbColor colors[7] = {WHITE_COLOR, 
                      RgbColor(236,195,28), 
                      RgbColor(255,0,156), 
                      RgbColor(0,168,255), 
                      RgbColor(255,156,0),
                      RgbColor(5,5,192),
                      RgbColor(150,0,255)};


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
    pixels.strip->Begin();
    pixels.strip->Show();

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
    RgbColor *color;

    switch (*data)
    {
        case ALL_AREAS_CODE: // Power Button
        {    
            // We want to use a function to activate/deactivate all areas 
            // if one of the areas is activated then all will be deactivated and vice versa.
            String msg = "";
            color = &colors[0];
            msg = "Deactivating pixels...";
            Serial.println(msg);

            setArea(&BLACK_COLOR);

            return SUCCESS_CODE;
        }
        case COLOR_ONE_CODE: // "1" Button
            index = 0;
            break;

        case COLOR_TWO_CODE: // "2" Button
            index = 1;
            break;

        case COLOR_THREE_CODE: // "3" Button
            index = 2;
            break;

        case COLOR_FOUR_CODE: // "4" Button
            index = 3;
            break;

        case COLOR_FIVE_CODE: // "5" Button
            index = 4;
            break;
        case COLOR_SIX_CODE: // "6" Button
            index = 5;
            break;
        case COLOR_SEVEN_CODE: // "7" Button
            index = 6;
            break;
        
        default:
            return FAIL_CODE;
    }

    Serial.println("Activating pixels to color...");
    color = &colors[index];

    return setArea(color);
}

// /**
//  * Set all pixels in an area to white color
//  */
// int activateArea(RgbColor color, Area *area)
// {
//     Serial.println("Setting " + area->name + " to white");
//     (*area).state = true;
//     // fadeAreaColor(index, BLACK_COLOR, WHITE_COLOR);
//     return setArea(area, WHITE_COLOR);
//     // return SUCCESS_CODE;
// }

// /**
//  * Sets all pixels in an area to black
//  */
// int deactivateArea(uint16_t index, Area *area)
// {
//     Serial.println("Setting " + area->name + " to black");
//     (*area).state = false;
//     // fadeAreaColor(index, WHITE_COLOR, BLACK_COLOR);
//     return setArea(area, BLACK_COLOR);
//     // return SUCCESS_CODE;
// }

/**
 * Set all pixels in an area to specific color
 */
int setArea(RgbColor *color)
{
    pixels.strip->ClearTo(*color);
    pixels.strip->Show();
    return SUCCESS_CODE;
}

// /**
//  * Animates the color fade using linear blend animation
//  */
// void blendAnimUpdate(const AnimationParam &param)
// {
//     RgbColor updatedColor = RgbColor::LinearBlend(
//         animStates[param.index].InitColor, 
//         animStates[param.index].FinColor, 
//         param.progress
//     );

//     areas[param.index].strip->ClearTo(updatedColor);
// }

// /**
//  * Starts the animation for color fade-in/fade-out
//  */
// void fadeAreaColor(uint16_t index, RgbColor initColor, RgbColor finColor)
// {
//     animStates[index] = {initColor, finColor};
//     animations.StartAnimation(index, ANIM_TIME, blendAnimUpdate);
// }