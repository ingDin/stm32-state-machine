#include "unity/unity.h"

// Declare the tests implemented in test_state_machine.c
void test_framework_runs(void);

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_framework_runs);

    return UNITY_END();
}
