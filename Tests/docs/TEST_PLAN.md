# **TEST PLAN – STM32 LED State Machine Project**

---

## **1. Test Objectives**
- Validate correctness of the finite state machine:
  **OFF → ON → BLINK_SLOW → BLINK_FAST → OFF**
- Verify non‑blocking LED logic (no HAL_Delay).
- Confirm timing behavior based on `HAL_GetTick()`.
- Validate button event generation and debounce logic.
- Ensure module isolation (button, led, state_machine, app).
- Achieve full TDD coverage through unit + integration tests.

---

## **2. Test Items**
- `state_machine.c / state_machine.h`
- `button.c / button.h`
- `led.c / led.h`
- `app.c` (logic only)
- `fake_hal.c` (test double)
- Unity unit test suite
- Integration tests (event flow)

---

## **3. Test Deliverables**
- **Test Plan** (this document)
- **Unit Test Suite**
- **Integration Test Suite**
- **Test Execution Report**
- **Defect Log**
- **Test Summary Report** (closure)

---

## **4. Test Environment**
- **Host:** PC with GCC/Clang
- **Framework:** Unity Test Framework
- **Fake HAL:** deterministic tick + toggle simulation
- **Build System:** Makefile / CMake
- **Optional Hardware:** STM32 board for smoke tests
- **Optional CI:** GitHub Actions

---

## **5. Test Schedule**
| Activity | Duration | Output |
|----------|----------|---------|
| Test planning | 1 day | Test Plan |
| Unit test development | 2–3 days | Unit test suite |
| TDD implementation | 3–5 days | Code + passing tests |
| Integration tests | 1–2 days | Integration suite |
| Hardware smoke tests | 1 day | Hardware test report |
| Test closure | 0.5 day | Summary report |

---

## **6. Estimating Test Resources**
- **1 Embedded Engineer** (TDD + implementation)
- **1 QA Engineer** (optional, hardware tests)
- **1 STM32 board**
- **Software:** Unity, GCC, GitHub, VSCode

---

## **7. Test Organization & Management**
- **Test Lead:** planning, reporting, defect triage
- **Developer:** TDD, implementation, writing tests
- **Reviewer:** code review + coverage validation
- **DevOps:** CI pipeline maintenance

---

## **8. Assigning Responsibilities**
| Role | Responsibilities |
|------|------------------|
| Developer | TDD, writing tests, implementing FSM |
| Test Lead | Planning, reviewing, reporting |
| QA | Hardware-level validation |
| DevOps | CI automation |

---

## **9. Test Management Tools**
- **GitHub Issues** – defect tracking  
- **GitHub Projects** – planning & Kanban  
- **GitHub Actions** – automated test execution  
- **lcov** – code coverage reports  

---

## **10. Risk Management**
### **Risks**
- Incorrect debounce → double transitions
- Timer overflow → unexpected behavior
- Tests dependent on each other → false positives
- Fake HAL mismatch → inaccurate timing tests

### **Mitigations**
- Full reset before each test
- Parameterized test tables
- Deterministic Fake HAL
- CI pipeline enforcing clean runs

---

## **11. Test Control, Monitoring & Reporting**
- Automated CI test runs on every commit  
- Daily reports including:
  - passed/failed tests
  - coverage percentage
  - new defects
- Sprint-end review of test results

---

## **12. Test Closure Activities**
- All tests passing  
- Coverage target achieved (>90%)  
- All defects resolved or accepted  
- Final Test Summary Report delivered  
- Archiving test artifacts and logs  

---
