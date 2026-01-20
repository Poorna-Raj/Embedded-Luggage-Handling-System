#include "utils/ServoMotor.h"

void ServoMotor_Create(ServoMotor *sm, uint8_t pin, uint8_t reset, uint8_t push)
{
    sm->gpio = pin;
    sm->resetAngle = reset;
    sm->pushAngle = push;
    sm->initialized = false;
}

void ServoMotor_Init(ServoMotor *sm)
{
    if (sm->initialized)
        return;

    sm->servo.attach(sm->gpio);
    sm->servo.write(sm->resetAngle);
    sm->initialized = true;
}

void ServoMotor_Trigger(ServoMotor *sm)
{
    if (sm->initialized)
    {
        sm->servo.write(sm->pushAngle);
    }
}

void ServoMotor_Reset(ServoMotor *sm)
{
    if (sm->initialized)
    {
        sm->servo.write(sm->resetAngle);
    }
}