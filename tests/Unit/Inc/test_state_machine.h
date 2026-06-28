/**
 * @file test_state_machine.h
 * @brief Test case structures for the state machine unit tests.
 *
 * Defines small data containers used by the parametrized tests:
 *   - state transition cases
 *   - blink timing cases
 *   - timer reset cases
 *
 * These structs support clean, table‑driven tests in test_state_machine.c.
 */


#ifndef TEST_STATE_MACHINE_H
#define TEST_STATE_MACHINE_H

#endif
 typedef struct {
    const char* description;
    int num_presses;
    state_t expected_state;
} state_transition_case_t;

typedef struct {
    state_t target_state;
    uint32_t tick;
    int expected_toggles;
    const char* description;
} blink_case_t;

typedef struct {
    const char* description;
    int num_presses_before_transition;
    state_t expected_state_after_transition;
} timer_reset_case_t;