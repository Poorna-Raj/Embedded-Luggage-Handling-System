#pragma once
#include "comm/TcpManager.h"

enum class CommManagerState
{
    ACTIVE,
    BIN_1_FULL,
    BIN_2_FULL,
    BIN_3_FULL
};

struct CommManager
{
    TcpManager &tcpManager;
    CommManagerState state;
    bool isMessageSent;
    bool initialized;
};

void CommManager_SendMessage(CommManager &cm, const char *msg);
void CommManager_HandleIncomming(CommManager &cm);

void CommManager_Init(CommManager &cm);
void CommManager_Update(CommManager &cm);

void CommManager_NotifyBinFull(CommManager &cm, uint8_t binNumber);
CommManagerState CommManager_GetState(CommManager &cm);
void CommManager_SetState(CommManager &cm, CommManagerState state);