#include <Arduino.h>
#include "BinManager.h"

IrSensor bin1Ir = IrSensor_Create(4);
IrSensor bin2Ir = IrSensor_Create(5);
IrSensor bin3Ir = IrSensor_Create(14);

BinManager binMng = BinManager_Create(bin1Ir, bin2Ir, bin3Ir);

void setup()
{
    Serial.begin(9600);
    Serial.println("Sorting Manager Test Start...");

    BinManager_Init(binMng);
}

void loop()
{
    BinManager_Update(binMng);
    if (BinManager_GetState(binMng) == BinManagerState::FULL)
    {
        Serial.println(BinManager_GetBin(binMng) == Bin::BIN_1 ? "1" : BinManager_GetBin(binMng) == Bin::BIN_2 ? "2"
                                                                   : BinManager_GetBin(binMng) == Bin::BIN_3   ? "3"
                                                                                                               : "NONE");
        if (Serial.available())
        {
            char ch = Serial.read();
            if (ch == 'o')
            {
                BinManager_OnCompleted(binMng);
            }
        }
    }
}