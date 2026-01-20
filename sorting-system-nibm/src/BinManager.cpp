#include "BinManager.h"

void handleBinManagerError(BinManager &bmn);
void handleBinManagerFull(BinManager &bmn);
void handleBinManagerIdle(BinManager &bmn);

BinManager BinManager_Create(IrSensor &ir1, IrSensor &ir2, IrSensor &ir3)
{
    return BinManager{
        .bin1Ir = ir1,
        .bin2Ir = ir2,
        .bin3Ir = ir3,
        .state = BinManagerState::IDLE,
        .fullBin = Bin::NONE,
        .bin1Samples = 0,
        .bin2Samples = 0,
        .bin3Samples = 0,
        .isErrorPrint = false,
        .isFullPrint = false,
        .initialized = false,
    };
};

void BinManager_Init(BinManager &bmn)
{
    if (bmn.initialized)
        return;

    IrSensor_Init(bmn.bin1Ir);
    IrSensor_Init(bmn.bin2Ir);
    IrSensor_Init(bmn.bin3Ir);

    bmn.initialized = true;
}

void BinManager_Update(BinManager &bmn)
{
    if (!bmn.initialized)
        return;

    switch (bmn.state)
    {
    case BinManagerState::IDLE:
        handleBinManagerIdle(bmn);
        break;
    case BinManagerState::FULL:
        handleBinManagerFull(bmn);
        break;
    case BinManagerState::ERROR:
        handleBinManagerError(bmn);
        break;

    default:
        break;
    }
}

void handleBinManagerError(BinManager &bmn)
{
    if (!bmn.isErrorPrint)
    {
        Serial.println("Something went wrong in Bin Manager");
        bmn.isErrorPrint = true;
    }
}

void handleBinManagerFull(BinManager &bmn)
{
    if (!bmn.isFullPrint)
    {
        Serial.println("Bin is Full");
        bmn.isFullPrint = true;
    }
}

void handleBinManagerIdle(BinManager &bmn)
{
    bmn.isErrorPrint = false;
    bmn.isFullPrint = false;

    IrSensor_Update(bmn.bin1Ir);
    IrSensor_Update(bmn.bin2Ir);
    IrSensor_Update(bmn.bin3Ir);

    // BIN 1
    if (IrSensor_IsDetected(bmn.bin1Ir))
        bmn.bin1Samples++;
    else
        bmn.bin1Samples = 0;

    if (bmn.bin1Samples >= BIN_SAMPLE_THRESHOLD)
    {
        bmn.state = BinManagerState::FULL;
        bmn.fullBin = Bin::BIN_1;
        return;
    }

    // BIN 2
    if (IrSensor_IsDetected(bmn.bin2Ir))
        bmn.bin2Samples++;
    else
        bmn.bin2Samples = 0;

    if (bmn.bin2Samples >= BIN_SAMPLE_THRESHOLD)
    {
        bmn.state = BinManagerState::FULL;
        bmn.fullBin = Bin::BIN_2;
        return;
    }

    // BIN 3
    if (IrSensor_IsDetected(bmn.bin3Ir))
        bmn.bin3Samples++;
    else
        bmn.bin3Samples = 0;

    if (bmn.bin3Samples >= BIN_SAMPLE_THRESHOLD)
    {
        bmn.state = BinManagerState::FULL;
        bmn.fullBin = Bin::BIN_3;
        return;
    }
}

BinManagerState BinManager_GetState(const BinManager &bmn)
{
    return bmn.state;
}

void BinManager_OnCompleted(BinManager &bmn)
{
    bmn.fullBin = Bin::NONE;
    bmn.state = BinManagerState::IDLE;

    bmn.bin1Samples = 0;
    bmn.bin2Samples = 0;
    bmn.bin3Samples = 0;
}

Bin BinManager_GetBin(const BinManager &bmn)
{
    return bmn.fullBin;
}