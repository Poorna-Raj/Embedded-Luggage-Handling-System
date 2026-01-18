#include <Arduino.h>
#include "ObjectManager.h"
#include "SortingManager.h"
#include "utils/ServoMotor.h"

// -------------------- Object Manager --------------------
ColorSensor cs = ColorSensor_Create(2, 3, 4); // Pins for color sensor
IrSensor ir = IrSensor_Create(7);             // Object detection IR

ObjectManager objMng = ObjectManager_Create(ir, cs);

// -------------------- Servo Motors --------------------
constexpr uint8_t SERVO1_PIN = 5;  // Left bin
constexpr uint8_t SERVO2_PIN = 11; // Right bin

ServoMotor servo1 = ServoMotor_Create(SERVO1_PIN, 90, 30); // reset=90, push=30
ServoMotor servo2 = ServoMotor_Create(SERVO2_PIN, 90, 30);

// -------------------- Bin IR sensors --------------------
IrSensor bin1Ir = IrSensor_Create(9);
IrSensor bin2Ir = IrSensor_Create(10);
IrSensor bin3Ir = IrSensor_Create(8); // End bin IR

// -------------------- Hardware structs --------------------
Hardware bin1 = {ir : bin1Ir, servo : servo1};
Hardware bin2 = {ir : bin2Ir, servo : servo2};

// -------------------- Sorting Manager --------------------
SortingManager sorter = SortingManager_Create(bin1, bin2, bin3Ir, objMng);

void setup()
{
    Serial.begin(9600);
    Serial.println("Sorting Manager Test Start...");

    ObjectManager_Init(objMng);
    SortingManager_Init(sorter);

    // Optional: simulate the servo IR triggers with a delay for testing
    // Real hardware would replace these with actual IR signals
}

void loop()
{
    // -------------------- Update Sorting Manager --------------------
    SortingManager_Update(sorter);

    if (Serial.available())
    {
        char ch = Serial.read();
        if (ch == '1')
        {
            sorter.bin1.ir.event = true;
        }
        else if (ch == '2')
        {
            sorter.bin2.ir.event = true;
        }
        else if (ch == '3')
        {
            sorter.bin3.event = true;
        }
    }
}
