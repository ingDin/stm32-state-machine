# State Machine – Full Specification, Traceability Matrix & UML Diagram

---

# 1. White‑Box Specification (Derived from test_state_machine.c)

## 1.1 Verified Behaviors
The test suite validates the following behaviors:  


- Full transition cycle:  
  **OFF → ON → BLINK_SLOW → BLINK_FAST → OFF**
- Blink timing behavior for slow/fast modes
- LED toggle timing based on `hal_get_tick()`
- Internal timer reset on state transitions
- Guard‑controlled transitions
- No transitions on irrelevant events

---

# 2. State Machine Definition

## 2.1 States
| State | Meaning |
|-------|---------|
| **STATE_OFF** | LED off, idle |
| **STATE_ON** | LED steady on |
| **STATE_BLINK_SLOW** | LED blinking at 1000 ms interval |
| **STATE_BLINK_FAST** | LED blinking at 200 ms interval |

---

## 2.2 Events
| Event | Meaning |
|-------|---------|
| **EVENT_BTN_PRESS** | User button press |
| **EVENT_BTN_RELEASE** | Release (ignored for transitions) |
| **EVENT_TIMEOUT** | Tick event (used for blinking) |

---

## 2.3 Transition Rules

## Button‑Driven Transitions

The state machine responds to `EVENT_BTN_PRESS` with the following sequence:

## Button‑Driven Transitions

STATE_OFF
    --(EVENT_BTN_PRESS + guard==true)-->
        STATE_ON

STATE_ON
    --(EVENT_BTN_PRESS + guard==true)-->
        STATE_BLINK_SLOW

STATE_BLINK_SLOW
    --(EVENT_BTN_PRESS + guard==true)-->
        STATE_BLINK_FAST

STATE_BLINK_FAST
    --(EVENT_BTN_PRESS + guard==true)-->
        STATE_OFF

## Non‑Transition Events

The following events NEVER cause a state change:

EVENT_TIMEOUT
    → state remains unchanged

EVENT_BTN_RELEASE
    → state remains unchanged

### Guard Behavior
- If guard is **false**, OFF stays OFF  
- If guard is **true**, OFF transitions to ON  

---

## 2.4 Blink Timing Rules

### Slow Blink
- Interval: **1000 ms**
- LED toggles only when `tick - last_toggle >= 1000`

### Fast Blink
- Interval: **200 ms**
- LED toggles only when `tick - last_toggle >= 200`

Tests confirm:  
- No toggle before interval  
- Toggle exactly at interval  
- Multiple toggles at multiples of interval  


---

## 2.5 Timer Reset Rules
On every state transition:
- `timer = 0`
- Timer restarts counting on next tick  


---

# 3. Traceability Matrix (REQ ↔ Test Case)

| Requirement ID | Description | Test Case | Expected |
|----------------|-------------|-----------|----------|
| **REQ‑SM1** | Initial state must be OFF | Initial state case | STATE_OFF |
| **REQ‑SM2** | OFF → ON | 1 press | STATE_ON |
| REQ‑SM2 | ON → BLINK_SLOW | 2 presses | STATE_BLINK_SLOW |
| REQ‑SM2 | BLINK_SLOW → BLINK_FAST | 3 presses | STATE_BLINK_FAST |
| REQ‑SM2 | BLINK_FAST → OFF | 4 presses | STATE_OFF |
| **REQ‑SM3** | LED toggles only after interval | BLINK_SLOW @1000 | toggle=1 |
| REQ‑SM3 | LED toggles only after interval | BLINK_FAST @200 | toggle=1 |
| **REQ‑SM4** | Blink timeout behavior | SLOW @0/999/1000/1500/2000 | correct toggles |
| REQ‑SM4 | Blink timeout behavior | FAST @0/199/200/350/400 | correct toggles |
| **REQ‑SM5** | Timer resets on state change | All transitions | timer=0 |
| **REQ‑SM6** | No transition on irrelevant events | TIMEOUT/RELEASE | state unchanged |
| **REQ‑SM7** | Guard controls OFF→ON | guard=false/true | OFF or ON |

---

# 4. UML State Diagram

```mermaid
stateDiagram-v2
    [*] --> STATE_OFF

    STATE_OFF --> STATE_ON : EVENT_BTN_PRESS / guard==true
    STATE_ON --> STATE_BLINK_SLOW : EVENT_BTN_PRESS
    STATE_BLINK_SLOW --> STATE_BLINK_FAST : EVENT_BTN_PRESS
    STATE_BLINK_FAST --> STATE_OFF : EVENT_BTN_PRESS

    STATE_OFF --> STATE_OFF : EVENT_TIMEOUT, EVENT_BTN_RELEASE
    STATE_ON --> STATE_ON : EVENT_TIMEOUT, EVENT_BTN_RELEASE
    STATE_BLINK_SLOW --> STATE_BLINK_SLOW : EVENT_TIMEOUT, EVENT_BTN_RELEASE
    STATE_BLINK_FAST --> STATE_BLINK_FAST : EVENT_TIMEOUT, EVENT_BTN_RELEASE

    state STATE_BLINK_SLOW {
        [*] --> wait
        wait --> toggle : tick >= 1000
        toggle --> wait : reset timer
    }

    state STATE_BLINK_FAST {
        [*] --> wait
        wait --> toggle : tick >= 200
        toggle --> wait : reset timer
    }