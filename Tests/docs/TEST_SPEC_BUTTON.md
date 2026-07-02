# Button Module – White‑Box Specification, Traceability Matrix & UML State Diagram

---

# 1. White‑Box Testing Methods Used

## 1.1 Control Flow Testing
Tests exercise all internal branches:
- `raw_level == stable_state`
- `raw_level != stable_state`
- debounce counter increments
- debounce counter reaches threshold
- debounce counter resets on bounce

## 1.2 Condition Testing
Tests validate internal boolean conditions:
- `debounce_counter < DEBOUNCE_TICKS`
- `debounce_counter >= DEBOUNCE_TICKS`
- transitions only after stable input

## 1.3 State Transition Testing
The button driver behaves as a small internal FSM:
- stable `0 → 1` → press  
- stable `1 → 0` → release

## 1.4 Internal Knowledge‑Based Testing
Tests rely on internal implementation details:
- exact debounce threshold (`DEBOUNCE_TICKS = 3`)
- how `button_tick()` updates internal variables
- how events are generated

---

# 2. Debounce State Machine

## 2.1 States

| State | Meaning |
|-------|---------|
| **STABLE_0** | Button is considered released |
| **STABLE_1** | Button is considered pressed |
| **DEBOUNCING_TO_1** | Raw changed to 1, waiting for stability |
| **DEBOUNCING_TO_0** | Raw changed to 0, waiting for stability |

## 2.2 Transitions

STABLE_0
    --(raw=1)-->
        DEBOUNCING_TO_1
            --(stable for 3 ticks)-->
                STABLE_1
                    → EVENT_BTN_PRESS

STABLE_1
    --(raw=0)-->
        DEBOUNCING_TO_0
            --(stable for 3 ticks)-->
                STABLE_0
                    → EVENT_BTN_RELEASE

---

# 3. Requirements and Test Cases

## REQ‑DB1 — Debounce Threshold
A raw input change is accepted only after  
**`DEBOUNCE_TICKS` consecutive identical samples**.

### Test Cases

| ID | Description | Raw Sequence | Expected |
|----|-------------|--------------|----------|
| TC‑DB1‑A | Not stable long enough | `{1,1}` | EVENT_NONE |
| TC‑DB1‑B | Stable exactly at threshold | `{1,1,1}` | EVENT_BTN_PRESS |
| TC‑DB1‑C | Bounce then stable | `{1,0,1,1,1}` | EVENT_BTN_PRESS |

---

## REQ‑SM1 — State Definitions
The module implements:
- STABLE_0  
- STABLE_1  
- DEBOUNCING_TO_1  
- DEBOUNCING_TO_0  

---

## REQ‑SM2 — Transition: 0 → 1 (Press)

### Test Cases

| ID | Description | Raw Sequence | Expected |
|----|-------------|--------------|----------|
| TC‑SM2‑A | Simple stable press | `{1,1,1}` | EVENT_BTN_PRESS |
| TC‑SM2‑B | Bounce before press | `{1,0,1,1,1}` | EVENT_BTN_PRESS |
| TC‑SM2‑C | Not enough ticks | `{1,1}` | EVENT_NONE |

---

## REQ‑SM3 — Transition: 1 → 0 (Release)

### Test Cases

| ID | Description | Raw Sequence | Expected |
|----|-------------|--------------|----------|
| TC‑SM3‑A | Simple stable release | `{1,1,1,0,0,0}` | EVENT_BTN_RELEASE |
| TC‑SM3‑B | Bounce before release | `{1,1,1,0,1,0,0,0}` | EVENT_BTN_RELEASE |
| TC‑SM3‑C | Not enough ticks | `{1,1,1,0,0}` | EVENT_BTN_PRESS |

---

## REQ‑DB2 — Counter Reset on Bounce

### Test Cases

| ID | Description | Raw Sequence | Expected |
|----|-------------|--------------|----------|
| TC‑DB2‑A | Bounce resets counter | `{1,0,1,1,1}` | EVENT_BTN_PRESS |
| TC‑DB2‑B | Bounce during release | `{1,1,1,0,1,0,0,0}` | EVENT_BTN_RELEASE |

---

## REQ‑EV1 — Single Event per Stable Transition

