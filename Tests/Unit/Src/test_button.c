/**
 * @file test_button.c
 * @brief Parameterized tests for debounced button events and state.
 *
 * Verifies press, release, and stable pressed-state behavior using raw
 * input sequences processed through ISR and debounce logic.
 */

#include "unity.h"
#include "button.h"
#include "test_button.h"
#include "fake_fsm.h"

typedef struct
{
    const char *name;
    int raw_sequence[10];
    int seq_len;
    event_t expected_event;
    int expected_pressed;
} button_test_case_t;

void setUp(void){}
void tearDown(void){}

static void run_sequence(const int *seq, int len)
{
    // ACT helper: feed raw ISR values + tick processing
    for (int i = 0; i < len; i++)
    {
        button_isr_handler(seq[i]);
        button_tick();
    }
}

// ---------------------------------------------------------
// Parameterized test: button press scenarios (AAA pattern)
// ---------------------------------------------------------

void test_button_press_scenarios_parameterized(void)
{
    button_test_case_t cases[] = {
        { .name = "Simple press after debounce",
          .raw_sequence = {1, 1, 1},
          .seq_len = 3,
          .expected_event = EVENT_BTN_PRESS,
          .expected_pressed = 1 },

        { .name = "Bouncing before stable press",
          .raw_sequence = {1, 0, 1, 1, 1},
          .seq_len = 5,
          .expected_event = EVENT_BTN_PRESS,
          .expected_pressed = 1 },

        { .name = "Not enough ticks → no press",
          .raw_sequence = {1, 1},
          .seq_len = 2,
          .expected_event = EVENT_NONE,
          .expected_pressed = 0 }
    };

    for (unsigned i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
    {
        // ARRANGE: reset button + FSM state
        button_init();
        fake_fsm_reset();

        // ACT: feed raw sequence through ISR + tick
        run_sequence(cases[i].raw_sequence, cases[i].seq_len);

        // ASSERT: check event + pressed state
        TEST_ASSERT_EQUAL_MESSAGE(
            cases[i].expected_event,
            fake_fsm_last_event(),
            cases[i].name);

        TEST_ASSERT_EQUAL_MESSAGE(
            cases[i].expected_pressed,
            button_is_pressed(),
            cases[i].name);
    }
}

// ---------------------------------------------------------
// Parameterized test: button release scenarios (AAA pattern)
// ---------------------------------------------------------

void test_button_release_scenarios_parameterized(void)
{
    button_test_case_t cases[] = {
        { .name = "Simple release after stable press",
          .raw_sequence = {1, 1, 1, 0, 0, 0},
          .seq_len = 6,
          .expected_event = EVENT_BTN_RELEASE,
          .expected_pressed = 0 },

        { .name = "Bouncing before stable release",
          .raw_sequence = {1, 1, 1, 0, 1, 0, 0, 0},
          .seq_len = 8,
          .expected_event = EVENT_BTN_RELEASE,
          .expected_pressed = 0 },

        { .name = "Not enough ticks → no release",
          .raw_sequence = {1, 1, 1, 0, 0},
          .seq_len = 5,
          .expected_event = EVENT_BTN_PRESS,
          .expected_pressed = 1 }
    };

    for (unsigned i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
    {
        // ARRANGE: reset button + FSM state
        button_init();
        fake_fsm_reset();

        // ACT: feed raw sequence
        run_sequence(cases[i].raw_sequence, cases[i].seq_len);

        // ASSERT: check event + pressed state
        TEST_ASSERT_EQUAL_MESSAGE(
            cases[i].expected_event,
            fake_fsm_last_event(),
            cases[i].name);

        TEST_ASSERT_EQUAL_MESSAGE(
            cases[i].expected_pressed,
            button_is_pressed(),
            cases[i].name);
    }
}

// ---------------------------------------------------------
// Parameterized test: button_is_pressed() scenarios (AAA pattern)
// ---------------------------------------------------------

void test_button_is_pressed_scenarios_parameterized(void)
{
    button_test_case_t cases[] = {
        { .name = "Never pressed",
          .raw_sequence = {0, 0, 0},
          .seq_len = 3,
          .expected_event = EVENT_NONE,
          .expected_pressed = 0 },

        { .name = "Pressed and stable",
          .raw_sequence = {1, 1, 1},
          .seq_len = 3,
          .expected_event = EVENT_BTN_PRESS,
          .expected_pressed = 1 },

        { .name = "Press then release",
          .raw_sequence = {1, 1, 1, 0, 0, 0},
          .seq_len = 6,
          .expected_event = EVENT_BTN_RELEASE,
          .expected_pressed = 0 }
    };

    for (unsigned i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
    {
        // ARRANGE: reset button + FSM state
        button_init();
        fake_fsm_reset();

        // ACT: feed raw sequence
        run_sequence(cases[i].raw_sequence, cases[i].seq_len);

        // ASSERT: only check pressed state here
        TEST_ASSERT_EQUAL_MESSAGE(
            cases[i].expected_pressed,
            button_is_pressed(),
            cases[i].name);
    }
}
