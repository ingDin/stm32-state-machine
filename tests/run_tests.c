#include "unity/unity.h"

// Declare the tests implemented in test_state_machine.c
void test_initial_state_is_OFF(void);

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_initial_state_is_OFF);

    return UNITY_END();
}
