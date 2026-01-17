#include <Arduino.h>

#include "utils/ColorSensor.h"

ColorSensor cs = ColorSensor_Create(2, 3, 4);

void setup()
{
    Serial.begin(9600);
    ColorSensor_Init(cs);
}

void loop()
{
    if (Serial.available())
    {
        char ch = Serial.read();
        if (ch == 'r') // for example, type 'r' to trigger
        {
            Color c = ColorSensor_DetectColor(cs);
            Serial.println(c == Color::RED ? "RED" : c == Color::GREEN ? "GREEN"
                                                 : c == Color::BLUE    ? "BLUE"
                                                                       : "NONE");
        }
    }
}
