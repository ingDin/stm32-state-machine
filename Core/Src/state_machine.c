#include "state_machine.h"
#include <stdint.h>
extern uint32_t HAL_GetTick(void);

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
        led_state = !led_state;
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
