#include <Arduino.h>
#include "utils/IrSensor.h"

const uint8_t IR_PIN = 0;
IrSensor ir = IrSensor_Create(IR_PIN);

void setup()
{
    Serial.begin(9600);
    IrSensor_Init(ir);
    Serial.println("IR Sensor Manual Test Started!");
    Serial.println("Cover or unblock the sensor to trigger events...");
    pinMode(12,OUTPUT);
}

void loop()
{
    // Update the sensor reading
    IrSensor_Update(ir);

    // Check if a rising/falling event occurred
    if (IrSensor_IsEventTrue(ir))
    {
        Serial.println("IR Sensor EVENT triggered!");
    }
    digitalWrite(12, HIGH);
}