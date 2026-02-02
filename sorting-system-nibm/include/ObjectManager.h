#pragma once

#include <Arduino.h>
#include "utils/IrSensor.h"
#include "utils/ColorSensor.h"

enum class ObjManagerState
{
    IDLE,
    DETECTED,
    FAILED
};

struct ObjectManager
{
    IrSensor &ir;
    ColorSensor &cs;

    ObjManagerState state;
    Color color;
    Color preColor;

    bool initialized;
    bool eventConsumed;
};

ObjectManager ObjectManager_Create(IrSensor &ir, ColorSensor &cs);

void ObjectManager_Init(ObjectManager &omn);
void ObjectManager_Update(ObjectManager &omn);

bool ObjectManager_IsEventTrue(ObjectManager &omn);
Color ObjectManager_GetColor(const ObjectManager &omn);
