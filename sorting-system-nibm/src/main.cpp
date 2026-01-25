#include <Arduino.h>
#include "comm/WiFiManager.h"
#include "comm/TcpManager.h"
#include "comm/CommManager.h"

#include "SortingManager.h"
#include "BinManager.h"
#include "SystemManager.h"

#include "utils/GearMotor.h"

constexpr char *SSID = "SLT-Fiber-2.4G-0A6C";
constexpr char *PASSWORD = "873399703%";

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
// -------------------- Object Manager --------------------
ColorSensor cs = ColorSensor_Create(2, 3, 4);
IrSensor ir = IrSensor_Create(7);

ObjectManager objMng = ObjectManager_Create(ir, cs);

// -------------------- Servo Motors --------------------
constexpr uint8_t SERVO1_PIN = 9; // Left bin
constexpr uint8_t RESET1_ANGLE = 30;
constexpr uint8_t PUSH1_ANGLE = 120;

constexpr uint8_t SERVO2_PIN = 11; // Right bin
constexpr uint8_t RESET2_ANGLE = 30;
constexpr uint8_t PUSH2_ANGLE = 120;

ServoMotor redServo;
ServoMotor greenServo;

// -------------------- Bin IR sensors --------------------
IrSensor bin1Ir = IrSensor_Create(5);
IrSensor bin2Ir = IrSensor_Create(10);
IrSensor bin3Ir = IrSensor_Create(8);

// -------------------- Hardware structs --------------------
Hardware bin1 = {ir : bin1Ir, servo : &redServo};
Hardware bin2 = {ir : bin2Ir, servo : &greenServo};

// -------------------- Sorting Manager --------------------
SortingManager sorter = SortingManager_Create(bin1, bin2, bin3Ir, objMng);

// -------------------- Bin Manager ------------------------
BinManager binMng = BinManager_Create(bin1Ir, bin2Ir, bin3Ir);

// -------------------- Convayer Gear Motor ---------------
GearMotor motor = GearMotor_Create(12, 13);

// -------------------- System Manager ---------------------
SystemManager mng = SystemManager_Create(&wifi, &tcp, &comm, &binMng, &sorter);

void setup()
{
    Serial.begin(9600);
    SystemManager_Init(mng);
}

void loop()
{
    SystemManager_Update(mng);
}
