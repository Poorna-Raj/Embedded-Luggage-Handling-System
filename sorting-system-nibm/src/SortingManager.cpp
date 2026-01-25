#include "SortingManager.h"

// Forward declarations of internal state handlers
void handleIdleState(SortingManager &mng);
void handleWaitingState(SortingManager &mng);
void handleActuatingState(SortingManager &mng);
void handleErrorState(SortingManager &mng);

/**
 * Creates the SortingManager instance.
 * Note: Hardware references must be valid for the lifetime of this manager.
 */
SortingManager SortingManager_Create(Hardware &bin1, Hardware &bin2, IrSensor &bin3, ObjectManager &obj)
{
    return SortingManager{
        .bin1 = bin1,
        .bin2 = bin2,
        .bin3 = bin3,
        .state = SortState::IDLE,
        .mng = obj,
        .color = Color::NONE,
        .bin1Count = 0,
        .bin2Count = 0,
        .bin3Count = 0,
        .errorPrint = false,
        .initialized = false,
    };
}

/**
 * Initializes all sub-components.
 * Note: ServoMotor_Init now takes a pointer.
 */
void SortingManager_Init(SortingManager &mng)
{
    if (mng.initialized)
        return;

    ObjectManager_Init(mng.mng);

    // Using pointers for the new ServoMotor logic
    ServoMotor_Init(mng.bin1.servo);
    ServoMotor_Init(mng.bin2.servo);

    mng.initialized = true;
    Serial.println("Sorting Manager Initialized");
}

/**
 * Main state machine update loop
 */
void SortingManager_Update(SortingManager &mng)
{
    if (!mng.initialized)
        return;

    // Update all sensors
    ObjectManager_Update(mng.mng);

    switch (mng.state)
    {
    case SortState::IDLE:
        handleIdleState(mng);
        break;
    case SortState::WAITING:
        handleWaitingState(mng);
        break;
    case SortState::ACTUATING:
        handleActuatingState(mng);
        break;
    case SortState::ERROR:
        handleErrorState(mng);
        break;
    default:
        mng.state = SortState::ERROR;
        break;
    }
}

void handleIdleState(SortingManager &mng)
{
    if (ObjectManager_IsEventTrue(mng.mng))
    {
        mng.color = ObjectManager_GetColor(mng.mng);

        if (mng.color == Color::NONE)
        {
            Serial.println("Sorting Error: Object detected but color is NONE");
            mng.state = SortState::ERROR;
        }
        else
        {
            Serial.print("Object Detected. Color Code: ");
            Serial.println((int)mng.color);
            mng.state = SortState::WAITING;
        }
    }
}

void handleWaitingState(SortingManager &mng)
{
    // Safety check
    if (mng.color == Color::NONE)
    {
        mng.state = SortState::ERROR;
        return;
    }

    // Actuate the correct arm based on color
    if (mng.color == Color::RED)
    {
        Serial.println("Sorting to Bin 1 (RED)");
        ServoMotor_Trigger(mng.bin1.servo);
    }
    else if (mng.color == Color::BLUE)
    {
        Serial.println("Sorting to Bin 2 (BLUE)");
        ServoMotor_Trigger(mng.bin2.servo);
    }
    else if (mng.color == Color::GREEN)
    {
        Serial.println("Sorting to Bin 3 (GREEN - No Servo)");
        // Green doesn't have a servo in your current logic,
        // it just waits for the IR sensor in Actuating state.
    }

    mng.state = SortState::ACTUATING;
}

void handleActuatingState(SortingManager &mng)
{
    // Bin 1 logic (RED)
    if (mng.color == Color::RED && IrSensor_IsEventTrue(mng.bin1.ir))
    {
        Serial.println("Item entered Bin 1. Resetting Servo.");
        ServoMotor_Reset(mng.bin1.servo);
        mng.bin1Count++;
        mng.color = Color::NONE;
        mng.state = SortState::IDLE;
    }
    // Bin 2 logic (BLUE)
    else if (mng.color == Color::BLUE && IrSensor_IsEventTrue(mng.bin2.ir))
    {
        Serial.println("Item entered Bin 2. Resetting Servo.");
        ServoMotor_Reset(mng.bin2.servo);
        mng.bin2Count++;
        mng.color = Color::NONE;
        mng.state = SortState::IDLE;
    }
    // Bin 3 logic (GREEN - Passive bin)
    else if (mng.color == Color::GREEN && IrSensor_IsEventTrue(mng.bin3))
    {
        Serial.println("Item entered Bin 3 (Passive).");
        mng.bin3Count++;
        mng.color = Color::NONE;
        mng.state = SortState::IDLE;
    }
}

void handleErrorState(SortingManager &mng)
{
    if (!mng.errorPrint)
    {
        Serial.println("CRITICAL: Sorting Manager entered ERROR state.");
        mng.errorPrint = true;
    }
    // Logic could be added here to pulse an LED or wait for a reset button
}