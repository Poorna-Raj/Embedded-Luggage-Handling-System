#pragma once

#include <Arduino.h>
#include <Servo.h>

struct ServoMotor
{
    uint8_t gpio;
    Servo servo;

    uint8_t resetAngle;
    uint8_t pushAngle;

    bool initialized;
};

// We pass by pointer to ensure we are modifying the original instance
void ServoMotor_Create(ServoMotor *sm, uint8_t pin, uint8_t reset, uint8_t push);
void ServoMotor_Init(ServoMotor *sm);
void ServoMotor_Trigger(ServoMotor *sm);
void ServoMotor_Reset(ServoMotor *sm);