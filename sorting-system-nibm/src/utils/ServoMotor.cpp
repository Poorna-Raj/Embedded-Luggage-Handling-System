#include "utils/ServoMotor.h"

ServoMotor ServoMotor_Create(const uint8_t pin, const uint8_t reset, const uint8_t push)
{
    return ServoMotor{
        .gpio = pin,
        .servo = Servo(),
        .resetAngle = reset,
        .pushAngle = push,
        .initialized = false,
    };
}

void ServoMotor_Init(ServoMotor &sm)
{
    if (sm.initialized)
        return;

    sm.servo.attach(sm.gpio);
    sm.servo.write(sm.resetAngle);

    sm.initialized = true;
}

void ServoMotor_Trigger(ServoMotor &sm)
{
    if (!sm.initialized)
        return;

    sm.servo.write(sm.pushAngle);
}

void ServoMotor_Reset(ServoMotor &sm)
{
    if (!sm.initialized)
        return;

    sm.servo.write(sm.resetAngle);
}