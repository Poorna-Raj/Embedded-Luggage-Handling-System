#pragma once

#include "Arduino.h"
#include "utils/IrSensor.h"

constexpr uint8_t BIN_SAMPLE_THRESHOLD = 5;

enum class BinManagerState
{
    IDLE,
    FULL,
    ERROR
};

enum class Bin
{
    BIN_1,
    BIN_2,
    BIN_3,
    NONE
};

struct BinManager
{
    IrSensor &bin1Ir;
    IrSensor &bin2Ir;
    IrSensor &bin3Ir;

    BinManagerState state;
    Bin fullBin;

    uint8_t bin1Samples;
    uint8_t bin2Samples;
    uint8_t bin3Samples;

    bool isErrorPrint;
    bool isFullPrint;

    bool initialized;
};

BinManager BinManager_Create(IrSensor &ir1, IrSensor &ir2, IrSensor &ir3);

void BinManager_Init(BinManager &bmn);
void BinManager_Update(BinManager &bmn);

BinManagerState BinManager_GetState(const BinManager &bmn);
void BinManager_OnCompleted(BinManager &bmn);
Bin BinManager_GetBin(const BinManager &bmn);