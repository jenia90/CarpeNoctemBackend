#include <Arduino.h>
#include <NeoPixelBus.h>
#include <IRremote.h>

#define SUCCESS_CODE 0
#define FAIL_CODE -1

const uint16_t AreaOnePixelCount = 5;
const uint8_t AreaOnePixelPin = 5;

// Uncomment for more strips
// const uint16_t AreaTwoPixelCount = 6;
// const uint8_t AreaTwoPixelPin = 5;
// const uint16_t AreaThreePixelCount = 7;
// const uint8_t AreaThreePixelPin = 5;
// const uint16_t AreaFourPixelCount = 8;
// const uint8_t AreaFourPixelPin = 5;
// const uint16_t AreaFivePixelCount = 9;
// const uint8_t AreaFivePixelPin = 5;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> stripAreaOne(AreaOnePixelCount, AreaOnePixelPin);
// NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> stripAreaTwo(AreaTwoPixelCount, AreaTwoPixelPin);
// NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> stripAreaThree(AreaThreePixelCount, AreaThreePixelPin);
// NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> stripAreaFour(AreaFourPixelCount, AreaFourPixelPin);
// NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> stripAreaFive(AreaFivePixelCount, AreaFivePixelPin);

// IR setup
const int IR_PIN = 12;
IRrecv recv(IR_PIN);
decode_results irData;

// Function declarations
int decodeRemoteCommands(unsigned long data);

void setup()
{
    // Initialize serial for debugging
    Serial.begin(9600);
    Serial.println("Initializing...");
    Serial.flush();

    Serial.println("Starting IR sensor...");
    recv.enableIRIn();
    Serial.println("IR enabled.");

    // Initialize led strips
    stripAreaOne.Begin();
    stripAreaOne.Show();
    // stripAreaTwo.Begin();
    // stripAreaTwo.Show();
    // stripAreaThree.Begin();
    // stripAreaThree.Show();
    // stripAreaFour.Begin();
    // stripAreaFour.Show();
    // stripAreaFive.Begin();
    // stripAreaFive.Show();

    Serial.println();
    Serial.println("Running...");
}

void loop()
{
    if(recv.decode(&irData))
    {
        // Skip if button pressed longer than needed.
        if(irData.value != 0xFFFFFFFF)
        {
            if(decodeRemoteCommands(irData.value) == FAIL_CODE)
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
int decodeRemoteCommands(unsigned long data)
{
    switch (data)
    {
        case 0xFD00FF: // Power Button
            Serial.println("Activating all areas...");
            return SUCCESS_CODE;

        case 0xFD08F7: // "1" Button
            Serial.println("Activating area 1...");
            return SUCCESS_CODE;

        case 0xFD8877: // "2" Button
            Serial.println("Activating area 2...");
            return SUCCESS_CODE;

        case 0xFD48B7: // "3" Button
            Serial.println("Activating area 3...");
            return SUCCESS_CODE;

        case 0xFD28D7: // "4" Button
            Serial.println("Activating area 4...");
            return SUCCESS_CODE;

        case 0xFDA857: // "5" Button
            Serial.println("Activating area 5...");
            return SUCCESS_CODE;
        
        default:
            return FAIL_CODE;
    }

    return FAIL_CODE;
}

int activateArea(unsigned short area)
{
    return SUCCESS_CODE;
}