#pragma once
#include <Arduino.h>

enum class GearMotorDirection
{
    FORWARD,
    BACKWARD
};

enum class GearMotorState
{
    STOPPED,
    RUNNING
};

struct GearMotor
{
    uint8_t pwmPin;   // Pin for PWM speed control
    uint8_t dirPin1;  // Pin for direction control 1 (IN1)
    uint8_t dirPin2;  // Pin for direction control 2 (IN2)
    bool hasDirPins;  // Flag for direction pins
    uint8_t speed;    // Motor speed (0-255 for PWM)
    GearMotorState state;
    bool initialized;
};

GearMotor GearMotor_Create(uint8_t pwmPin, uint8_t dirPin1, uint8_t dirPin2);
void GearMotor_Init(GearMotor &m);
void GearMotor_Update(GearMotor &m);

void GearMotor_Run(GearMotor &m, uint8_t speed, GearMotorDirection direction);
void GearMotor_Stop(GearMotor &m);

bool GearMotor_IsRunning(const GearMotor &m);
uint8_t GearMotor_GetSpeed(const GearMotor &m);
