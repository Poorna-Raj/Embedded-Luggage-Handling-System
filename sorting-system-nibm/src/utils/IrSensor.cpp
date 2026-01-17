#include "utils/IrSensor.h"

IrSensor IrSensor_Create(const uint8_t gpio)
{
    return IrSensor{
        .gpio = gpio,
        .current = false,
        .previous = false,
        .event = false,
        .initialized = false,
        .busy = false,
        .lastEventTime = 0,
        .debounceMs = 500,
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

    if (!ir.event)
    {
        if (!ir.current && ir.previous && (millis() - ir.lastEventTime > ir.debounceMs))
        {
            ir.event = true;
            ir.lastEventTime = millis();
        }
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