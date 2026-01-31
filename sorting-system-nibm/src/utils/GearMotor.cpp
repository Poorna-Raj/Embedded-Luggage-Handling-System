#include "utils/GearMotor.h"

GearMotor GearMotor_Create(uint8_t pwmPin)
{
    return GearMotor{
        .pwmPin = pwmPin,
        .dirPin = 0,
        .hasDirPin = false,
        .speed = 0,
        .state = GearMotorState::STOPPED,
        .initialized = false
    };
}

GearMotor GearMotor_Create(uint8_t pwmPin, uint8_t dirPin)
{
    return GearMotor{
        .pwmPin = pwmPin,
        .dirPin = dirPin,
        .hasDirPin = true,
        .speed = 0,
        .state = GearMotorState::STOPPED,
        .initialized = false
    };
}

void GearMotor_Init(GearMotor &m)
{
    if (m.initialized)
        return;

    pinMode(m.pwmPin, OUTPUT);

    if (m.hasDirPin)
    {
        pinMode(m.dirPin, OUTPUT);
        digitalWrite(m.dirPin, HIGH);  // Default direction HIGH (FORWARD)
    }

    analogWrite(m.pwmPin, 0);  // Initialize the motor to STOPPED
    m.state = GearMotorState::STOPPED;
    m.initialized = true;
}

void GearMotor_Update(GearMotor &m)
{
    if (!m.initialized)
        return;

    if (m.state == GearMotorState::RUNNING)
    {
        analogWrite(m.pwmPin, m.speed);  // Set speed if running
    }
    else
    {
        analogWrite(m.pwmPin, 0);  // Stop the motor if it's not running
    }
}

void GearMotor_Run(GearMotor &m, uint8_t speed, GearMotorDirection direction)
{
    m.speed = speed;
    m.state = GearMotorState::RUNNING;

    // Set motor direction
    if (m.hasDirPin)
    {
        if (direction == GearMotorDirection::FORWARD)
            digitalWrite(m.dirPin, HIGH);  // Set direction forward
        else
            digitalWrite(m.dirPin, LOW);  // Set direction backward
    }

    GearMotor_Update(m);  // Apply the changes to motor
}


void GearMotor_Stop(GearMotor &m)
{
    m.speed = 0;
    m.state = GearMotorState::STOPPED;
    GearMotor_Update(m);  // Stop the motor
}

bool GearMotor_IsRunning(const GearMotor &m)
{
    return m.state == GearMotorState::RUNNING;
}

uint8_t GearMotor_GetSpeed(const GearMotor &m)
{
    return m.speed;
}
