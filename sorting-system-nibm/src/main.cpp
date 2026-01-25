#include <Arduino.h>
#include "comm/WiFiManager.h"
#include "comm/TcpManager.h"
#include "comm/CommManager.h"

const char *SSID = "SLT-Fiber-2.4G-0A6C";
const char *PASSWORD = "873399703%";

WiFiManager wifi{
    .ssid = SSID,
    .password = PASSWORD,
    .connected = false,
    .initialized = false,
};

TcpManager tcp{
    .server = WiFiServer(8080),
    .client = WiFiClient(),
    .port = 8080,
};

CommManager comm{
    .tcpManager = tcp,
    .state = CommManagerState::ACTIVE,
    .isMessageSent = false,
    .initialized = false,
};

unsigned long lastTestTime = 0;
uint8_t testStep = 0;

void setup()
{
    Serial.begin(9600);
    delay(1000);

    Serial.println("=== COMM SYSTEM TEST START ===");

    WiFiManager_Init(wifi);
    TcpManager_Init(tcp);
    CommManager_Init(comm);
}

void loop()
{
    WiFiManager_Update(wifi);
    TcpManager_Update(tcp);
    CommManager_Update(comm);

    if (!WiFiManager_IsConnected(wifi))
        return;

    // Run tests every 5 seconds
    if (millis() - lastTestTime < 5000)
        return;

    lastTestTime = millis();

    switch (testStep)
    {
    case 0:
        Serial.println("[TEST] Trigger BIN 1 FULL");
        CommManager_NotifyBinFull(comm, 1);
        break;

    case 1:
        Serial.println("[TEST] Trigger BIN 2 FULL");
        CommManager_NotifyBinFull(comm, 2);
        break;

    case 2:
        Serial.println("[TEST] Trigger BIN 3 FULL");
        CommManager_NotifyBinFull(comm, 3);
        break;

    case 3:
        Serial.println("[TEST] All tests triggered. Waiting for DONE/FAILED responses.");
        break;
    }

    if (testStep < 3)
        testStep++;
}
