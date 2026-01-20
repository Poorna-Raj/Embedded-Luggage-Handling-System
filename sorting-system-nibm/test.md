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