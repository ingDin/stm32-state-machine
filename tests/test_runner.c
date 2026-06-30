#include "unity.h"
#include <string.h>
#include <stdio.h>

void test_state_transitions_parametrized(void);
void test_blink_timeout_behavior_parametrized(void);
void test_timer_resets_on_state_change_parametrized(void);
void test_next_state_returns_current_when_no_transition_parametrized(void);
void test_guard_always_true_parametrized(void);

typedef void (*test_fn_t)(void);

typedef struct {
    const char *match;
    test_fn_t fn;
} test_entry_t;

static const test_entry_t tests[] = {
    { "test_state_transitions",  test_state_transitions_parametrized },
    { "test_blink_timeout",      test_blink_timeout_behavior_parametrized },
    { "test_timer_resets",       test_timer_resets_on_state_change_parametrized },
    { "test_next_state_returns", test_next_state_returns_current_when_no_transition_parametrized },
    { "test_guard_always_true",  test_guard_always_true_parametrized }
};

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    const char *exe = argv[0];

    // // DEBUG mode → run only ONE test
    // if (argc == 2 && strcmp(argv[1], "debug") == 0) {
    //     for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
    //         if (strstr(exe, tests[i].match) != NULL) {
    //             RUN_TEST(tests[i].fn);
    //             return UNITY_END();
    //         }
    //     }
    //     printf("ERROR: Unknown test executable: %s\n", exe);
    //     return UNITY_END();
    // }

    // NORMAL mode → run ALL tests
    for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        RUN_TEST(tests[i].fn);
    }

    return UNITY_END();
}
