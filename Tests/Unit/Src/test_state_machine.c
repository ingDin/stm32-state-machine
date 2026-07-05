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

#include "unity.h"
#include "state_machine.h"
#include "fake_hal.h"
#include "fake_tguard.h"
#include "test_state_machine.h"

typedef struct
{
    const char *description;
    int num_presses;
    state_t expected_state;
} state_transition_case_t;

typedef struct
{
    state_t target_state;
    uint32_t tick;
    int expected_toggles;
    const char *description;
} blink_case_t;

typedef struct
{
    const char *description;
    int num_presses_before_transition;
    state_t expected_state_after_transition;
} timer_reset_case_t;

typedef struct
{
    const char *description;
    state_t initial_state;
    event_t event_without_transition;
} no_transition_case_t;

typedef struct
{
    bool guard_value;
    state_t expected_state;
    const char *description;
} guard_case_t;

void test_state_transitions_parameterized(void)
{
    // ---------------------------------------------------------
    // Arrange: define all state transition scenarios
    // ---------------------------------------------------------
    state_transition_case_t cases[] = {
        {"Initial state must be OFF", 0, STATE_OFF},
        {"OFF → ON on first button press", 1, STATE_ON},
        {"ON → BLINK_SLOW on second button press", 2, STATE_BLINK_SLOW},
        {"BLINK_SLOW → BLINK_FAST on third button press", 3, STATE_BLINK_FAST},
        {"BLINK_FAST → OFF on fourth button press", 4, STATE_OFF},
    };

    const int num_cases = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < num_cases; i++)
    {
        // ---------------------------------------------------------
        // Arrange: reset state machine
        // ---------------------------------------------------------
        sm_init();

        // Apply N button presses
        for (int p = 0; p < cases[i].num_presses; p++)
        {
            sm_handle_event(EVENT_BTN_PRESS);
        }

        // ---------------------------------------------------------
        // Act: read current state
        // ---------------------------------------------------------
        state_t actual = sm_get_state();

        // ---------------------------------------------------------
        // Assert: verify expected state
        // ---------------------------------------------------------
        TEST_ASSERT_EQUAL_MESSAGE(
            cases[i].expected_state,
            actual,
            cases[i].description);
    }
}

void test_blink_toggles_led_after_interval_parameterized(void)
{
    // ---------------------------------------------------------
    // Arrange: define blink interval test cases
    // ---------------------------------------------------------
    blink_case_t cases[] = {
        {STATE_BLINK_SLOW, 1000, 0, "BLINK_SLOW interval"},
        {STATE_BLINK_FAST, 250, 0, "BLINK_FAST interval"},
    };

    const int num_cases = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < num_cases; i++)
    {
        // ---------------------------------------------------------
        // Arrange: full reset of state machine + HAL
        // ---------------------------------------------------------
        sm_init();
        fake_hal_reset();
        fake_hal_set_tick(0);

        // Enter BLINK_SLOW
        sm_handle_event(EVENT_BTN_PRESS); // OFF → ON
        sm_handle_event(EVENT_BTN_PRESS); // ON → BLINK_SLOW

        // Enter BLINK_FAST if needed
        if (cases[i].target_state == STATE_BLINK_FAST)
        {
            sm_handle_event(EVENT_BTN_PRESS);
        }

        // ---------------------------------------------------------
        // Act: before interval → no toggle expected
        // ---------------------------------------------------------
        sm_tick();
        TEST_ASSERT_EQUAL_MESSAGE(
            0,
            fake_hal_get_toggle_count(),
            "LED must NOT toggle before interval");

        // ---------------------------------------------------------
        // Act: at interval → toggle expected
        // ---------------------------------------------------------
        fake_hal_set_tick(cases[i].tick);
        sm_tick();

        // ---------------------------------------------------------
        // Assert
        // ---------------------------------------------------------
        TEST_ASSERT_EQUAL_MESSAGE(
            1,
            fake_hal_get_toggle_count(),
            cases[i].description);
    }
}

