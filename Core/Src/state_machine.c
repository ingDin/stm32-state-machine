#include "state_machine.h"

static state_t current_state;

void sm_init(void) {
    current_state = STATE_OFF;
}

state_t sm_get_state(void) {
    return current_state;
}
