/**
 * @file test_state_machine.c
 * @brief Unity tests for the state machine module.
 *
 * This suite verifies:
 *   - all state transitions triggered by EVENT_BTN_PRESS
 *     OFF → ON → BLINK_SLOW → BLINK_FAST → OFF
 *   - blink timing behavior for slow/fast modes
 *   - LED toggle timing based on HAL_GetTick()
 *   - internal state timer reset on transitions
 *
 * Tests follow the AAA pattern and support a TDD workflow
 * for the embedded state machine in state_machine.c.
 */


#include "../unity/unity.h"
#include "../../Core/Inc/state_machine.h"
#include "../fakes/fake_hal.h"
#include "test_state_machine.h"

void setUp(void) {}
void tearDown(void) {}

void test_state_transitions_parametrized(void)
{
    // ---------------------------------------------------------
    // Arrange: define all state transition scenarios
    // ---------------------------------------------------------
    state_transition_case_t cases[] = {
        { "Initial state must be OFF",                     0, STATE_OFF },
        { "OFF → ON on first button press",                1, STATE_ON },
        { "ON → BLINK_SLOW on second button press",        2, STATE_BLINK_SLOW },
        { "BLINK_SLOW → BLINK_FAST on third button press", 3, STATE_BLINK_FAST },
        { "BLINK_FAST → OFF on fourth button press",       4, STATE_OFF },
    };

    const int num_cases = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < num_cases; i++) {

        // ---------------------------------------------------------
        // Arrange
        // ---------------------------------------------------------
        sm_init();

        // Apply N button presses
        for (int p = 0; p < cases[i].num_presses; p++) {
            sm_handle_event(EVENT_BTN_PRESS);
        }

        // ---------------------------------------------------------
        // Act
        state_t actual = sm_get_state();

        // ---------------------------------------------------------
        // Assert
        TEST_ASSERT_EQUAL_MESSAGE(
            cases[i].expected_state,
            actual,
            cases[i].description
        );
    }
}


void test_blink_toggles_led_after_interval_parametrized(void)
{
    blink_case_t cases[] = {
        { STATE_BLINK_SLOW, 1000, 0, "BLINK_SLOW interval" },
        { STATE_BLINK_FAST,  250, 0, "BLINK_FAST interval" },
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
        fake_hal_set_tick(cases[i].tick);
        sm_tick();

        TEST_ASSERT_EQUAL_MESSAGE(
            1,
            fake_hal_get_toggle_count(),
            cases[i].description
        );
    }
}

void test_blink_timeout_behavior_parametrized(void)
{
    // ---------------------------------------------------------
    // Arrange: define test cases for both BLINK_SLOW and BLINK_FAST
    // ---------------------------------------------------------
    blink_case_t cases[] = {
        // BLINK_SLOW (interval = 1000 ms)
        { STATE_BLINK_SLOW,    0, 0, "BLINK_SLOW: Tick 0 → no toggle" },
        { STATE_BLINK_SLOW,  999, 0, "BLINK_SLOW: Tick 999 → no toggle" },
        { STATE_BLINK_SLOW, 1000, 1, "BLINK_SLOW: Tick 1000 → toggle once" },
        { STATE_BLINK_SLOW, 1500, 1, "BLINK_SLOW: Tick 1500 → still one toggle" },
        { STATE_BLINK_SLOW, 2000, 2, "BLINK_SLOW: Tick 2000 → toggle twice" },

        // BLINK_FAST (interval = 200 ms)
        { STATE_BLINK_FAST,    0, 0, "BLINK_FAST: Tick 0 → no toggle" },
        { STATE_BLINK_FAST,  199, 0, "BLINK_FAST: Tick 199 → no toggle" },
        { STATE_BLINK_FAST,  200, 1, "BLINK_FAST: Tick 200 → toggle once" },
        { STATE_BLINK_FAST,  350, 1, "BLINK_FAST: Tick 350 → still one toggle" },
        { STATE_BLINK_FAST,  400, 2, "BLINK_FAST: Tick 400 → toggle twice" },
    };

    const int num_cases = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < num_cases; i++) {

        // ---------------------------------------------------------
        // Arrange: reset system and enter correct blink mode
        // ---------------------------------------------------------
        sm_init();
        fake_hal_reset();

        // OFF → ON → BLINK_SLOW
        sm_handle_event(EVENT_BTN_PRESS);
        sm_handle_event(EVENT_BTN_PRESS);

        // BLINK_SLOW → BLINK_FAST (if needed)
        if (cases[i].target_state == STATE_BLINK_FAST) {
            sm_handle_event(EVENT_BTN_PRESS);
        }

        uint32_t interval = (cases[i].target_state == STATE_BLINK_SLOW) ? 1000 : 200;

        // ---------------------------------------------------------
        // Act: call sm_tick() once per interval boundary
        // ---------------------------------------------------------
        int ticks_to_process = cases[i].tick / interval;

        for (int t = 1; t <= ticks_to_process; t++) {
            fake_hal_set_tick(t * interval);
            sm_tick();
        }

        // Final call at the exact tick of the test case
        fake_hal_set_tick(cases[i].tick);
        sm_tick();

        // ---------------------------------------------------------
        // Assert
        // ---------------------------------------------------------
        TEST_ASSERT_EQUAL_MESSAGE(
            cases[i].expected_toggles,
            fake_hal_get_toggle_count(),
            cases[i].description
        );
    }
}

void test_timer_resets_on_state_change_parametrized(void)
{
    timer_reset_case_t cases[] = {
        { "OFF → ON resets timer",                     1, STATE_ON },
        { "ON → BLINK_SLOW resets timer",              2, STATE_BLINK_SLOW },
        { "BLINK_SLOW → BLINK_FAST resets timer",      3, STATE_BLINK_FAST },
        { "BLINK_FAST → OFF resets timer",             4, STATE_OFF },
    };

    const int num_cases = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < num_cases; i++) {

        sm_init();
        fake_hal_reset();
        fake_hal_set_tick(0);

        // Let the timer increment in the initial state
        sm_tick();
        TEST_ASSERT_TRUE(sm_get_timer() > 0);

        // Apply required button presses
        for (int p = 0; p < cases[i].num_presses_before_transition; p++) {
            sm_handle_event(EVENT_BTN_PRESS);
        }

        // Verify state
        TEST_ASSERT_EQUAL_MESSAGE(
            cases[i].expected_state_after_transition,
            sm_get_state(),
            cases[i].description
        );

        // Timer must be reset
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(
            0,
            sm_get_timer(),
            "Timer must reset on state transition"
        );

        // Timer must start counting again
        fake_hal_set_tick(100);
        sm_tick();

        TEST_ASSERT_EQUAL_UINT32_MESSAGE(
            1,
            sm_get_timer(),
            "Timer must restart from 0 after transition"
        );
    }
}
