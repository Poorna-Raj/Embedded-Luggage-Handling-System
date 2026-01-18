#include <Arduino.h>
#include "utils/ServoMotor.h"

// Adjust angles if needed
constexpr uint8_t SERVO_PIN = 9;
constexpr uint8_t RESET_ANGLE = 90;
constexpr uint8_t PUSH_ANGLE = 30;

ServoMotor servo = ServoMotor_Create(
    SERVO_PIN,
    RESET_ANGLE,
    PUSH_ANGLE);

void setup()
{
    Serial.begin(9600);
    Serial.println("Servo Motor Test Starting...");

    ServoMotor_Init(servo);
}

void loop()
{
    Serial.println("Triggering servo (PUSH)");
    ServoMotor_Trigger(servo);

    delay(1000);

    Serial.println("Resetting servo (HOME)");
    ServoMotor_Reset(servo);

    delay(2000);
}
