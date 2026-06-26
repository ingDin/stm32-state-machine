#include "unity/unity.h"

void test_initial_state_is_OFF(void);
void test_off_to_on_on_button_press(void);
void test_on_to_blink_slow_on_button_press(void);
void test_blink_slow_to_blink_fast_on_button_press(void);
void test_blink_fast_to_off_on_button_press(void);
void test_blink_toggles_led_after_interval_parametrized(void);
void test_blink_timeout_behavior_parametrized(void);

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_initial_state_is_OFF);
    RUN_TEST(test_off_to_on_on_button_press);
    RUN_TEST(test_on_to_blink_slow_on_button_press);
    RUN_TEST(test_blink_slow_to_blink_fast_on_button_press);
    RUN_TEST(test_blink_fast_to_off_on_button_press);
    RUN_TEST(test_blink_toggles_led_after_interval_parametrized);
    RUN_TEST(test_blink_timeout_behavior_parametrized);

    return UNITY_END();
}
