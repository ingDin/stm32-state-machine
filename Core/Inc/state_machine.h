#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum {
    STATE_OFF,
    STATE_ON,
    STATE_BLINK_SLOW,
    STATE_BLINK_FAST
} state_t;

typedef enum {
    EVENT_BTN_PRESS
} event_t;

void sm_init(void);
state_t sm_get_state(void);
void sm_handle_event(event_t event);

#endif
