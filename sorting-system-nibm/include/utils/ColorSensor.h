#pragma once

#include <Arduino.h>

enum class Color
{
    RED,
    BLUE,
    GREEN,
    NONE
};

struct ColorSensor
{
    const uint8_t s2Gpio;
    const uint8_t s3Gpio;
    const uint8_t outGpio;
    int samplesToAverage;

    bool initialized;
};

ColorSensor ColorSensor_Create(const uint8_t s2, const uint8_t s3, const uint8_t out);
void ColorSensor_Init(ColorSensor &cs);

Color ColorSensor_DetectColor(ColorSensor &cs);