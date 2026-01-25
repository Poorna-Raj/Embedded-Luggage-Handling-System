#include "utils/GearMotor.h"

GearMotor GearMotor_Create(uint8_t pwmPin)
{
    return GearMotor{
        .pwmPin = pwmPin,
        .dirPin = 0,
        .hasDirPin = false,
        .speed = 0,
        .state = GearMotorState::STOPPED,
        .initialized = false};
}

GearMotor GearMotor_Create(uint8_t pwmPin, uint8_t dirPin)
{
    return GearMotor{
        .pwmPin = pwmPin,
        .dirPin = dirPin,
        .hasDirPin = true,
        .speed = 0,
        .state = GearMotorState::STOPPED,
        .initialized = false};
}

void GearMotor_Init(GearMotor &m)
{
    if (m.initialized)
        return;

    pinMode(m.pwmPin, OUTPUT);

    if (m.hasDirPin)
    {
        pinMode(m.dirPin, OUTPUT);
        digitalWrite(m.dirPin, HIGH);
    }

    analogWrite(m.pwmPin, 0);
    m.state = GearMotorState::STOPPED;
    m.initialized = true;
}

void GearMotor_Update(GearMotor &m)
{
    if (!m.initialized)
        return;

    if (m.state == GearMotorState::RUNNING)
    {
        analogWrite(m.pwmPin, m.speed);
    }
    else
    {
        analogWrite(m.pwmPin, 0);
    }
}

void GearMotor_Run(GearMotor &m)
{
    GearMotor_Run(m, 255);
}

void GearMotor_Run(GearMotor &m, uint8_t speed)
{
    m.speed = speed;
    m.state = GearMotorState::RUNNING;
}

void GearMotor_Stop(GearMotor &m)
{
    m.speed = 0;
    m.state = GearMotorState::STOPPED;
}

bool GearMotor_IsRunning(const GearMotor &m)
{
    return m.state == GearMotorState::RUNNING;
}

uint8_t GearMotor_GetSpeed(const GearMotor &m)
{
    return m.speed;
}
