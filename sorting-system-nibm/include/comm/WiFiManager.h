#pragma once
#include <ESP8266WiFi.h>

struct WiFiManager
{
    const char *ssid;
    const char *password;

    bool connected;
    bool initialized;
};

void WiFiManager_Init(WiFiManager &m);
void WiFiManager_Update(WiFiManager &m);

bool WiFiManager_IsConnected(const WiFiManager &m);
bool WiFiManager_IsInitialized(const WiFiManager &m);