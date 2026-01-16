#include "utils/Led.h"

Led Led_Create(const uint8_t gpio)
{
    return Led{
        .gpio = gpio,
        .state = LedState::STOP,
        .lastToggle = 0,
        .interval = 500,
        .ledOn = false,
        .initialized = false,
    };
}

void Led_Init(Led &led)
{
    if (led.initialized)
        return;

    pinMode(led.gpio, OUTPUT);
    digitalWrite(led.gpio, led.ledOn);

    led.initialized = true;
}

void Led_Update(Led &led)
{
    if (!led.initialized)
        return;

    unsigned long now = millis();

    switch (led.state)
    {
    case LedState::RUN:
        led.ledOn = true;
        break;
    case LedState::STOP:
        led.ledOn = false;
        break;
    case LedState::BLINK:
        if (now - led.lastToggle >= led.interval)
        {
            led.ledOn = !led.ledOn;
            led.lastToggle = now;
        }
        break;
    case LedState::ERROR:
        if (now - led.lastToggle >= 200)
        {
            led.ledOn = !led.ledOn;
            led.lastToggle = now;
        }
        break;
    default:
        led.ledOn = false;
        Serial.println("Invalid LED State change!");
        break;
    }

    digitalWrite(led.gpio, led.ledOn);
}

void Led_SetState(Led &led, LedState state)
{
    if (led.state != state)
    {
        led.lastToggle = millis();
        led.ledOn = false;
        led.state = state;
    }
}