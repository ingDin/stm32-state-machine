#include "../unity/unity.h"
#include "../../Core/Inc/state_machine.h"

void setUp(void) {}
void tearDown(void) {}

void test_initial_state_is_OFF(void) {
    sm_init();
    TEST_ASSERT_EQUAL(STATE_OFF, sm_get_state());
}

void test_off_to_on_on_button_press(void) {
    sm_init();
    sm_handle_event(EVENT_BTN_PRESS);
    TEST_ASSERT_EQUAL(STATE_ON, sm_get_state());
}

void test_on_to_blink_slow_on_button_press(void) {
    sm_init();
    sm_handle_event(EVENT_BTN_PRESS); // OFF → ON
    sm_handle_event(EVENT_BTN_PRESS); // ON → BLINK_SLOW
    TEST_ASSERT_EQUAL(STATE_BLINK_SLOW, sm_get_state());
}
