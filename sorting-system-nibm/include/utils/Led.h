#pragma once

#include <Arduino.h>

enum class LedState
{
    RUN,
    STOP,
    BLINK,
    ERROR
};

struct Led
{
    const uint8_t gpio;

    LedState state;
    unsigned long lastToggle;
    unsigned long interval;
    bool ledOn;
    bool initialized;
};

Led Led_Create(const uint8_t gpio);
void Led_Init(Led &led);
void Led_Update(Led &led);

void Led_SetState(Led &led, LedState state);