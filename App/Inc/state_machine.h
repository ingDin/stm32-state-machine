#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    STATE_OFF,
    STATE_ON,
    STATE_BLINK_SLOW,
    STATE_BLINK_FAST,
    STATE_COUNT
} state_t;

typedef enum {
    EVENT_BTN_PRESS,
    EVENT_TIMEOUT,
    EVENT_BTN_RELEASE,
    EVENT_NONE
} event_t;

typedef bool (*guard_fn_t)(void);
typedef void (*action_fn_t)(void);

typedef struct {
    state_t from;
    event_t event;
    guard_fn_t guard;
    state_t to;
} transition_t;

void sm_init(void);
void sm_handle_event(event_t event);
state_t sm_get_state(void);
void sm_tick(void);
uint32_t sm_get_timer(void);

#endif
