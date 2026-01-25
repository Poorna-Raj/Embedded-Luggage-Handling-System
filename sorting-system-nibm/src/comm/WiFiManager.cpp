#include "comm/WiFiManager.h"

void WiFiManager_Init(WiFiManager &m)
{
    if (m.initialized)
        return;

    WiFi.mode(WIFI_STA);
    WiFi.begin(m.ssid, m.password);
    m.initialized = true;
}

void WiFiManager_Update(WiFiManager &m)
{
    if (!m.initialized)
        return;

    if (WiFi.status() == WL_CONNECTED)
    {
        if (!m.connected)
        {
            m.connected = true;
            Serial.print("WiFi Initialized: ");
            Serial.println(WiFi.localIP());
        }
    }
    else
    {
        m.connected = false;
    }
}

bool WiFiManager_IsConnected(const WiFiManager &m)
{
    return m.connected;
}

bool WiFiManager_IsInitialized(const WiFiManager &m)
{
    return m.initialized;
}