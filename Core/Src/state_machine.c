/**
 * @file state_machine.c
 * @brief LED control state machine with time‑driven and event‑driven behavior.
 *
 * This module implements a simple finite state machine (FSM) for controlling an
 * LED on an embedded system. The FSM supports four states:
 *
 *   - STATE_OFF        : LED is off
 *   - STATE_ON         : LED is on
 *   - STATE_BLINK_SLOW : LED toggles at a slow interval
 *   - STATE_BLINK_FAST : LED toggles at a fast interval
 *
 * The FSM reacts to:
 *   - time progression (via sm_tick)
 *   - external events (via sm_handle_event)
 *
 * The design separates:
 *   - event‑driven transitions (button presses)
 *   - time‑driven actions (blink intervals)
 *
 * Hardware dependencies:
 *   - HAL_GetTick()            : provides system time in ms
 *   - HAL_GPIO_TogglePin()     : toggles the LED pin
 *
 * These functions are declared as extern so the module can be tested using
 * fake HAL implementations without modifying production code.
 *
 * This architecture enables deterministic unit testing using a fake HAL,
 * while keeping the production code clean and hardware‑agnostic.
 */
 
#include "state_machine.h"
#include <stdint.h>
#include <stdbool.h>

// HAL dependencies (real in firmware, fake in tests)
extern uint32_t HAL_GetTick(void);
extern void HAL_GPIO_TogglePin(void* GPIOx, uint16_t GPIO_Pin);

// ---------------------------------------------------------
// INTERNAL STATE
// ---------------------------------------------------------
static state_t current_state;
static uint32_t last_toggle_time = 0;
static bool led_state = false;

// ---------------------------------------------------------
// GUARDS
// ---------------------------------------------------------
static bool guard_always_true(void) { return true; }

// ---------------------------------------------------------
// ACTIONS
// ---------------------------------------------------------
static void action_off(void) {
    led_state = false;
}

static void action_on(void) {
    led_state = true;
}

static bool timer_expired(uint32_t interval) {
    return (HAL_GetTick() - last_toggle_time) >= interval;
}

static void action_blink(uint32_t interval) {
    if (timer_expired(interval)) {
        HAL_GPIO_TogglePin(NULL, 0);
        last_toggle_time = HAL_GetTick();
    }
}

static void action_blink_slow(void)  { action_blink(1000); }
static void action_blink_fast(void)  { action_blink(200); }

// ---------------------------------------------------------
// ACTION TABLE
// ---------------------------------------------------------
static action_fn_t actions[STATE_COUNT] = {
    [STATE_OFF]        = action_off,
    [STATE_ON]         = action_on,
    [STATE_BLINK_SLOW] = action_blink_slow,
    [STATE_BLINK_FAST] = action_blink_fast
};

// ---------------------------------------------------------
// TRANSITION TABLE
// ---------------------------------------------------------
static const transition_t transitions[] = {
    { STATE_OFF,        EVENT_BTN_PRESS, guard_always_true, STATE_ON },
    { STATE_ON,         EVENT_BTN_PRESS, guard_always_true, STATE_BLINK_SLOW },
    { STATE_BLINK_SLOW, EVENT_BTN_PRESS, guard_always_true, STATE_BLINK_FAST },
    { STATE_BLINK_FAST, EVENT_BTN_PRESS, guard_always_true, STATE_OFF }
};

static const int transition_count =
    sizeof(transitions) / sizeof(transitions[0]);

// ---------------------------------------------------------
// GENERIC NEXT-STATE FUNCTION
// ---------------------------------------------------------
static state_t next_state(state_t current, event_t event) {
    for (int i = 0; i < transition_count; i++) {
        const transition_t *t = &transitions[i];
        if (t->from == current && t->event == event && t->guard()) {
            return t->to;
        }
    }
    return current;
}

// ---------------------------------------------------------
// PUBLIC API
// ---------------------------------------------------------
void sm_init(void) {
    current_state = STATE_OFF;
    last_toggle_time = HAL_GetTick();
    led_state = false;
}

void sm_handle_event(event_t event) {
    state_t new_state = next_state(current_state, event);
    if (new_state != current_state) {
        current_state = new_state;
        last_toggle_time = HAL_GetTick();
    }
}

state_t sm_get_state(void) {
    return current_state;
}

void sm_update(void) {
    actions[current_state]();
}

void sm_tick(void) {
    actions[current_state]();
}
