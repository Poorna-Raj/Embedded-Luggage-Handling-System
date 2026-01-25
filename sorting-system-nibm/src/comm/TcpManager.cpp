#include "comm/TcpManager.h"

void TcpManager_Init(TcpManager &tcp)
{
    tcp.server.begin(tcp.port);
    tcp.server.setNoDelay(true);
    Serial.println("TCP Server Initialized");
}

void TcpManager_Update(TcpManager &tcp)
{
    if (!tcp.client || !tcp.client.connected())
    {
        WiFiClient newClient = tcp.server.accept();

        if (newClient)
        {
            if (tcp.client)
                tcp.client.stop();

            tcp.client = newClient;
            Serial.println("Client Connected");
        }
    }

    if (tcp.client && !tcp.client.connected())
    {
        tcp.client.stop();
        Serial.println("Client Disconnected");
    }
}

WiFiClient &TcpManager_GetClient(TcpManager &tcp)
{
    return tcp.client;
}

bool TcpManager_IsClientConnected(TcpManager &tcp)
{
    return (tcp.client && tcp.client.connected());
}