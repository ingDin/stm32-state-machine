#include "unity.h"
#include <string.h>
#include <stdio.h>
#include "test_led.h"
#include "test_button.h"
#include "test_state_machine.h"
#include "test_app.h"

typedef void (*test_fn_t)(void);

typedef struct
{
    const char *match;
    test_fn_t fn;
} test_entry_t;

static const test_entry_t tests[] = {
    {"test_state_transitions", test_state_transitions_parameterized},
    {"test_blink_timeout", test_blink_timeout_behavior_parameterized},
    {"test_timer_resets", test_timer_resets_on_state_change_parameterized},
    {"test_next_state_returns", test_next_state_returns_current_when_no_transition_parameterized},
    {"test_guard_always_true", test_guard_always_true_parameterized},
    {"test_button_press", test_button_press_scenarios_parameterized},
    {"test_button_release", test_button_release_scenarios_parameterized},
    {"test_button_is_pressed", test_button_is_pressed_scenarios_parameterized},
    {"test_led_init", test_led_init_parameterized},
    {"test_led_tick", test_led_tick_parameterized},
    {"test_app_tick_press", test_app_tick_press_parameterized},
    {"test_app_tick_release", test_app_tick_release_parameterized},
    {"test_app_init", test_app_init_parameterized},
    {"test_app_tick_no_event", test_app_tick_no_event_parameterized},
    {"test_app_tick_bounce", test_app_tick_bounce_parameterized},
    {"test_app_tick_stable_no_new_event", test_app_tick_stable_no_new_event_parameterized},
};


int main(int argc, char **argv)
{
    UNITY_BEGIN();

    const char *exe = argv[0];

    // Run the test that matches the executable name
    for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++)
    {
        if (strstr(exe, tests[i].match) != NULL)
        {
            RUN_TEST(tests[i].fn);
            return UNITY_END();
        }
    }

    printf("ERROR: Unknown test executable: %s\n", exe);
    return UNITY_END();
}
