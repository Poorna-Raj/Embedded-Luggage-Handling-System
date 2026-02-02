# Led Test

```cpp
#include <Arduino.h>
#include "utils/Led.h"

// --- Configure your LED pin ---
const uint8_t LED_PIN = 7;
Led led = Led_Create(LED_PIN);

// --- State cycle settings ---
LedState states[] = {LedState::RUN, LedState::STOP, LedState::BLINK, LedState::ERROR};
const unsigned long STATE_INTERVAL = 5000; // 5 seconds per state
int stateIndex = 0;
unsigned long lastStateChange = 0;

void setup()
{
  Serial.begin(9600);
  Led_Init(led);

  Serial.println("Starting LED manual test!");
  Serial.println("State cycle: RUN -> STOP -> BLINK -> ERROR");

  // Start with the first state
  Led_SetState(led, states[stateIndex]);
  lastStateChange = millis();
  Serial.print("LED state: ");
  Serial.println("RUN");
}

void loop()
{
  // Update LED behavior every loop
  Led_Update(led);

  // Automatically cycle to next state after interval
  if (millis() - lastStateChange >= STATE_INTERVAL)
  {
    stateIndex = (stateIndex + 1) % 4; // loop through all states
    Led_SetState(led, states[stateIndex]);
    lastStateChange = millis();

    Serial.print("LED state changed to: ");
    switch (states[stateIndex])
    {
    case LedState::RUN:
      Serial.println("RUN");
      break;
    case LedState::STOP:
      Serial.println("STOP");
      break;
    case LedState::BLINK:
      Serial.println("BLINK");
      break;
    case LedState::ERROR:
      Serial.println("ERROR");
      break;
    }
  }
}
```

# IR Sensor

```cpp
#include <Arduino.h>
#include "utils/IrSensor.h"

const uint8_t IR_PIN = 2;
IrSensor ir = IrSensor_Create(IR_PIN);

void setup()
{
  Serial.begin(9600);
  IrSensor_Init(ir);
  Serial.println("IR Sensor Manual Test Started!");
  Serial.println("Cover or unblock the sensor to trigger events...");
}

void loop()
{
  // Update the sensor reading
  IrSensor_Update(ir);

  // Check if something is currently detected
  if (IrSensor_IsDetected(ir))
  {
    Serial.println("IR Sensor DETECTED something!");
  }

  // Check if a rising/falling event occurred
  if (IrSensor_IsEventTrue(ir))
  {
    Serial.println("IR Sensor EVENT triggered!");
  }

  delay(100);
}

```

# Color Sensor

```cpp
#include <Arduino.h>

#include "utils/ColorSensor.h"

ColorSensor cs = ColorSensor_Create(2, 3, 4);

void setup()
{
    Serial.begin(9600);
    ColorSensor_Init(cs);
}

void loop()
{
    if (Serial.available())
    {
        char ch = Serial.read();
        if (ch == 'r') // for example, type 'r' to trigger
        {
            Color c = ColorSensor_DetectColor(cs);
            Serial.println(c == Color::RED ? "RED" : c == Color::GREEN ? "GREEN"
                                                 : c == Color::BLUE    ? "BLUE"
                                                                       : "NONE");
        }
    }
}
```

# Object Manager
```cpp
#include <Arduino.h>

#include "ObjectManager.h"

ColorSensor cs = ColorSensor_Create(2, 3, 4);
IrSensor ir = IrSensor_Create(7);

ObjectManager objMng = ObjectManager_Create(ir, cs);

void setup()
{
    Serial.begin(9600);
    ObjectManager_Init(objMng);
}

void loop()
{
    ObjectManager_Update(objMng);

    if (ObjectManager_IsEventTrue(objMng))
    {
        Color cl = ObjectManager_GetColor(objMng);
        Serial.println(cl == Color::RED ? "RED" : cl == Color::BLUE ? "BLUE"
                                              : cl == Color::GREEN  ? "GREEN"
                                                                    : "404");
    }
}
```

