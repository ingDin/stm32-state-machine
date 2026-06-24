#include "unity/unity.h"

void test_initial_state_is_OFF(void);
void test_off_to_on_on_button_press(void);
void test_on_to_blink_slow_on_button_press(void);

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_initial_state_is_OFF);
    RUN_TEST(test_off_to_on_on_button_press);
    RUN_TEST(test_on_to_blink_slow_on_button_press);

    return UNITY_END();
}
