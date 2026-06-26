/**
 * @file test_state_machine.c
 * @brief Unit tests for the state machine module using the Unity framework.
 *
 * This file contains a suite of tests that validate the behavior of the
 * state machine implemented in state_machine.c. The tests follow the
 * Arrange–Act–Assert (AAA) pattern and verify all state transitions
 * triggered by EVENT_BTN_PRESS:
 *
 *   OFF → ON → BLINK_SLOW → BLINK_FAST → OFF
 *
 * The purpose of this test suite is to ensure deterministic and predictable
 * state transitions, supporting a TDD workflow for embedded applications.
 *
 * Dependencies:
 *   - Unity test framework
 *   - state_machine.h (public API of the state machine)
 *
 * Usage:
 *   These tests are executed via the project's test runner (e.g. Makefile
 *   target `make test`). All tests must pass before new features are added.
 */

#include "../unity/unity.h"
#include "../../Core/Inc/state_machine.h"
#include "../fakes/fake_hal.h"



void setUp(void) {}
void tearDown(void) {}

void test_initial_state_is_OFF(void) {
    // Arrange
    sm_init();

    // Assert
    TEST_ASSERT_EQUAL(STATE_OFF, sm_get_state());
}

void test_off_to_on_on_button_press(void) {
    // Arrange
    sm_init();

    // Act
    sm_handle_event(EVENT_BTN_PRESS);

    // Assert
    TEST_ASSERT_EQUAL(STATE_ON, sm_get_state());
}

void test_on_to_blink_slow_on_button_press(void) {
    // Arrange
    sm_init();

    // Act
    sm_handle_event(EVENT_BTN_PRESS); // OFF → ON
    sm_handle_event(EVENT_BTN_PRESS); // ON → BLINK_SLOW

    // Assert
    TEST_ASSERT_EQUAL(STATE_BLINK_SLOW, sm_get_state());
}

void test_blink_slow_to_blink_fast_on_button_press(void) {
    // Arrange
    sm_init();

    // Act
    sm_handle_event(EVENT_BTN_PRESS); // OFF → ON
    sm_handle_event(EVENT_BTN_PRESS); // ON → BLINK_SLOW
    sm_handle_event(EVENT_BTN_PRESS); // BLINK_SLOW → BLINK_FAST

    // Assert
    TEST_ASSERT_EQUAL(STATE_BLINK_FAST, sm_get_state());
}

void test_blink_fast_to_off_on_button_press(void) {
    // Arrange
    sm_init();

    // Act
    sm_handle_event(EVENT_BTN_PRESS); // OFF → ON
    sm_handle_event(EVENT_BTN_PRESS); // ON → BLINK_SLOW
    sm_handle_event(EVENT_BTN_PRESS); // BLINK_SLOW → BLINK_FAST
    sm_handle_event(EVENT_BTN_PRESS); // BLINK_FAST → OFF

    // Assert
    TEST_ASSERT_EQUAL(STATE_OFF, sm_get_state());
}


typedef struct {
    state_t target_state;
    uint32_t interval;
    const char* description;
} blink_param_t;

void test_blink_toggles_led_after_interval_parametrized(void)
{
    blink_param_t cases[] = {
        { STATE_BLINK_SLOW, 1000, "BLINK_SLOW interval" },
        { STATE_BLINK_FAST,  250, "BLINK_FAST interval" },
    };

    const int num_cases = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < num_cases; i++) {

        // ---------------------------------------------------------
        // FULL RESET — this is the critical part
        // ---------------------------------------------------------
        sm_init();          // reset state machine
        fake_hal_reset();   // reset tick + toggle counter
        fake_hal_set_tick(0);

        // Enter the correct state
        sm_handle_event(EVENT_BTN_PRESS); // OFF → ON
        sm_handle_event(EVENT_BTN_PRESS); // ON → BLINK_SLOW

        if (cases[i].target_state == STATE_BLINK_FAST) {
            sm_handle_event(EVENT_BTN_PRESS); // BLINK_SLOW → BLINK_FAST
        }

        // ---------------------------------------------------------
        // Before interval
        // ---------------------------------------------------------
        sm_tick();
        TEST_ASSERT_EQUAL_MESSAGE(
            0,
            fake_hal_get_toggle_count(),
            "LED must NOT toggle before interval"
        );

        // ---------------------------------------------------------
        // At interval
        // ---------------------------------------------------------
        fake_hal_set_tick(cases[i].interval);
        sm_tick();

        TEST_ASSERT_EQUAL_MESSAGE(
            1,
            fake_hal_get_toggle_count(),
            cases[i].description
        );
    }
}



