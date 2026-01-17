#pragma once

#include <Arduino.h>

struct IrSensor
{
    const uint8_t gpio;

    bool current;
    bool previous;
    bool event;

    bool initialized;

    unsigned long lastEventTime;
    const unsigned long debounceMs;
};

IrSensor IrSensor_Create(const uint8_t gpio);

void IrSensor_Init(IrSensor &ir);
void IrSensor_Update(IrSensor &ir);

bool IrSensor_IsDetected(IrSensor &ir);
bool IrSensor_IsEventTrue(IrSensor &ir);