void test_blink_timeout_behavior_parameterized(void)
{
    // ---------------------------------------------------------
    // Arrange: define detailed blink timeout scenarios
    // ---------------------------------------------------------
    blink_case_t cases[] = {
        // BLINK_SLOW (interval = 1000 ms)
        {STATE_BLINK_SLOW, 0,    0, "BLINK_SLOW: Tick 0 → no toggle"},
        {STATE_BLINK_SLOW, 999,  0, "BLINK_SLOW: Tick 999 → no toggle"},
        {STATE_BLINK_SLOW, 1000, 1, "BLINK_SLOW: Tick 1000 → toggle once"},
        {STATE_BLINK_SLOW, 1500, 1, "BLINK_SLOW: Tick 1500 → still one toggle"},
        {STATE_BLINK_SLOW, 2000, 2, "BLINK_SLOW: Tick 2000 → toggle twice"},

        // BLINK_FAST (interval = 200 ms)
        {STATE_BLINK_FAST, 0,    0, "BLINK_FAST: Tick 0 → no toggle"},
        {STATE_BLINK_FAST, 199,  0, "BLINK_FAST: Tick 199 → no toggle"},
        {STATE_BLINK_FAST, 200,  1, "BLINK_FAST: Tick 200 → toggle once"},
        {STATE_BLINK_FAST, 350,  1, "BLINK_FAST: Tick 350 → still one toggle"},
        {STATE_BLINK_FAST, 400,  2, "BLINK_FAST: Tick 400 → toggle twice"},
    };

    const int num_cases = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < num_cases; i++)
    {
        // ---------------------------------------------------------
        // Arrange: reset system and enter correct blink mode
        // ---------------------------------------------------------
        sm_init();
        fake_hal_reset();

        // OFF → ON → BLINK_SLOW
        sm_handle_event(EVENT_BTN_PRESS);
        sm_handle_event(EVENT_BTN_PRESS);

        // BLINK_SLOW → BLINK_FAST if needed
        if (cases[i].target_state == STATE_BLINK_FAST)
        {
            sm_handle_event(EVENT_BTN_PRESS);
        }

        uint32_t interval =
            (cases[i].target_state == STATE_BLINK_SLOW) ? 1000 : 200;

        // ---------------------------------------------------------
        // Act: process ticks up to the test case tick
        // ---------------------------------------------------------
        int ticks_to_process = cases[i].tick / interval;

        for (int t = 1; t <= ticks_to_process; t++)
        {
            fake_hal_set_tick(t * interval);
            sm_tick();
        }

        // Final tick
        fake_hal_set_tick(cases[i].tick);
        sm_tick();

        // ---------------------------------------------------------
        // Assert: verify toggle count
        // ---------------------------------------------------------
        TEST_ASSERT_EQUAL_MESSAGE(
            cases[i].expected_toggles,
            fake_hal_get_toggle_count(),
            cases[i].description);
    }
}

void test_timer_resets_on_state_change_parameterized(void)
{
    // ---------------------------------------------------------
    // Arrange: define all timer-reset scenarios
    // ---------------------------------------------------------
    timer_reset_case_t cases[] = {
        {"OFF → ON resets timer", 1, STATE_ON},
        {"ON → BLINK_SLOW resets timer", 2, STATE_BLINK_SLOW},
        {"BLINK_SLOW → BLINK_FAST resets timer", 3, STATE_BLINK_FAST},
        {"BLINK_FAST → OFF resets timer", 4, STATE_OFF},
    };

    const int num_cases = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < num_cases; i++)
    {
        // ---------------------------------------------------------
        // Arrange: reset FSM + HAL
        // ---------------------------------------------------------
        sm_init();
        fake_hal_reset();
        fake_hal_set_tick(0);

        // Let the timer increment in the initial state
        sm_tick();
        TEST_ASSERT_TRUE(sm_get_timer() > 0);

        // Apply required button presses to reach target state
        for (int p = 0; p < cases[i].num_presses_before_transition; p++)
        {
            sm_handle_event(EVENT_BTN_PRESS);
        }

        // ---------------------------------------------------------
        // Assert: verify state after transition
        // ---------------------------------------------------------
        TEST_ASSERT_EQUAL_MESSAGE(
            cases[i].expected_state_after_transition,
            sm_get_state(),
            cases[i].description);

        // Timer must be reset on transition
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(
            0,
            sm_get_timer(),
            "Timer must reset on state transition");

        // ---------------------------------------------------------
        // Act: allow timer to tick again
        // ---------------------------------------------------------
        fake_hal_set_tick(100);
        sm_tick();

        // ---------------------------------------------------------
        // Assert: timer restarted from zero
        // ---------------------------------------------------------
        TEST_ASSERT_EQUAL_UINT32_MESSAGE(
            1,
            sm_get_timer(),
            "Timer must restart from 0 after transition");
    }
}

