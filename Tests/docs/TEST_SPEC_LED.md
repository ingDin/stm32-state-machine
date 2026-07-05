# TEST_SPEC_LED — LED Module Test Specification

## 1. Overview
This document defines the functional test specification for the LED timing module.  
It is derived from the parameterized unit tests:
- `test_led_init_parameterized`
- `test_led_tick_parameterized`

These tests validate periodic LED toggling using a deterministic fake HAL.

---

## 2. Functional Description

### 2.1 LED Timing Logic
The LED toggles periodically every **500 ms** (`LED_PERIOD_MS`).  
Timing is computed using:

(now - last_tick) >= LED_PERIOD_MS

Code

Where:
- `now` = `HAL_GetTick()`
- `hal_toggle_led()` performs the actual toggle

### 2.2 Initialization Behavior
`led_init()` must:
- Read current tick
- Reset internal timing (`last_tick`)
- Reset toggle counter (`fake_hal_reset`)
- Prepare module for periodic toggling

### 2.3 Fake HAL Behavior
The fake HAL provides:
- Deterministic tick progression (`fake_hal_set_tick`)
- Deterministic toggle counting (`fake_hal_get_toggle_count`)
- Isolation from hardware

---

## 3. Requirements

| Requirement ID        | Description                                      |
|-----------------------|--------------------------------------------------|
| REQ‑LED‑INIT‑01       | Initialization must reset tick baseline          |
| REQ‑LED‑INIT‑02       | Initialization must reset toggle counter         |
| REQ‑LED‑TICK‑01       | LED must not toggle before 500 ms                |
| REQ‑LED‑TICK‑02       | LED must toggle exactly at 500 ms                |
| REQ‑LED‑TICK‑03       | LED must toggle at multiples of 500 ms           |
| REQ‑LED‑HAL‑01        | Tick must use HAL_GetTick deterministically      |
| REQ‑LED‑HAL‑02        | Toggle must call hal_toggle_led()                |

---

## 4. Test Cases

### 4.1 test_led_init_parameterized
Validates:
- Tick baseline reset  
- Toggle counter reset  
- Correct initialization across multiple initial tick values  

### 4.2 test_led_tick_parameterized
Validates:
- No toggle before 500 ms  
- Toggle at 500 ms  
- Toggles at 1000 ms, 1500 ms  
- Deterministic tick progression  

---

## 5. Traceability Matrix

| Requirement           | Test Case                    |
|-----------------------|------------------------------|
| REQ‑LED‑INIT‑01       | test_led_init_parameterized  |
| REQ‑LED‑INIT‑02       | test_led_init_parameterized  |
| REQ‑LED‑TICK‑01       | test_led_tick_parameterized  |
| REQ‑LED‑TICK‑02       | test_led_tick_parameterized  |
| REQ‑LED‑TICK‑03       | test_led_tick_parameterized  |
| REQ‑LED‑HAL‑01        | test_led_tick_parameterized  |
| REQ‑LED‑HAL‑02        | test_led_tick_parameterized  |

---

## 6. UML Sequence Diagram (Mermaid)

```mermaid
sequenceDiagram
    participant Test
    participant LED
    participant HAL

    Test->>HAL: fake_hal_reset()
    Test->>HAL: fake_hal_set_tick(initial_tick)
    Test->>LED: led_init()
    LED->>HAL: HAL_GetTick()
    LED->>HAL: fake_hal_reset()

    loop For each tick value
        Test->>HAL: fake_hal_set_tick(tick)
        Test->>LED: led_tick()
        LED->>HAL: HAL_GetTick()
        alt tick - last_tick >= 500
            LED->>HAL: hal_toggle_led()
            HAL->>HAL: toggle_counter++
        end
    end

    Test->>HAL: fake_hal_get_toggle_count()