# TEST_STRATEGY.md  
STM32 LED State Machine – TDD Project

---

# 1. Objectives and Goals of Testing
- Ensure the LED state machine behaves exactly as specified.
- Detect defects early through TDD.
- Validate correctness of all state transitions: OFF → ON → BLINK_SLOW → BLINK_FAST → OFF.
- Verify timing behavior using deterministic tick simulation.
- Ensure non‑blocking behavior (no HAL_Delay).
- Confirm robustness against edge cases (timing boundaries, rapid button presses).
- Guarantee maintainability and predictable embedded behavior.
- Provide confidence that simulation matches hardware behavior.

---

# 2. Scope and Boundaries of Testing

## 2.1 In Scope
- FSM logic (states, transitions, guards, timers)
- LED control logic (ON/OFF, slow/fast blink)
- Button event generation + debounce logic
- Fake HAL tick simulation
- Unit tests
- Integration tests
- Timing validation
- Code coverage

## 2.2 Out of Scope
- Electrical hardware validation
- Long-term endurance testing
- HAL driver correctness
- Performance or power analysis

---

# 3. Methodology – Overall Approach

## 3.1 Testing Philosophy
- Strict TDD: write failing test → implement → refactor.
- Deterministic simulation using Fake HAL.
- Table-driven tests for transitions and timing.
- Incremental testing: unit → integration → hardware smoke tests.
- Automation-first: CI runs all tests.

## 3.2 Testing Approach
- Deterministic Fake HAL for ticks and GPIO toggles.
- Parameterized tests for transitions and timing.
- Black-box + white-box techniques combined.
- Model-based testing using FSM diagram.

---

# 4. Test Levels

## 4.1 Unit Testing
- Framework: Unity
- Modules:
  - state_machine.c
  - button.c
  - led.c
  - fake_hal.c
- Focus:
  - transitions
  - timing boundaries
  - debounce
  - LED toggle logic

## 4.2 Integration Testing
- Flow: Button → FSM → LED
- Validates:
  - event propagation
  - timing correctness
  - module interaction

## 4.3 Hardware Smoke Testing
- Run on STM32 board
- Validate LED behavior matches simulation

---

# 5. Test Techniques

## 5.1 State Transition Testing
- Core technique because system = FSM.
- Ensures:
  - all states reachable
  - all transitions valid
  - no illegal transitions

## 5.2 Boundary Value Analysis (BVA)
Applied to timing:
- Slow blink: 999, 1000, 1001
- Fast blink: 199, 200, 201

## 5.3 Equivalence Partitioning (EP)
Tick ranges:
- < interval
- == interval
- > interval but < 2×interval
- >= 2×interval

## 5.4 Model-Based Testing
FSM diagram → derive:
- transition coverage
- path coverage
- guard coverage

## 5.5 White-Box Testing
- internal timer logic
- tick accumulation
- reset behavior

## 5.6 Black-Box Testing
- button events
- LED outputs
- observable FSM behavior

---

# 6. Test Strategies

## 6.1 Analytical
- Requirements-based test derivation
- Timing boundaries from blink intervals

## 6.2 Model-Based
- FSM diagram used as test model

## 6.3 Methodical / Standard-Compliant
- TDD
- Unity conventions
- deterministic Fake HAL

## 6.4 Reactive
- Tests added when defects appear

## 6.5 Consultative
- Developer + reviewer collaboration
- Peer review of test cases

---

# 7. Resources

## 7.1 Human
- 1 Embedded Developer
- 1 QA Engineer (optional)
- 1 Reviewer

## 7.2 Technical
- GCC/Clang
- Unity
- Fake HAL
- STM32 board
- GitHub Actions
- lcov

---

# 8. Roles and Responsibilities

| Role | Responsibilities |
|------|------------------|
| Developer | Write tests, implement FSM, maintain Fake HAL |
| QA | Hardware smoke tests |
| Reviewer | Review tests, ensure coverage |
| DevOps | Maintain CI |

---

# 9. Entry and Exit Criteria

## 9.1 Entry
- Requirements defined
- FSM diagram available
- Project structure created
- Fake HAL implemented
- Unity configured

## 9.2 Exit
- All unit tests pass
- All integration tests pass
- Coverage ≥ 90%
- No critical defects
- Hardware smoke tests passed

---

# 10. Risk Assessment

## 10.1 Risks
- Incorrect debounce → double transitions
- Timer overflow → unexpected behavior
- Fake HAL mismatch → incorrect timing tests
- Tests dependent on each other → false positives

