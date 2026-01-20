#include <Arduino.h>
#include "ObjectManager.h"
#include "SortingManager.h"
#include "utils/ServoMotor.h"

// -------------------- Object Manager --------------------
ColorSensor cs = ColorSensor_Create(2, 3, 4); // Pins for color sensor
IrSensor ir = IrSensor_Create(7);             // Object detection IR

ObjectManager objMng = ObjectManager_Create(ir, cs);

// -------------------- Servo Motors --------------------
constexpr uint8_t SERVO1_PIN = 9; // Left bin
constexpr uint8_t RESET1_ANGLE = 30;
constexpr uint8_t PUSH1_ANGLE = 120;

constexpr uint8_t SERVO2_PIN = 11; // Right bin
constexpr uint8_t RESET2_ANGLE = 30;
constexpr uint8_t PUSH2_ANGLE = 120;

ServoMotor redServo;
ServoMotor greenServo;

// -------------------- Bin IR sensors --------------------
IrSensor bin1Ir = IrSensor_Create(5);
IrSensor bin2Ir = IrSensor_Create(10);
IrSensor bin3Ir = IrSensor_Create(8); // End bin IR

// -------------------- Hardware structs --------------------
Hardware bin1 = {ir : bin1Ir, servo : &redServo};
Hardware bin2 = {ir : bin2Ir, servo : &greenServo};

// -------------------- Sorting Manager --------------------
SortingManager sorter = SortingManager_Create(bin1, bin2, bin3Ir, objMng);

void setup()
{
    Serial.begin(9600);
    Serial.println("Sorting Manager Test Start...");

    ServoMotor_Create(&redServo, SERVO1_PIN, RESET1_ANGLE, PUSH1_ANGLE);
    ServoMotor_Create(&greenServo, SERVO2_PIN, RESET2_ANGLE, PUSH2_ANGLE);

    ObjectManager_Init(objMng);
    SortingManager_Init(sorter);
}

void loop()
{
    // -------------------- Update Sorting Manager --------------------
    SortingManager_Update(sorter);
}