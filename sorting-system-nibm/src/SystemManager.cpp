#include "SystemManager.h"

void handleIdleState(SystemManager &sm);
void handleFullState(SystemManager &sm);
void handleWaitingState(SystemManager &sm);
void handleErrorState(SystemManager &sm);

SystemManager SystemManager_Create(WiFiManager *w, TcpManager *t, CommManager *cm, BinManager *b, SortingManager *sm, GearMotor &gm)
{
    return SystemManager{
        .wifi = w,
        .tcp = t,
        .comm = cm,
        .binMng = b,
        .sortMng = sm,
        .gm = gm,
        .state = SystemState::IDLE,
        .initialize = false,
    };
}

void SystemManager_Init(SystemManager &sm)
{
    if (sm.initialize)
        return;

    WiFiManager_Init(*sm.wifi);
    TcpManager_Init(*sm.tcp);
    CommManager_Init(*sm.comm);

    IrSensor_Init(sm.sortMng->bin1.ir);
    IrSensor_Init(sm.sortMng->bin2.ir);
    IrSensor_Init(sm.sortMng->bin3);

    SortingManager_Init(*sm.sortMng);
    BinManager_Init(*sm.binMng);

    GearMotor_Init(sm.gm);  // Ensure motor is initialized

    sm.initialize = true;
}

void SystemManager_Update(SystemManager &sm)
{
    IrSensor_Update(sm.sortMng->bin1.ir);
    IrSensor_Update(sm.sortMng->bin2.ir);
    IrSensor_Update(sm.sortMng->bin3);

    WiFiManager_Update(*sm.wifi);
    TcpManager_Update(*sm.tcp);
    CommManager_Update(*sm.comm);

    bool online = WiFiManager_IsConnected(*sm.wifi);

    SortingManager_Update(*sm.sortMng);
    BinManager_Update(*sm.binMng);
    GearMotor_Update(sm.gm);

    if (!online)
        return;

    switch (sm.state)
    {
    case SystemState::IDLE:
        handleIdleState(sm);
        break;
    case SystemState::FULL:
        handleFullState(sm);
        break;
    case SystemState::WAITING:
        handleWaitingState(sm);
        break;
    case SystemState::ERROR:
        handleErrorState(sm);
        break;
    }
}

void handleIdleState(SystemManager &sm)
{
    // Conveyor belt running forward
    GearMotor_Run(sm.gm, 128, GearMotorDirection::FORWARD);  // Run motor forward at speed 128

    if (BinManager_GetState(*sm.binMng) == BinManagerState::FULL)
    {
        sm.state = SystemState::FULL;
    }
}

void handleFullState(SystemManager &sm)
{
    GearMotor_Stop(sm.gm);  // Stop motor when the bin is full
    // Send the bin message
    Bin fullBin = BinManager_GetBin(*sm.binMng);

    if (fullBin == Bin::NONE)
        return;

    if (fullBin == Bin::BIN_1)
    {
        CommManager_NotifyBinFull(*sm.comm, 1);
        sm.state = SystemState::WAITING;
    }
    else if (fullBin == Bin::BIN_2)
    {
        CommManager_NotifyBinFull(*sm.comm, 2);
        sm.state = SystemState::WAITING;
    }
    else
    {
        CommManager_NotifyBinFull(*sm.comm, 3);
        sm.state = SystemState::WAITING;
    }
}

void handleWaitingState(SystemManager &sm)
{
    GearMotor_Stop(sm.gm);  // Stop motor while waiting
    if (CommManager_GetState(*sm.comm) == CommManagerState::ACTIVE)
    {
        sm.state = SystemState::IDLE;
    }
}

void handleErrorState(SystemManager &sm)
{
    GearMotor_Stop(sm.gm);  // Stop motor if there's an error
    Serial.println("Error at System Manager");
}