### Test Cases

| ID | Description | Raw Sequence | Expected |
|----|-------------|--------------|----------|
| TC‑EV1‑A | No event during bounce | `{1,0,1}` | EVENT_NONE |
| TC‑EV1‑B | Event only after stable press | `{1,1,1}` | EVENT_BTN_PRESS |
| TC‑EV1‑C | Event only after stable release | `{1,1,1,0,0,0}` | EVENT_BTN_RELEASE |

---

## REQ‑QS1 — `button_is_pressed()` Reflects Stable State

### Test Cases

| ID | Description | Raw Sequence | Expected |
|----|-------------|--------------|----------|
| TC‑QS1‑A | Never pressed | `{0,0,0}` | is_pressed = 0 |
| TC‑QS1‑B | Pressed and stable | `{1,1,1}` | is_pressed = 1 |
| TC‑QS1‑C | Press then release | `{1,1,1,0,0,0}` | is_pressed = 0 |

---

# 4. Traceability Matrix (REQ ↔ Test Case)

| Requirement ID | Test Case ID | Raw Sequence | Expected Result |
|----------------|--------------|--------------|-----------------|
| REQ‑DB1 | TC‑DB1‑A | `{1,1}` | EVENT_NONE |
| REQ‑DB1 | TC‑DB1‑B | `{1,1,1}` | EVENT_BTN_PRESS |
| REQ‑DB1 | TC‑DB1‑C | `{1,0,1,1,1}` | EVENT_BTN_PRESS |
| REQ‑SM2 | TC‑SM2‑A | `{1,1,1}` | EVENT_BTN_PRESS |
| REQ‑SM2 | TC‑SM2‑B | `{1,0,1,1,1}` | EVENT_BTN_PRESS |
| REQ‑SM2 | TC‑SM2‑C | `{1,1}` | EVENT_NONE |
| REQ‑SM3 | TC‑SM3‑A | `{1,1,1,0,0,0}` | EVENT_BTN_RELEASE |
| REQ‑SM3 | TC‑SM3‑B | `{1,1,1,0,1,0,0,0}` | EVENT_BTN_RELEASE |
| REQ‑SM3 | TC‑SM3‑C | `{1,1,1,0,0}` | EVENT_BTN_PRESS |
| REQ‑DB2 | TC‑DB2‑A | `{1,0,1,1,1}` | EVENT_BTN_PRESS |
| REQ‑DB2 | TC‑DB2‑B | `{1,1,1,0,1,0,0,0}` | EVENT_BTN_RELEASE |
| REQ‑EV1 | TC‑EV1‑A | `{1,0,1}` | EVENT_NONE |
| REQ‑EV1 | TC‑EV1‑B | `{1,1,1}` | EVENT_BTN_PRESS |
| REQ‑EV1 | TC‑EV1‑C | `{1,1,1,0,0,0}` | EVENT_BTN_RELEASE |
| REQ‑QS1 | TC‑QS1‑A | `{0,0,0}` | is_pressed = 0 |
| REQ‑QS1 | TC‑QS1‑B | `{1,1,1}` | is_pressed = 1 |
| REQ‑QS1 | TC‑QS1‑C | `{1,1,1,0,0,0}` | is_pressed = 0 |

---

# 5. Mermaid UML State Diagram

```mermaid
stateDiagram-v2
    [*] --> STABLE_0

    STABLE_0 --> DEBOUNCING_TO_1 : raw=1
    DEBOUNCING_TO_1 --> STABLE_1 : stable for 3 ticks
    DEBOUNCING_TO_1 --> DEBOUNCING_TO_0 : raw=0 (bounce)

    STABLE_1 --> DEBOUNCING_TO_0 : raw=0
    DEBOUNCING_TO_0 --> STABLE_0 : stable for 3 ticks
    DEBOUNCING_TO_0 --> DEBOUNCING_TO_1 : raw=1 (bounce)

    STABLE_1 --> STABLE_1 : raw=1 (stable)
    STABLE_0 --> STABLE_0 : raw=0 (stable)

    STABLE_0 --> STABLE_1 : EVENT_BTN_PRESS
    STABLE_1 --> STABLE_0 : EVENT_BTN_RELEASE