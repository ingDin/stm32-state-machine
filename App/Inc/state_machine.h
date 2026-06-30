/**
 * @file state_machine.h
 * @brief Public API for the LED state machine.
 *
 * Defines the FSM states, events, transition structure and the
 * functions used to initialize, update and query the state machine.
 *
 * State flow:
 *   OFF → ON → BLINK_SLOW → BLINK_FAST → OFF
 *
 * The FSM exposes:
 *   - sm_init(): reset and enter initial state
 *   - sm_handle_event(): process external events
 *   - sm_update(): run per‑state actions
 *   - sm_tick(): increment internal timer + actions
 *   - sm_get_state(): read current state
 *   - sm_get_timer(): read internal timer
 */


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
