#include "utils/GearMotor.h"

GearMotor GearMotor_Create(uint8_t pwmPin, uint8_t dirPin1, uint8_t dirPin2)
{
    return GearMotor{
        .pwmPin = pwmPin,
        .dirPin1 = dirPin1,
        .dirPin2 = dirPin2,
        .hasDirPins = true,
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
    pinMode(m.dirPin1, OUTPUT);
    pinMode(m.dirPin2, OUTPUT);

    // Set the direction pins to default (e.g., forward)
    digitalWrite(m.dirPin1, HIGH);  // Default direction forward
    digitalWrite(m.dirPin2, LOW);   // Default direction forward

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
    if (m.hasDirPins)
    {
        if (direction == GearMotorDirection::FORWARD)
        {
            digitalWrite(m.dirPin1, HIGH);
            digitalWrite(m.dirPin2, LOW);
        }
        else
        {
            digitalWrite(m.dirPin1, LOW);
            digitalWrite(m.dirPin2, HIGH);
        }
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
