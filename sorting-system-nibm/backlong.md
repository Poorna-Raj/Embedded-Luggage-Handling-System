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

### Notes:

- The Object Manager acts as an FSM, transitioning between states automatically based on sensor readings.

- It abstracts the hardware details of the sensors from the Sorting Manager, providing a clean interface for object detection and color reporting.