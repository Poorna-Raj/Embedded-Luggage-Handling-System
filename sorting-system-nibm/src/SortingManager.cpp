#include "SortingManager.h"

void handleIdleState(SortingManager &mng);
void handleWaitingState(SortingManager &mng);
void handleAcutatingState(SortingManager &mng);
void handleErrorState(SortingManager &mng);

SortingManager SortingManager_Create(Hardware &bin1, Hardware &bin2, IrSensor &bin3, ObjectManager &obj)
{
    return SortingManager{
        .bin1 = bin1,
        .bin2 = bin2,
        .bin3 = bin3,
        .state = SortState::IDLE,
        .mng = obj,
        .color = Color::NONE,
        .bin1Count = 0,
        .bin2Count = 0,
        .bin3Count = 0,
        .errorPrint = false,
        .initialized = false,
    };
}

void SortingManager_Init(SortingManager &mng)
{
    if (mng.initialized)
        return;

    ObjectManager_Init(mng.mng);

    IrSensor_Init(mng.bin1.ir);
    IrSensor_Init(mng.bin2.ir);
    IrSensor_Init(mng.bin3);

    ServoMotor_Init(mng.bin1.servo);
    ServoMotor_Init(mng.bin2.servo);

    mng.initialized = true;
}

void SortingManager_Update(SortingManager &mng)
{
    if (!mng.initialized)
        return;

    ObjectManager_Update(mng.mng);
    IrSensor_Update(mng.bin1.ir);
    IrSensor_Update(mng.bin2.ir);
    IrSensor_Update(mng.bin3);

    switch (mng.state)
    {
    case SortState::IDLE:
        handleIdleState(mng);
        break;
    case SortState::WAITING:
        handleWaitingState(mng);
        break;
    case SortState::ACTUATING:
        handleAcutatingState(mng);
        break;
    case SortState::ERROR:
        handleErrorState(mng);
        break;

    default:
        mng.state = SortState::ERROR;
        break;
    }
}

void handleIdleState(SortingManager &mng)
{
    if (ObjectManager_IsEventTrue(mng.mng))
    {
        mng.color = ObjectManager_GetColor(mng.mng);
        Serial.println("Color Detected");

        if (mng.color == Color::NONE)
        {
            Serial.println("Invalid Color at Sorting Manager");
            mng.state = SortState::ERROR;
        }
        else
        {
            mng.state = SortState::WAITING;
        }
    }
}

void handleWaitingState(SortingManager &mng)
{
    Serial.println("In waiting state");
    if (mng.color == Color::NONE)
    {
        Serial.println("Invalid Color at Sorting Manager");
        mng.state = SortState::ERROR;
        return;
    }

    if (mng.color == Color::RED)
    {
        Serial.println("Trigger red");
        ServoMotor_Trigger(mng.bin1.servo);
    }
    else if (mng.color == Color::BLUE)
    {
        Serial.println("Trigger blue");
        ServoMotor_Trigger(mng.bin2.servo);
    }

    mng.state = SortState::ACTUATING;
}
void handleAcutatingState(SortingManager &mng)
{
    if (mng.color == Color::RED && IrSensor_IsEventTrue(mng.bin1.ir))
    {
        Serial.println("Got for red bin IR");
        ServoMotor_Reset(mng.bin1.servo);
        mng.color = Color::NONE;
        mng.bin1Count++;
        mng.state = SortState::IDLE;
    }
    else if (mng.color == Color::BLUE && IrSensor_IsEventTrue(mng.bin2.ir))
    {
        Serial.println("Got for blue bin IR");
        ServoMotor_Reset(mng.bin2.servo);
        mng.color = Color::NONE;
        mng.bin2Count++;
        mng.state = SortState::IDLE;
    }
    else if (mng.color == Color::GREEN && IrSensor_IsEventTrue(mng.bin3))
    {
        Serial.println("Got for green bin IR");
        mng.color = Color::NONE;
        mng.bin3Count++;
        mng.state = SortState::IDLE;
    }
}
void handleErrorState(SortingManager &mng)
{
    if (!mng.errorPrint)
    {
        Serial.println("Something went wrong at Sorting Manager :: ERROR");
        mng.errorPrint = true;
    }
}