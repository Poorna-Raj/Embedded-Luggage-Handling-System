#include "comm/CommManager.h"

void CommManager_Init(CommManager &cm)
{
    if (cm.initialized)
        return;

    cm.state = CommManagerState::ACTIVE;
    cm.isMessageSent = false;
    cm.initialized = true;
}

void CommManager_NotifyBinFull(CommManager &cm, uint8_t binNumber)
{
    if (cm.state != CommManagerState::ACTIVE)
        return;

    switch (binNumber)
    {
    case 1:
        cm.state = CommManagerState::BIN_1_FULL;
        break;
    case 2:
        cm.state = CommManagerState::BIN_2_FULL;
        break;
    case 3:
        cm.state = CommManagerState::BIN_3_FULL;
        break;
    default:
        break;
    }
    cm.isMessageSent = false;
}

void CommManager_Update(CommManager &cm)
{
    if (!cm.initialized)
        return;

    if (!cm.isMessageSent && cm.state != CommManagerState::ACTIVE && TcpManager_IsClientConnected(cm.tcpManager))
    {
        switch (cm.state)
        {
        case CommManagerState::BIN_1_FULL:
            CommManager_SendMessage(cm, "BIN_1_FULL");
            break;
        case CommManagerState::BIN_2_FULL:
            CommManager_SendMessage(cm, "BIN_2_FULL");
            break;
        case CommManagerState::BIN_3_FULL:
            CommManager_SendMessage(cm, "BIN_3_FULL");
            break;
        default:
            break;
        }
        cm.isMessageSent = true;
    }

    CommManager_HandleIncomming(cm);
}

void CommManager_SendMessage(CommManager &cm, const char *msg)
{
    WiFiClient &client = TcpManager_GetClient(cm.tcpManager);
    if (client && client.connected())
    {
        client.print(msg);
        client.print("\n");
        Serial.print("Sent: ");
        Serial.println(msg);
    }
}

void CommManager_HandleIncomming(CommManager &cm)
{
    WiFiClient &client = TcpManager_GetClient(cm.tcpManager);
    if (!client || !client.connected())
        return;

    static char buffer[32];
    static uint8_t index = 0;

    while (client.available())
    {
        char c = client.read();
        if (c == '\r')
            continue;

        if (c == '\n')
        {
            buffer[index] = '\0';
            index = 0;

            if (strcmp(buffer, "DONE") == 0 || strcmp(buffer, "FAILED") == 0)
            {
                Serial.print("Received: ");
                Serial.println(buffer);
                cm.state = CommManagerState::ACTIVE;
                cm.isMessageSent = false;
            }
        }
        else if (index < sizeof(buffer) - 1)
        {
            buffer[index++] = c;
        }
    }
}

CommManagerState CommManager_GetState(CommManager &cm)
{
    return cm.state;
}

void CommManager_SetState(CommManager &cm, CommManagerState state)
{
    if (cm.state != state)
    {
        cm.state = state;
    }
}