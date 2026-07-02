#ifndef TEST_BUTTON_H
#define TEST_BUTTON_H

#include "fake_fsm.h"

typedef struct {
    const char* name;
    int raw_sequence[10];
    int seq_len;
    event_t expected_event;
    int expected_pressed;
} button_test_case_t;

void test_button_press_scenarios_parametrized(void);
void test_button_release_scenarios_parametrized(void);
void test_button_is_pressed_scenarios_parametrized(void);

#endif
