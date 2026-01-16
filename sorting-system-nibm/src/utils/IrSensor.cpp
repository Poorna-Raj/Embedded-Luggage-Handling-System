#include "utils/IrSensor.h"

IrSensor IrSensor_Create(const uint8_t gpio)
{
    return IrSensor{
        .gpio = gpio,
        .current = false,
        .previous = false,
        .event = false,
        .initialized = false,
    };
}

void IrSensor_Init(IrSensor &ir)
{
    if (ir.initialized)
        return;

    pinMode(ir.gpio, INPUT_PULLUP);
    ir.initialized = true;
}

void IrSensor_Update(IrSensor &ir)
{
    if (!ir.initialized)
        return;

    ir.current = digitalRead(ir.gpio);

    if (!ir.current && ir.previous)
    {
        ir.event = true;
    }

    ir.previous = ir.current;
}

bool IrSensor_IsDetected(IrSensor &ir)
{
    return !ir.current;
}

bool IrSensor_IsEventTrue(IrSensor &ir)
{
    if (ir.event)
    {
        ir.event = false;
        return true;
    }
    return false;
}