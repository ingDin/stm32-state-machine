/**
 * @file test_state_machine.h
 * @brief Test case structures for the state machine unit tests.
 *
 * Defines small data containers used by the parameterized tests:
 *   - state transition cases
 *   - blink timing cases
 *   - timer reset cases
 *
 * These structs support clean, table‑driven tests in test_state_machine.c.
 */

#ifndef TEST_STATE_MACHINE_H
#define TEST_STATE_MACHINE_H

void test_state_transitions_parameterized(void);
void test_blink_timeout_behavior_parameterized(void);
void test_timer_resets_on_state_change_parameterized(void);
void test_next_state_returns_current_when_no_transition_parameterized(void);
void test_guard_always_true_parameterized(void);

#endif