# Servo Motor
```cpp
#include <Arduino.h>
#include "utils/ServoMotor.h"

// Configuration
constexpr uint8_t SERVO_PIN = 9;
constexpr uint8_t RESET_ANGLE = 30;
constexpr uint8_t PUSH_ANGLE = 120;

// Declare the instance globally
ServoMotor myServo;

void setup()
{
    Serial.begin(9600);
    Serial.println("Servo Motor Test Starting...");

    // Setup values and initialize
    ServoMotor_Create(&myServo, SERVO_PIN, RESET_ANGLE, PUSH_ANGLE);
    ServoMotor_Init(&myServo);
}

void loop()
{
    Serial.println("Triggering servo (PUSH)");
    ServoMotor_Trigger(&myServo);
    delay(1000);

    Serial.println("Resetting servo (HOME)");
    ServoMotor_Reset(&myServo);
    delay(2000);
}
```

# Sorting Manager (Mock)

```cpp
#include <Arduino.h>
#include "ObjectManager.h"
#include "SortingManager.h"
#include "utils/ServoMotor.h"

// -------------------- Object Manager --------------------
ColorSensor cs = ColorSensor_Create(2, 3, 4); // Pins for color sensor
IrSensor ir = IrSensor_Create(7);             // Object detection IR

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
IrSensor bin3Ir = IrSensor_Create(8); // End bin IR

// -------------------- Hardware structs --------------------
Hardware bin1 = {ir : bin1Ir, servo : &redServo};
Hardware bin2 = {ir : bin2Ir, servo : &greenServo};

// -------------------- Sorting Manager --------------------
SortingManager sorter = SortingManager_Create(bin1, bin2, bin3Ir, objMng);

void setup()
{
    Serial.begin(9600);
    Serial.println("Sorting Manager Test Start...");

    ServoMotor_Create(&redServo, SERVO1_PIN, RESET1_ANGLE, PUSH1_ANGLE);
    ServoMotor_Create(&greenServo, SERVO2_PIN, RESET2_ANGLE, PUSH2_ANGLE);

    SortingManager_Init(sorter);
}

void loop()
{
    // -------------------- Update Sorting Manager --------------------
    SortingManager_Update(sorter);
}
```

# Bin Manager Test
```cpp
#include <Arduino.h>
#include "BinManager.h"

IrSensor bin1Ir = IrSensor_Create(4);
IrSensor bin2Ir = IrSensor_Create(5);
IrSensor bin3Ir = IrSensor_Create(14);

BinManager binMng = BinManager_Create(bin1Ir, bin2Ir, bin3Ir);

void setup()
{
    Serial.begin(9600);
    Serial.println("Sorting Manager Test Start...");

    BinManager_Init(binMng);
}

void loop()
{
    BinManager_Update(binMng);
    if (BinManager_GetState(binMng) == BinManagerState::FULL)
    {
        Serial.println(BinManager_GetBin(binMng) == Bin::BIN_1 ? "1" : BinManager_GetBin(binMng) == Bin::BIN_2 ? "2"
                                                                   : BinManager_GetBin(binMng) == Bin::BIN_3   ? "3"
                                                                                                               : "NONE");
        if (Serial.available())
        {
            char ch = Serial.read();
            if (ch == 'o')
            {
                BinManager_OnCompleted(binMng);
            }
        }
    }
}
```

# WiFi Communication Test
```cpp
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
```

# Servo Test
```cpp
#include <Arduino.h>

#include "utils/ColorSensor.h"
#include "utils/GearMotor.h"

// Pin assignments for the color sensor (optional if not used)
ColorSensor cs = ColorSensor_Create(D4, D2, D6);

// Pin assignments for motor driver
const uint8_t pwmPin = D0;   // GPIO D0 for PWM control
const uint8_t dirPin = D3;   // GPIO D3 for direction control

// Create the motor (use correct pins for motor)
GearMotor motor = GearMotor_Create(pwmPin, dirPin);  // Example: PWM on D0, Direction on D3

void setup() {
    Serial.begin(9600);  // Start serial communication for debugging
    GearMotor_Init(motor);  // Initialize the motor
}

void loop() {
    // Run motor forward at speed 200 (out of 255)
    GearMotor_Run(motor, 250, GearMotorDirection::FORWARD);  
    Serial.println("Motor running forward at speed 200");
    delay(2000);  // Run for 2 seconds

    // Run motor backward at speed 100
    GearMotor_Run(motor, 100, GearMotorDirection::BACKWARD);  
    Serial.println("Motor running backward at speed 100");
    delay(2000);  // Run for 2 seconds

    // Stop motor
    GearMotor_Stop(motor);  
    Serial.println("Motor stopped");
    delay(1000);  // Wait for 1 second before restarting
}
```