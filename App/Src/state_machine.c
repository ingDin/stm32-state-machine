/**
 * @file state_machine.c
 * @brief Finite state machine implementation for LED control and CAN reporting.
 *
 * This module implements the runtime behavior of the LED FSM. It evaluates
 * transitions, executes entry/exit/state actions, maintains an internal
 * state timer, and sends a CAN frame whenever a state transition occurs.
 * LED behavior is delegated to the LED driver, while CAN reporting uses
 * the generic CAN interface (loopback in TEST mode).
 */

#include "state_machine.h"
#include "hal_wrapper.h"
#include "guards.h"
#include "led.h"
#include "can_if.h"
#include "can_loopback.h"

#include <stdint.h>
#include <stdbool.h>

static state_t current_state;
static uint32_t state_timer = 0;
static Led fsm_led;
static bool led_state = false;

/* CAN driver */
extern const can_driver_t CAN_LOOPBACK;
static const can_driver_t *can = &CAN_LOOPBACK;

/* ---------------- ENTRY ACTIONS ---------------- */

static void entry_common(void)
{
    state_timer = 0;
    fsm_led.last_tick = hal_get_tick();
}

static void entry_off(void)        { led_state = false; entry_common(); }
static void entry_on(void)         { led_state = true;  entry_common(); }
static void entry_blink_slow(void) { entry_common(); fsm_led.period_ms = 1000; }
static void entry_blink_fast(void) { entry_common(); fsm_led.period_ms = 200; }

static action_fn_t entry_actions[STATE_COUNT] = {
    [STATE_OFF]        = entry_off,
    [STATE_ON]         = entry_on,
    [STATE_BLINK_SLOW] = entry_blink_slow,
    [STATE_BLINK_FAST] = entry_blink_fast
};

/* ---------------- EXIT ACTIONS ---------------- */

static void exit_noop(void) {}

static action_fn_t exit_actions[STATE_COUNT] = {
    [STATE_OFF]        = exit_noop,
    [STATE_ON]         = exit_noop,
    [STATE_BLINK_SLOW] = exit_noop,
    [STATE_BLINK_FAST] = exit_noop
};

/* ---------------- STATE ACTIONS ---------------- */

static void action_off(void)        { hal_write_led(0); }
static void action_on(void)         { hal_write_led(1); }
static void action_blink_slow(void) { led_tick(&fsm_led); }
static void action_blink_fast(void) { led_tick(&fsm_led); }

static action_fn_t actions[STATE_COUNT] = {
    [STATE_OFF]        = action_off,
    [STATE_ON]         = action_on,
    [STATE_BLINK_SLOW] = action_blink_slow,
    [STATE_BLINK_FAST] = action_blink_fast
};

/* ---------------- TRANSITIONS ---------------- */

static const transition_t transitions[] = {
    {STATE_OFF,        EVENT_BTN_PRESS, tguard, STATE_ON},
    {STATE_ON,         EVENT_BTN_PRESS, tguard, STATE_BLINK_SLOW},
    {STATE_BLINK_SLOW, EVENT_BTN_PRESS, tguard, STATE_BLINK_FAST},
    {STATE_BLINK_FAST, EVENT_BTN_PRESS, tguard, STATE_OFF}
};

static const int transition_count =
    sizeof(transitions) / sizeof(transitions[0]);

static state_t next_state(state_t s, event_t e)
{
    for (int i = 0; i < transition_count; i++)
    {
        const transition_t *t = &transitions[i];
        if (t->from == s && t->event == e && t->guard())
            return t->to;
    }
    return s;
}

/* ---------------- PUBLIC API ---------------- */

void sm_init(void)
{
    led_init(&fsm_led, 1000);
    current_state = STATE_OFF;
    entry_actions[current_state]();
}

void sm_handle_event(event_t event)
{
    state_t new_state = next_state(current_state, event);

    if (new_state != current_state)
    {
        /* Transmit a CAN frame for the state transition */
        can_frame_t f = {
            .id = 0x100,
            .dlc = 2,
            .data = { current_state, new_state }
        };
        can->send(&f);

        exit_actions[current_state]();
        current_state = new_state;
        entry_actions[current_state]();
    }
}

void sm_tick(void)
{
    state_timer++;
    actions[current_state]();
}

state_t sm_get_state(void)
{
    return current_state;
}

uint32_t sm_get_timer(void)
{
    return state_timer;
}
