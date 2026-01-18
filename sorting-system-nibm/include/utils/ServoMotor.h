#pragma once

#include <Arduino.h>
#include <Servo.h>

struct ServoMotor
{
    const uint8_t gpio;
    Servo servo;

    const uint8_t resetAngle;
    const uint8_t pushAngle;

    bool initialized;
};

ServoMotor ServoMotor_Create(const uint8_t pin, const uint8_t reset, const uint8_t push);

void ServoMotor_Init(ServoMotor &sm);

void ServoMotor_Trigger(ServoMotor &sm);
void ServoMotor_Reset(ServoMotor &sm);