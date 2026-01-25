#pragma once

#include <ESP8266WiFi.h>

struct TcpManager
{
    WiFiServer server;
    WiFiClient client;
    uint16_t port;
};

void TcpManager_Init(TcpManager &tcp);
void TcpManager_Update(TcpManager &tcp);

WiFiClient &TcpManager_GetClient(TcpManager &tcp);
bool TcpManager_IsClientConnected(TcpManager &tcp);
