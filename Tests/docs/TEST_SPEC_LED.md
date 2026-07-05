# TEST_SPEC_LED.md

## Overview
This document defines the complete functional test specification for the LED module, including requirements, traceability, verified behaviors, and a UML sequence diagram written as plain text. The specification is derived from the parameterized unit tests `test_led_init_parameterized` and `test_led_tick_parameterized`, which validate timing‑based LED toggling using a deterministic fake HAL.

## Functional Description
The LED module implements periodic toggling based on a fixed interval (LED_PERIOD_MS = 500 ms). It uses HAL_GetTick() to determine elapsed time and hal_toggle_led() to perform the actual toggle. A fake HAL is used in tests to ensure deterministic tick progression and toggle counting.

### Initialization Behavior
led_init() must:
- Read the current tick
- Reset internal timing (last_tick)
- Reset the toggle counter (fake_hal_reset)
- Prepare the module for periodic toggling

### Periodic Behavior
The LED toggles when:
(now - last_tick) >= LED_PERIOD_MS
Where "now" is obtained from HAL_GetTick().

### Fake HAL Behavior
The fake HAL provides deterministic tick progression (fake_hal_set_tick), deterministic toggle counting (fake_hal_get_toggle_count), and isolation from hardware.

## Requirements
REQ‑LED‑INIT‑01: LED initialization must reset tick baseline  
REQ‑LED‑INIT‑02: LED initialization must reset toggle counter  
REQ‑LED‑TICK‑01: LED must not toggle before 500 ms  
REQ‑LED‑TICK‑02: LED must toggle exactly at 500 ms  
REQ‑LED‑TICK‑03: LED must toggle at multiples of 500 ms  
REQ‑LED‑HAL‑01: LED tick must use HAL_GetTick deterministically  
REQ‑LED‑HAL‑02: LED toggle must call hal_toggle_led()  

## Test Cases
test_led_init_parameterized validates:
- Tick baseline reset
- Toggle counter reset
- Correct initialization behavior across multiple initial tick values

test_led_tick_parameterized validates:
- No toggle before 500 ms
- Toggle at 500 ms
- Multiple toggles at 1000 and 1500 ms
- Deterministic tick progression

## Traceability Matrix
REQ‑LED‑INIT‑01 → test_led_init_parameterized  
REQ‑LED‑INIT‑02 → test_led_init_parameterized  
REQ‑LED‑TICK‑01 → test_led_tick_parameterized  
REQ‑LED‑TICK‑02 → test_led_tick_parameterized  
REQ‑LED‑TICK‑03 → test_led_tick_parameterized  
REQ‑LED‑HAL‑01 → test_led_tick_parameterized  
REQ‑LED‑HAL‑02 → test_led_tick_parameterized  

## UML Sequence Diagram (plain text)
Sequence:
1. Test resets fake HAL  
2. Test sets initial tick  
3. Test calls led_init()  
4. LED reads HAL_GetTick  
5. LED resets toggle counter  
6. Loop for each tick value:  
   - Test sets tick  
   - Test calls led_tick()  
   - LED reads HAL_GetTick  
   - If (tick - last_tick >= 500):  
       - LED calls hal_toggle_led()  
       - HAL increments toggle counter  
7. Test reads fake_hal_get_toggle_count  

## Coverage Summary
The parameterized tests cover initialization behavior, timing boundaries, multiple intervals, deterministic tick progression, and LED toggle logic, providing full functional coverage of the LED timing subsystem.

## Conclusion
This specification fully describes the LED module behavior and its verification through parameterized unit tests. It follows the same structure and clarity as the existing TEST_SPEC_STATE_MACHINE.md in your repository.
