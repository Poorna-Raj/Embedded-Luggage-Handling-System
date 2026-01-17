#include "utils/ColorSensor.h"

ColorSensor ColorSensor_Create(const uint8_t s2, const uint8_t s3, const uint8_t out)
{
    return ColorSensor{
        .s2Gpio = s2,
        .s3Gpio = s3,
        .outGpio = out,
        .samplesToAverage = 5,
        .initialized = false,
    };
}

void ColorSensor_Init(ColorSensor &cs)
{
    if (cs.initialized)
        return;

    pinMode(cs.s2Gpio, OUTPUT);
    pinMode(cs.s3Gpio, OUTPUT);
    pinMode(cs.outGpio, INPUT);

    digitalWrite(cs.s2Gpio, LOW);
    digitalWrite(cs.s3Gpio, LOW);

    cs.initialized = true;
}

int ColorSensor_ReadFreq(ColorSensor &cs, Color color)
{
    switch (color)
    {
    case Color::RED:
        digitalWrite(cs.s2Gpio, LOW);
        digitalWrite(cs.s3Gpio, LOW);
        break;
    case Color::GREEN:
        digitalWrite(cs.s2Gpio, HIGH);
        digitalWrite(cs.s3Gpio, HIGH);
        break;
    case Color::BLUE:
        digitalWrite(cs.s2Gpio, LOW);
        digitalWrite(cs.s3Gpio, HIGH);
        break;
    default:
        return -1;
    }

    return pulseIn(cs.outGpio, LOW, 1000);
}

Color ColorSensor_DetectColor(ColorSensor &cs)
{
    if (!cs.initialized)
        return Color::NONE;

    int redSum = 0, greenSum = 0, blueSum = 0;

    for (int i = 0; i < cs.samplesToAverage; i++)
    {
        redSum += ColorSensor_ReadFreq(cs, Color::RED);
        greenSum += ColorSensor_ReadFreq(cs, Color::GREEN);
        blueSum += ColorSensor_ReadFreq(cs, Color::BLUE);
    }

    int redAvg = redSum / cs.samplesToAverage;
    int greenAvg = greenSum / cs.samplesToAverage;
    int blueAvg = blueSum / cs.samplesToAverage;

    if (redAvg < greenAvg && redAvg < blueAvg)
        return Color::RED;
    if (greenAvg < redAvg && greenAvg < blueAvg)
        return Color::GREEN;
    if (blueAvg < redAvg && blueAvg < greenAvg)
        return Color::BLUE;

    return Color::NONE;
}