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