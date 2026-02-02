#include "ObjectManager.h"

void handleIdleState(ObjectManager &omn);
void handleDetectedState(ObjectManager &omn);
void handleFailedState(ObjectManager &omn);

// Create an instance of ObjectManager with sensor references
ObjectManager ObjectManager_Create(IrSensor &ir, ColorSensor &cs)
{
    return ObjectManager{
        .ir = ir,
        .cs = cs,
        .state = ObjManagerState::IDLE,
        .color = Color::NONE,
        .preColor = Color::NONE,
        .initialized = false,
        .eventConsumed = false,
    };
}

// Initialize the ObjectManager
void ObjectManager_Init(ObjectManager &omn)
{
    if (omn.initialized)
        return;

    IrSensor_Init(omn.ir);
    ColorSensor_Init(omn.cs);
    ColorSensor_CalibrateWhite(omn.cs);

    omn.initialized = true;
}

// Update the state of the ObjectManager
void ObjectManager_Update(ObjectManager &omn)
{
    if (!omn.initialized)
        return;

    // Update the IR sensor state
    IrSensor_Update(omn.ir);

    // Process based on the current state
    switch (omn.state)
    {
    case ObjManagerState::IDLE:
        handleIdleState(omn);
        break;
    case ObjManagerState::DETECTED:
        handleDetectedState(omn);
        break;
    case ObjManagerState::FAILED:
        handleFailedState(omn);
        break;
    default:
        omn.state = ObjManagerState::FAILED;  // Handle unknown states
        break;
    }
}

// Handle the IDLE state
void handleIdleState(ObjectManager &omn)
{
    if (IrSensor_IsEventTrue(omn.ir))  // Check for IR sensor event
    {
        Color color = ColorSensor_DetectColor(omn.cs);  // Detect color
        if (color != Color::NONE)
        {
            omn.color = color;  // Store detected color
            omn.state = ObjManagerState::DETECTED;  // Transition to DETECTED state
            omn.eventConsumed = false;
        }
        else
        {
            omn.state = ObjManagerState::IDLE;  // Transition to FAILED if no color detected
        }
    }
}

// Handle the DETECTED state
void handleDetectedState(ObjectManager &omn)
{
    if (!omn.eventConsumed && !IrSensor_IsDetected(omn.ir))  // Wait for IR sensor to stop detecting
    {
        omn.state = ObjManagerState::IDLE;  // Transition back to IDLE state
        omn.eventConsumed = false;  // Reset event consumption for the next cycle
    }
}

// Handle the FAILED state
void handleFailedState(ObjectManager &omn)
{
    if (!omn.eventConsumed)  // Print failure message only once
    {
        Serial.println("Something went wrong in Object Manager");
        omn.eventConsumed = true;  // Prevent repeated printing
    }
}

// Check if the event in the DETECTED state is valid
bool ObjectManager_IsEventTrue(ObjectManager &omn)
{
    if (omn.state == ObjManagerState::DETECTED && !omn.eventConsumed)
    {
        omn.preColor = omn.color;  // Store the color when the event is true
        omn.eventConsumed = true;  // Mark the event as consumed
        omn.state = ObjManagerState::IDLE;  // Reset state to IDLE
        return true;
    }
    return false;
}

// Get the last detected color
Color ObjectManager_GetColor(const ObjectManager &omn)
{
    return omn.preColor;  // Return the previously detected color
}
