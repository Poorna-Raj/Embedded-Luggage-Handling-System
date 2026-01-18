#pragma once

#include "utils/IrSensor.h"
#include "utils/ServoMotor.h"
// #include "utils/Led.h"
#include "ObjectManager.h"

enum class SortState
{
    IDLE,
    WAITING,
    ACTUATING,
    ERROR,
};

struct Hardware
{
    IrSensor &ir;
    ServoMotor &servo;
};

struct SortingManager
{
    Hardware &bin1;
    Hardware &bin2;
    IrSensor &bin3;

    SortState state;

    ObjectManager &mng;
    Color color;

    unsigned long bin1Count;
    unsigned long bin2Count;
    unsigned long bin3Count;

    bool errorPrint;

    bool initialized;
};

SortingManager SortingManager_Create(Hardware &bin1, Hardware &bin2, IrSensor &bin3, ObjectManager &obj);

void SortingManager_Init(SortingManager &mng);
void SortingManager_Update(SortingManager &mng);