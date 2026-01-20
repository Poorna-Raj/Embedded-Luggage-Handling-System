# 2026-01-16

## Implement the IR Sensor

### Description
When designing the IR sensor, I need to implement both systems:

1. Edge-detecting IR sensor

    - Used for:

        - Detecting object arrival for color sensor reading
        - Detecting object arrival for servo push action

2. Live-reading IR sensor

    - Used for:
        - Detecting whether a bin is full or not

DONE WHEN:

- IR sensor module provides:
    - Event-based detection (edge)
    - Continuous blocked/unblocked state (live)

No repeated triggers while the object stays in front

## Implement the LED

### Description
LED should act as an FSM (finite state machine) and be able to operate in different states.

1. RUN - light up continuously
2. STOP - stop the light up
3. BLINK - should blink the LED
4. ERROR - something went wrong

# 2026-01-17

## Test the LED
- Perform physical testing for the LED covering all the states in the system.
- Test code is also written - [code](test.md#led-test).
- No issues have been founded.

## Test the IR Sensor
- Perform physical testing covering all the functions.
- Check for edge detection as well as continuous state.
- Test code is also written - [code](test.md#ir-sensor).
- No issues have been founded.

## Implement and Test Color Sensor

### Implementation
The Color Sensor is responsible for detecting one of three colors: **RED, GREEN, or BLUE**.

- **Input Trigger:** The IR sensor signals object presence.
  - **No direct polling or updating is required**
  - The color sensor only reads when triggered.
- **Color Detection Logic:**
  - Classify the object as **RED**, **GREEN**, or **BLUE**.
  - Return the detected color as an enum.
- **Integration:**
  - The Color Sensor does **not control the servo** directly.
  - The SortingManager or controller module will use the detected color to decide which bin to push the object into.
- **DONE WHEN:**
  - The color sensor module exposes a single function:
    ```c++
    Color DetectColor(); // returns RED, GREEN, or BLUE

    ```
  - Works correctly for all test objects
  - Only reads color **when the IR sensor signals presence**
  - No repeated or continuous readings outside the trigger

```c++
enum class Color
{
    NONE,
    RED,
    GREEN,
    BLUE
};
```

### Testing
- Perform physical testing covering all the functions.
- Test code is also written - [code](test.md#color-sensor)
- Issues / TODOs:
    - [ ] Blocking Structure (//TODO)
    - [ ] Default reading as RED

## Design the Object Manager

### Purpose
The Object Manager is responsible for detecting objects and identifying their colors. It serves as a subsystem for the Sorting Manager, providing information about detected objects and their colors for further processing.

### Responsibilities

- Monitor an IR sensor to detect the presence of an object.

- Use a Color Sensor to determine the color of the detected object.

- Maintain and update an internal finite state machine (FSM) with the following states:
```cpp
enum class ObjectState {
    IDLE,      // No object detected
    DETECTED,  // Object detected and color identified
    FAILED     // Detection or color identification failed
};
```
- Store the detected color internally and provide it to other modules (e.g., Sorting Manager).

- Return to the IDLE state after completing detection and reporting.

### Notes

- The Object Manager acts as an FSM, transitioning between states automatically based on sensor readings.

- It abstracts the hardware details of the sensors from the Sorting Manager, providing a clean interface for object detection and color reporting.

## Design the Servo Motor Structure

### Purpose
The Servo Motor is responsible for pushing objects on the conveyor belt into their corresponding bins.

### Responsibilities

- Receive a trigger signal (e.g., from the Object Manager or IR sensor) to actuate and push the object.

- Automatically return to the reset/home position after a predefined duration or once the push action is complete.

- Provide smooth and reliable motion to ensure objects are pushed accurately without damaging them or the system.

### Notes

- The servo motor should abstract the mechanical details from higher-level modules (e.g., Sorting Manager).

- Timing or duration can be configured depending on the object size or speed of the conveyor.

- `Important` - The design got [updated](backlog.md#update-the-design-of-the-servo-motor) to cater the requirements of the Sorting System.

## Implement the Object Manager
- Perform the physical testing ensuring all the states working properly.
- Ensure safe transistions between the states, so it won't trigger duplicate events.
- Test code is included - [code](test.md#object-manager)

> Had an issue where the color sensor get triggered twise per movement,

To fix this issue, few changes has been implemented.
```cpp
bool ObjectManager_IsEventTrue(ObjectManager &omn)
{
    if (omn.state == ObjManagerState::DETECTED && !omn.eventConsumed)
    {
        omn.preColor = omn.color;
        omn.eventConsumed = true;
        return true;
    }
    return false;
}
```
- Add a flag to the `ObjectManager_IsEventTrue()` function without the immediate state change.
```cpp
void handleDetectedState(ObjectManager &omn)
{
    if (!omn.detectedPrinted)
    {
        omn.detectedPrinted = true;
    }

    if (omn.eventConsumed && !IrSensor_IsDetected(omn.ir))
    {
        omn.state = ObjManagerState::IDLE;
        omn.detectedPrinted = false;
        omn.failedPrinted = false;
        omn.eventConsumed = false;
    }
}
```
- Check weather the event is consumed and the object had been moved before resetting the state to `IDLE`.

## Add debounce to the IR Sensor
To get a more precise reading, the debounce has been implemented with a `busy falg`. Otherwise the IR sensor will detect multiple trigger per same movement.
```cpp
void IrSensor_Update(IrSensor &ir)
{
    if (!ir.initialized)
        return;

    ir.current = digitalRead(ir.gpio);

    if (!ir.event)
    {
        if (!ir.current && ir.previous && (millis() - ir.lastEventTime > ir.debounceMs))
        {
            ir.event = true;
            ir.lastEventTime = millis();
        }
    }

    ir.previous = ir.current;
}
```
- If an event is already stored, it won't look for another event till the stored event got consumed.
- The debounce done in the conditinoal block so it will be more consistant.

# 2026-01-18

## Design the Sorting Manager

### Purpose

The **Sorting Manager** is responsible for directing detected objects into their corresponding bins on the conveyor system.
It operates as a **finite state machine (FSM)** and uses the **Object Manager** as a sub-system to obtain object detection and color classification information.

### Responsibilities

- Coordinate the complete sorting cycle for **one object at a time**.
- Request and receive object color information from the **Object Manager**.
- Control **two independent servo motors**, each assigned to a specific side bin.
- Allow objects belonging to the remaining category to pass directly to the **end bin** without actuation.
- Maintain and update an internal FSM to ensure deterministic and non-blocking operation.
- Safely return the system to the **IDLE** state after each sorting cycle.

### Finite State Machine (FSM)

```c++
enum class SortState {
    IDLE,       // Waiting for an object
    WAITING,    // Object detected and classified
    ACTUATING,  // Servo motor is pushing the object
    ERROR       // Fault or invalid condition
};
```

### FSM Behavior

- **IDLE**
  - The system waits for the Object Manager to report a detected object.
  - No actuators are active in this state.
- **WAITING**
  - The Sorting Manager retrieves the detected object’s color from the Object Manager.
  - Based on the color, it determines the target bin:
    - **Left Bin** → Servo Motor A
    - **Right Bin** → Servo Motor B
    - **End Bin** → No servo actuation
- **ACTUATING**
  - The corresponding servo motor is triggered to push the object into its assigned bin.
  - Servo motion and reset are handled in a **non-blocking manner** using timing logic.
  - After actuation, the system prepares to return to the IDLE state.
- **ERROR**
  - Entered when an unexpected or invalid condition occurs.
  - Allows safe recovery or diagnostics.

## Update the Design of the Servo Motor

### Purpose

The Servo Motor is responsible for physically diverting objects on the conveyor belt
into their corresponding bins by pushing them into a specified direction.

### Responsibilities

- Move to a commanded position when triggered by the Sorting Manager.
- Hold its position until an explicit reset command is received.
- Return to the home/reset position only when instructed (e.g., after object passage
  is confirmed by an IR sensor).
- Provide stable and repeatable motion suitable for continuous operation.

### Notes

- The Servo Motor does not contain any timing logic or autonomous behavior.
- All sequencing, timing, and decision-making are handled by the Sorting Manager FSM.
- This abstraction keeps mechanical control simple and predictable.

## Implement the Sorting Manager
- Implement the sorting manager including all the states.
- A mock test has been executed with positive result - [code](test.md#sorting-manager-mock).
- A complete test need to be initalized.
- Ensure the Object Manager functions properly inside the Sorting Manager but had to face some issues where,
    - Multiple readings with object manager without being processed with the sorting manager seems to add someking of queue and goes to a failed state in Object Manager.
    - [ ] - Make sure to add an LED to represent the 1 object per cycle rule.
    - [ ] - Perform a full test with all the IR and Servo Motors.

## Design the Bin Manager
### Purpose

The Bin Manager is used to detect whether the Bins are full or not. These detections will be used to trigger the forklift system to reset the bin.

### Responsibilities

- Should constantly read the bin status.
- Each bin should contain an IR sensor.
- Take an average of the samples to maintain the accuracy.
- Maintain and update the internal FSM to ensure deterministic and non-blocking operations.

### Finite State Machine

```c++
enum class BinState {
  IDLE,
  FULL,
  ERROR,
}
```

- It should also maintain details about the full bin.

### Note

- Since there is only one object per cycle, there is no need to handle multiple bin-full situations simultaneously.
- IR sensors should measure the real-time data, not any edge detecting mechanism.

# 2026-01-19

## Rework on the Servo Motor
- The servo motor needed a rework since the copy of the servo object didn't function properly.
- Therefore, we moved the servo motor to pointers.

## Update the sorting manager to handle updated servo motor
- Rework on the sorting manager to use the servo motor as pointers.

## Test the sorting manager with full hardware setup
- Perform physic tests of the sorting manager to check all the states.
- Find some issues where the IR sensors get auto triggered but might be an hardware issue where the IR sensor filker since they were too close. //TODO

# 2026-01-20

## Implement the Bin Manager
- Implement the states and all it's implementations.
- Logic might need rework on physical constraints.
- The averate threashold for bins might need a change.