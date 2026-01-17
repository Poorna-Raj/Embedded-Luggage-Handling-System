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