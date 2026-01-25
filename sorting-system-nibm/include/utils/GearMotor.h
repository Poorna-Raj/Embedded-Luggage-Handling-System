#pragma once
#include <Arduino.h>

enum class GearMotorState
{
    STOPPED,
    RUNNING
};

struct GearMotor
{
    uint8_t pwmPin;
    uint8_t dirPin;
    bool hasDirPin;

    uint8_t speed;
    GearMotorState state;
    bool initialized;
};

GearMotor GearMotor_Create(uint8_t pwmPin);
GearMotor GearMotor_Create(uint8_t pwmPin, uint8_t dirPin);

void GearMotor_Init(GearMotor &m);
void GearMotor_Update(GearMotor &m);

void GearMotor_Run(GearMotor &m);
void GearMotor_Run(GearMotor &m, uint8_t speed);
void GearMotor_Stop(GearMotor &m);

bool GearMotor_IsRunning(const GearMotor &m);
uint8_t GearMotor_GetSpeed(const GearMotor &m);
