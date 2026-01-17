#include "ObjectManager.h"

void handleIdleState(ObjectManager &omn);
void handleDetectedState(ObjectManager &omn);
void handleFailedState(ObjectManager &omn);

ObjectManager ObjectManager_Create(IrSensor &ir, ColorSensor &cs)
{
    return ObjectManager{
        .ir = ir,
        .cs = cs,
        .state = ObjManagerState::IDLE,
        .color = Color::NONE,
        .preColor = Color::NONE,
        .initialized = false,
        .detectedPrinted = false,
        .failedPrinted = false,
    };
}

void ObjectManager_Init(ObjectManager &omn)
{
    if (omn.initialized)
        return;

    IrSensor_Init(omn.ir);
    ColorSensor_Init(omn.cs);

    omn.initialized = true;
}

void ObjectManager_Update(ObjectManager &omn)
{
    if (!omn.initialized)
        return;

    IrSensor_Update(omn.ir);

    switch (omn.state)
    {
    case ObjManagerState::IDLE:
        handleIdleState(omn);
        break;
    case ObjManagerState::DETECTED:
        handleDetectedState(omn);
        break;
    case ObjManagerState::FAILED:
        handleFailedState(omn);
        break;
    default:
        omn.state = ObjManagerState::FAILED;
        break;
    }
}

void handleIdleState(ObjectManager &omn)
{
    if (IrSensor_IsEventTrue(omn.ir))
    {
        Serial.println("IR detected");
        Color color = ColorSensor_DetectColor(omn.cs);
        if (color != Color::NONE)
        {
            omn.color = color;
            omn.state = ObjManagerState::DETECTED;
        }
        else
        {
            omn.state = ObjManagerState::FAILED;
        }
    }
}

void handleDetectedState(ObjectManager &omn)
{
    if (!omn.detectedPrinted)
    {
        Serial.print("Color detected by Object Manager: ");
        Serial.println(omn.color == Color::RED ? "RED" : omn.color == Color::BLUE ? "BLUE"
                                                     : omn.color == Color::GREEN  ? "GREEN"
                                                                                  : "404");
        omn.detectedPrinted = true;
    }
}

void handleFailedState(ObjectManager &omn)
{
    if (!omn.failedPrinted)
    {
        Serial.println("Something went wrong in Object Manager");
        omn.failedPrinted = true;
    }
}

bool ObjectManager_IsEventTrue(ObjectManager &omn)
{
    if (omn.state == ObjManagerState::DETECTED && omn.color != Color::NONE)
    {
        omn.preColor = omn.color;
        omn.color = Color::NONE;
        omn.state = ObjManagerState::IDLE;

        omn.detectedPrinted = false;
        omn.failedPrinted = false;

        return true;
    }
    return false;
}

Color ObjectManager_GetColor(const ObjectManager &omn)
{
    return omn.preColor;
}