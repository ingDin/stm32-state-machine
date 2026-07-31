/**
 * @file test_app.c
 * @brief Tests debounced button events generated through app_tick().
 *
 * Validates press and release event generation using parameterized ISR
 * sequences and debounce timing.
 */

#include "unity.h"
#include "app.h"
#include "fake_hal.h"
#include "fake_fsm.h"
#include "fake_button_isr.h"
#include "button.h"
#include "test_app.h"
#include "state_machine.h"

// A single parameter structure used for both PRESS and RELEASE tests.
// sequence[] contains: ISR level + 3 debounce ticks.
typedef struct
{
    int sequence[4];
    event_t expected_event;
} button_test_t;

// ---------------------------------------------------------
// Parameterized test: EVENT_BTN_PRESS (stable_state 0 -> raw_level 1)
// Requires: ISR + 3 debounce ticks
// ---------------------------------------------------------
void test_app_tick_press_parameterized(void)
{
    button_test_t tests[] = {
        {.sequence = {1, 1, 1, 1}, .expected_event = EVENT_BTN_PRESS},
        {.sequence = {1, 1, 1, 1}, .expected_event = EVENT_BTN_PRESS},
        {.sequence = {1, 1, 1, 1}, .expected_event = EVENT_BTN_PRESS}};

    const int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        fake_hal_reset();
        fake_fsm_reset();
        button_init();
        sm_init();

        // ISR + 3 debounce ticks
        for (int t = 0; t < 4; t++)
        {
            fake_button_isr(tests[i].sequence[t]);
            fake_hal_set_tick(t);
            app_tick();
        }

        TEST_ASSERT_EQUAL(tests[i].expected_event, fake_fsm_last_event());
    }
}

// ---------------------------------------------------------
// Parameterized test: EVENT_BTN_RELEASE (stable_state 1 -> raw_level 0)
// Requires: PRESS first, then ISR + 3 debounce ticks
// ---------------------------------------------------------
void test_app_tick_release_parameterized(void)
{
    button_test_t tests[] = {
        {.sequence = {0, 0, 0, 0}, .expected_event = EVENT_BTN_RELEASE},
        {.sequence = {0, 0, 0, 0}, .expected_event = EVENT_BTN_RELEASE},
        {.sequence = {0, 0, 0, 0}, .expected_event = EVENT_BTN_RELEASE}};

    const int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        fake_hal_reset();
        fake_fsm_reset();
        button_init();
        sm_init();

        // First simulate a PRESS so stable_state becomes 1
        fake_button_isr(1);
        fake_hal_set_tick(0);
        app_tick();
        fake_hal_set_tick(1);
        app_tick();
        fake_hal_set_tick(2);
        app_tick();

        // Now test RELEASE: ISR + 3 debounce ticks
        for (int t = 0; t < 4; t++)
        {
            fake_button_isr(tests[i].sequence[t]);
            fake_hal_set_tick(3 + t);
            app_tick();
        }

        TEST_ASSERT_EQUAL(tests[i].expected_event, fake_fsm_last_event());
    }
}

// ---------------------------------------------------------
// Parameterized test: app_init() resets HAL and button
// ---------------------------------------------------------
void test_app_init_parameterized(void)
{
    // Dirty initial state
    hal_get_tick();        // just to touch HAL, if needed
    fake_button_isr(1); // raw_level = 1, button "dirty"

    app_init();

    // HAL tick should be reset by fake_hal_reset() behind hal_get_tick
    TEST_ASSERT_EQUAL_UINT32(0, hal_get_tick());

    // Button should be in a clean, not-pressed state after button_init()
    TEST_ASSERT_FALSE(button_is_pressed());
}

// ---------------------------------------------------------
// Parameterized test: No event when raw level is stable (no change)
// ---------------------------------------------------------
void test_app_tick_no_event_parameterized(void)
{
    const int sequences[][5] = {
        {0, 0, 0, 0, 0}, // stable 0 → no event
        {1, 1, 1, 1, 1}, // stable 1 → no event (after press)
        {0, 0, 0, 0, 0}  // stable 0 → no event
    };

    const int num_tests = sizeof(sequences) / sizeof(sequences[0]);

    for (int i = 0; i < num_tests; i++)
    {
        app_init();

        // If sequence is stable 1, generate a PRESS first
        if (sequences[i][0] == 1)
        {
            fake_button_isr(1);
            fake_hal_set_tick(0);
            app_tick();
            fake_hal_set_tick(1);
            app_tick();
            fake_hal_set_tick(2);
            app_tick();
            fake_hal_set_tick(3);
            app_tick(); // EVENT_BTN_PRESS

            fake_fsm_reset(); // clear event
        }

        // Now run the stable sequence
        for (int t = 0; t < 5; t++)
        {
            fake_button_isr(sequences[i][t]);
            fake_hal_set_tick(10 + t);
            app_tick();
        }

        TEST_ASSERT_EQUAL(EVENT_NONE, fake_fsm_last_event());
    }
}

// ---------------------------------------------------------
// Parameterized test: Bounce resets debounce counter (no event)
// ---------------------------------------------------------
void test_app_tick_bounce_parameterized(void)
{
    const int sequences[][3] = {
        {1, 0, 1},
        {0, 1, 0},
        {1, 0, 1}};

    const int num_tests = sizeof(sequences) / sizeof(sequences[0]);

    for (int i = 0; i < num_tests; i++)
    {
        app_init();

        for (int t = 0; t < 3; t++)
        {
            fake_button_isr(sequences[i][t]);
            fake_hal_set_tick(t);
            app_tick();
        }

        TEST_ASSERT_EQUAL(EVENT_NONE, fake_fsm_last_event());
    }
}

// ---------------------------------------------------------
// Parameterized test: Stable but already pressed → no new event
// ---------------------------------------------------------
void test_app_tick_stable_no_new_event_parameterized(void)
{
    const int sequences[][5] = {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1}};

    const int num_tests = sizeof(sequences) / sizeof(sequences[0]);

    for (int i = 0; i < num_tests; i++)
    {
        app_init();

        // First generate a PRESS
        fake_button_isr(1);
        fake_hal_set_tick(0);
        app_tick();
        fake_hal_set_tick(1);
        app_tick();
        fake_hal_set_tick(2);
        app_tick();
        fake_hal_set_tick(3);
        app_tick(); // EVENT_BTN_PRESS

        fake_fsm_reset(); // clear event

        // Now stable 1 → no new event
        for (int t = 0; t < 5; t++)
        {
            fake_button_isr(sequences[i][t]);
            fake_hal_set_tick(10 + t);
            app_tick();
        }

        TEST_ASSERT_EQUAL(EVENT_NONE, fake_fsm_last_event());
    }
}
