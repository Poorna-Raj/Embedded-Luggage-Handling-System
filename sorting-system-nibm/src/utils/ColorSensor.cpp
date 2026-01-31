#include "utils/ColorSensor.h"

ColorSensor ColorSensor_Create(const uint8_t s2, const uint8_t s3, const uint8_t out) {
    return ColorSensor{
        .s2Gpio = s2,
        .s3Gpio = s3,
        .outGpio = out,
        .samplesToAverage = 5,
        .initialized = false
    };
}

void ColorSensor_Init(ColorSensor &cs) {
    if (cs.initialized) return;

    pinMode(cs.s2Gpio, OUTPUT);
    pinMode(cs.s3Gpio, OUTPUT);
    pinMode(cs.outGpio, INPUT);

    // Initial state
    digitalWrite(cs.s2Gpio, LOW);
    digitalWrite(cs.s3Gpio, LOW);

    cs.initialized = true;
}

// Internal helper for reading raw frequency
static int ColorSensor_ReadFreq(ColorSensor &cs, Color color) {
    switch (color) {
        case Color::RED:
            digitalWrite(cs.s2Gpio, LOW);  digitalWrite(cs.s3Gpio, LOW);
            break;
        case Color::GREEN:
            digitalWrite(cs.s2Gpio, HIGH); digitalWrite(cs.s3Gpio, HIGH);
            break;
        case Color::BLUE:
            digitalWrite(cs.s2Gpio, LOW);  digitalWrite(cs.s3Gpio, HIGH);
            break;
        default: return 99999;
    }

    // Crucial for ESP8266: Wait for sensor to switch filters
    delay(10); 
    
    // Increased timeout to 20ms for reliable reading on ESP8266
    long duration = pulseIn(cs.outGpio, LOW, 20000);
    return (duration == 0) ? 99999 : (int)duration;
}

void ColorSensor_CalibrateWhite(ColorSensor &cs) {
    Serial.println("Calibrating... Place a WHITE object in front of sensor.");
    delay(2000); // Give user time to place object
    cs.minValues[0] = ColorSensor_ReadFreq(cs, Color::RED);
    cs.minValues[1] = ColorSensor_ReadFreq(cs, Color::GREEN);
    cs.minValues[2] = ColorSensor_ReadFreq(cs, Color::BLUE);
    Serial.println("Calibration Done!");
}

Color ColorSensor_DetectColor(ColorSensor &cs) {
    if (!cs.initialized) return Color::NONE;

    // Get raw pulse widths (Lower = Stronger)
    int r = ColorSensor_ReadFreq(cs, Color::RED);
    int g = ColorSensor_ReadFreq(cs, Color::GREEN);
    int b = ColorSensor_ReadFreq(cs, Color::BLUE);

    // DEBUG PRINT - Look at these numbers in Serial Monitor!
    Serial.printf("RAW DATA -> R: %d | G: %d | B: %d\n", r, g, b);

    // If G is 99999 or 0, the pin/wire is broken.
    if (g >= 99999 || g <= 0) {
        Serial.println("ERROR: Green sensor reading failed. Check S2/S3 wiring.");
        return Color::NONE;
    }

    // Apply a "Green Bias" 
    // Most TCS3200 sensors are 20-30% less sensitive to Green.
    // We subtract a buffer from Green to make it "more competitive."
    int greenAdjusted = g - (g * 0.20); 

    if (r < greenAdjusted && r < b) return Color::RED;
    if (greenAdjusted < r && greenAdjusted < b) return Color::GREEN;
    if (b < r && b < greenAdjusted) return Color::BLUE;

    return Color::NONE;
}