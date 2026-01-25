#include <Arduino.h>
#include "comm/WiFiManager.h"
#include "comm/CommManager.h"

#include "BinManager.h"
#include "SortingManager.h"

#include "utils/GearMotor.h"

enum class SystemState
{
    IDLE,
    FULL,
    WAITING,
    ERROR
};

struct SystemManager
{
    WiFiManager *wifi;
    TcpManager *tcp;
    CommManager *comm;

    BinManager *binMng;
    SortingManager *sortMng;

    GearMotor &gm;

    SystemState state;
    bool initialize;
};

SystemManager SystemManager_Create(WiFiManager *w, TcpManager *t, CommManager *cm, BinManager *b, SortingManager *sm);
void SystemManager_Init(SystemManager &sm);
void SystemManager_Update(SystemManager &sm);