void test_next_state_returns_current_when_no_transition_parameterized(void)
{
    // ---------------------------------------------------------
    // Arrange: define scenarios where no transition should occur
    // ---------------------------------------------------------
    no_transition_case_t cases[] = {
        {"OFF + EVENT_TIMEOUT → remain OFF", STATE_OFF, EVENT_TIMEOUT},
        {"ON + EVENT_TIMEOUT → remain ON", STATE_ON, EVENT_TIMEOUT},
        {"BLINK_SLOW + EVENT_TIMEOUT → remain BLINK_SLOW", STATE_BLINK_SLOW, EVENT_TIMEOUT},
        {"BLINK_FAST + EVENT_TIMEOUT → remain BLINK_FAST", STATE_BLINK_FAST, EVENT_TIMEOUT},

        {"OFF + EVENT_BTN_RELEASE → remain OFF", STATE_OFF, EVENT_BTN_RELEASE},
        {"ON + EVENT_BTN_RELEASE → remain ON", STATE_ON, EVENT_BTN_RELEASE},
        {"BLINK_SLOW + EVENT_BTN_RELEASE → remain BLINK_SLOW", STATE_BLINK_SLOW, EVENT_BTN_RELEASE},
        {"BLINK_FAST + EVENT_BTN_RELEASE → remain BLINK_FAST", STATE_BLINK_FAST, EVENT_BTN_RELEASE}
    };

    const int num_cases = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < num_cases; i++)
    {
        // ---------------------------------------------------------
        // Arrange: reset FSM and move it into the desired initial state
        // ---------------------------------------------------------
        sm_init();

        switch (cases[i].initial_state)
        {
        case STATE_ON:
            sm_handle_event(EVENT_BTN_PRESS);
            break;

        case STATE_BLINK_SLOW:
            sm_handle_event(EVENT_BTN_PRESS);
            sm_handle_event(EVENT_BTN_PRESS);
            break;

        case STATE_BLINK_FAST:
            sm_handle_event(EVENT_BTN_PRESS);
            sm_handle_event(EVENT_BTN_PRESS);
            sm_handle_event(EVENT_BTN_PRESS);
            break;

        default:
            break; // STATE_OFF already set by sm_init()
        }

        TEST_ASSERT_EQUAL_MESSAGE(
            cases[i].initial_state,
            sm_get_state(),
            "FSM must be in the expected initial state");

        // ---------------------------------------------------------
        // Act: trigger an event that should NOT cause a transition
        // ---------------------------------------------------------
        sm_handle_event(cases[i].event_without_transition);

        // ---------------------------------------------------------
        // Assert: state must remain unchanged
        // ---------------------------------------------------------
        TEST_ASSERT_EQUAL_MESSAGE(
            cases[i].initial_state,
            sm_get_state(),
            cases[i].description);
    }
}

void test_guard_always_true_parameterized(void)
{
    // ---------------------------------------------------------
    // Arrange: define guard evaluation scenarios
    // ---------------------------------------------------------
    guard_case_t cases[] = {
        {false, STATE_OFF, "guard FALSE → remain OFF"},
        {true, STATE_ON, "guard TRUE → transition to ON"}
    };

    int num_cases = sizeof(cases) / sizeof(cases[0]);

    for (int i = 0; i < num_cases; i++)
    {
        // ---------------------------------------------------------
        // Arrange: reset FSM + guard
        // ---------------------------------------------------------
        sm_init();
        fake_tguard_reset();
        fake_tguard_set(cases[i].guard_value);

        // ---------------------------------------------------------
        // Act: trigger button press
        // ---------------------------------------------------------
        sm_handle_event(EVENT_BTN_PRESS);

        // ---------------------------------------------------------
        // Assert: verify resulting state
        // ---------------------------------------------------------
        TEST_ASSERT_EQUAL_MESSAGE(
            cases[i].expected_state,
            sm_get_state(),
            cases[i].description);
    }
}
