#include <Arduino.h>

#include "ObjectManager.h"

ColorSensor cs = ColorSensor_Create(2, 3, 4);
IrSensor ir = IrSensor_Create(7);

ObjectManager objMng = ObjectManager_Create(ir, cs);

void setup()
{
    Serial.begin(9600);
    ObjectManager_Init(objMng);
}

void loop()
{
    ObjectManager_Update(objMng);

    if (ObjectManager_IsEventTrue(objMng))
    {
        Color cl = ObjectManager_GetColor(objMng);
        Serial.println(cl == Color::RED ? "RED" : cl == Color::BLUE ? "BLUE"
                                              : cl == Color::GREEN  ? "GREEN"
                                                                    : "404");
    }
}