## 10.2 Mitigations
- Reset state before each test
- Deterministic Fake HAL
- Table-driven tests
- CI enforcing clean runs
- BVA for timing

---

# 11. Test Environment
- GCC/Clang
- Unity
- Fake HAL
- STM32 hardware (optional)
- GitHub Actions

---

# 12. Test Data (DETAILED)

## 12.1 Tick Values (Timing Data)

### Slow Blink Interval = 1000 ms  
### Fast Blink Interval = 200 ms

| Scenario | Tick Value | Expected Behavior |
|---------|------------|-------------------|
| Before slow interval | 0–999 | No toggle |
| At slow boundary | 1000 | Toggle |
| After boundary | 1001 | No toggle |
| Before fast interval | 0–199 | No toggle |
| At fast boundary | 200 | Toggle |
| After fast boundary | 201 | No toggle |
| Multiple intervals | 2000, 3000 | Toggle once per interval |

**Tick Test Set:**
{0, 199, 200, 201, 500, 999, 1000, 1001, 1500, 2000, 3000}

Code

---

## 12.2 Button Press Sequences (Debounce + Events)

Assume debounce threshold = 50 ms.

| ID | Sequence | Expected Event | Notes |
|----|----------|----------------|-------|
| S1 | press 0 → release 20 | BUTTON_PRESSED | Normal |
| S2 | press 0 → release 10 → press 15 → release 25 | NO EVENT | Bounce |
| S3 | press 0 → release 40 → press 100 → release 140 | 2 × BUTTON_PRESSED | Valid double press |
| S4 | press 0 → release 500 | BUTTON_PRESSED | Long press |
| S5 | rapid presses at 0,50,100,150,200 | 5 × BUTTON_PRESSED | Stress test |

**Raw Data:**
S1: [(0,press),(20,release)]
S2: [(0,press),(10,release),(15,press),(25,release)]
S3: [(0,press),(40,release),(100,press),(140,release)]
S4: [(0,press),(500,release)]
S5: [(0,p),(20,r),(50,p),(70,r),(100,p),(120,r),(150,p),(170,r),(200,p),(220,r)]

Code

---

## 12.3 State Transition Table (FSM Core Test Data)

### States:
- OFF  
- ON  
- BLINK_SLOW  
- BLINK_FAST  

### Event:
- BUTTON_PRESSED

| Test ID | Current State | Event | Next State | Description |
|---------|----------------|--------|------------|-------------|
| T1 | OFF | BUTTON_PRESSED | ON | LED turns ON |
| T2 | ON | BUTTON_PRESSED | BLINK_SLOW | Start slow blinking |
| T3 | BLINK_SLOW | BUTTON_PRESSED | BLINK_FAST | Increase speed |
| T4 | BLINK_FAST | BUTTON_PRESSED | OFF | Turn LED OFF |

**Parameterized Test Data:**
{
{"OFF → ON", OFF, BUTTON_PRESSED, ON},
{"ON → BLINK_SLOW", ON, BUTTON_PRESSED, BLINK_SLOW},
{"BLINK_SLOW → BLINK_FAST", BLINK_SLOW, BUTTON_PRESSED, BLINK_FAST},
{"BLINK_FAST → OFF", BLINK_FAST, BUTTON_PRESSED, OFF}
}

Code

---

## 12.4 Expected LED Toggle Counts

### Slow Blink (1000 ms)
| Tick | Expected Toggles |
|------|------------------|
| 0–999 | 0 |
| 1000 | 1 |
| 1500 | 1 |
| 2000 | 2 |
| 3000 | 3 |

### Fast Blink (200 ms)
| Tick | Expected Toggles |
|------|------------------|
| 0–199 | 0 |
| 200 | 1 |
| 400 | 2 |
| 600 | 3 |
| 1000 | 5 |

**Toggle Test Data:**
Slow: 0→0, 999→0, 1000→1, 1500→1, 2000→2, 3000→3
Fast: 0→0, 199→0, 200→1, 400→2, 600→3, 1000→5

Code

---

# 13. Test Automation
- All unit tests automated via Unity.
- CI pipeline runs tests on every commit.
- Coverage reports generated automatically.
- Hardware tests remain manual.

---

# 14. Test Control, Monitoring, and Reporting
- CI provides pass/fail status.
- Coverage monitored.
- Defects tracked in GitHub Issues.
- Weekly review of test results.

---

# 15. Test Closure
- All tests green.
- Coverage target met.
- Defects resolved.
- Test Summary Report created.
- Artifacts archived.

---