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
        .state = SystemState::WAITING,
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

    if (!WiFiManager_IsConnected(*sm.wifi))
        return;

    SortingManager_Update(*sm.sortMng);
    BinManager_Update(*sm.binMng);

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

void handleIdleState(SystemManager &sm);
void handleFullState(SystemManager &sm);
void handleWaitingState(SystemManager &sm);
void handleErrorState(SystemManager &sm);