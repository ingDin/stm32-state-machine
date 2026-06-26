#include "unity/unity.h"

void test_state_transitions_parametrized(void);
void test_blink_timeout_behavior_parametrized(void);

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_state_transitions_parametrized);
    RUN_TEST(test_blink_timeout_behavior_parametrized);

    return UNITY_END();
}
