#include "state_machine.h"

static state_t current_state;

void sm_init(void) {
    current_state = STATE_OFF;
}

state_t sm_get_state(void) {
    return current_state;
}

void sm_handle_event(event_t event) {
    if (current_state == STATE_OFF && event == EVENT_BTN_PRESS) {
        current_state = STATE_ON;
    }
    else if (current_state == STATE_ON && event == EVENT_BTN_PRESS) {
        current_state = STATE_BLINK_SLOW;
    }
}
