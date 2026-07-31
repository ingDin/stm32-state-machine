/**
 * @file state_machine.c
 * @brief Finite state machine for LED control.
 *
 * Implements a simple FSM with four states:
 *   OFF → ON → BLINK_SLOW → BLINK_FAST → OFF
 *
 * Features:
 *   - entry/exit actions
 *   - per‑state periodic actions
 *   - internal state timer (reset on transitions)
 *   - blink timing based on hal_get_tick()
 *
 * Designed for deterministic behavior and full unit‑test coverage.
 */

#include "state_machine.h"
#include "hal_wrapper.h"
#include "guards.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

extern uint32_t hal_get_tick(void);

static state_t current_state;
static uint32_t last_toggle_time = 0;
static uint32_t state_timer = 0;
static bool led_state = false;

/* COMMON ENTRY HELPERS */
static void entry_common(void)
{
    state_timer = 0;
    last_toggle_time = hal_get_tick();
}

static void entry_led(bool on)
{
    led_state = on;
    entry_common();
}

/* ENTRY ACTIONS */
static void entry_off(void) {
	printf("DEBUG: entry_off\n");
	entry_led(false);
}
static void entry_on(void) {
	entry_led(true);
	printf("DEBUG: entry_on\n");
}
static void entry_blink_slow(void) {
	entry_common();
	printf("DEBUG: entry_blink_slow\n");}
static void entry_blink_fast(void) {
	entry_common();
	printf("DEBUG: entry_blink_fast\n");}

/* EXIT ACTIONS (all no-op) */
static void exit_noop(void) {}

static action_fn_t exit_actions[STATE_COUNT] = {
    [STATE_OFF] = exit_noop,
    [STATE_ON] = exit_noop,
    [STATE_BLINK_SLOW] = exit_noop,
    [STATE_BLINK_FAST] = exit_noop};

/* STATE ACTIONS */

static void action_set(bool state)
{
    led_state = state;
    hal_write_led(led_state);
}

static void action_off(void)
{
    action_set(false);
}

static void action_on(void)
{
    action_set(true);
}


static bool timer_expired(uint32_t interval)
{
    return (hal_get_tick() - last_toggle_time) >= interval;
}

static void action_blink(uint32_t interval)
{
    if (timer_expired(interval))
    {
        hal_toggle_led();
        last_toggle_time = hal_get_tick();
    }
}

static void action_blink_slow(void) { action_blink(1000); }
static void action_blink_fast(void) { action_blink(200); }

static action_fn_t entry_actions[STATE_COUNT] = {
    [STATE_OFF] = entry_off,
    [STATE_ON] = entry_on,
    [STATE_BLINK_SLOW] = entry_blink_slow,
    [STATE_BLINK_FAST] = entry_blink_fast};

static action_fn_t actions[STATE_COUNT] = {
    [STATE_OFF] = action_off,
    [STATE_ON] = action_on,
    [STATE_BLINK_SLOW] = action_blink_slow,
    [STATE_BLINK_FAST] = action_blink_fast};

static const transition_t transitions[] = {
    {STATE_OFF, EVENT_BTN_PRESS, tguard, STATE_ON},
    {STATE_ON, EVENT_BTN_PRESS, tguard, STATE_BLINK_SLOW},
    {STATE_BLINK_SLOW, EVENT_BTN_PRESS, tguard, STATE_BLINK_FAST},
    {STATE_BLINK_FAST, EVENT_BTN_PRESS, tguard, STATE_OFF}};

static const int transition_count =
    sizeof(transitions) / sizeof(transitions[0]);

static state_t next_state(state_t current, event_t event)
{
    for (int i = 0; i < transition_count; i++)
    {
        const transition_t *t = &transitions[i];
        if (t->from == current && t->event == event && t->guard())
        {
            return t->to;
        }
    }
    return current;
}

void sm_init(void)
{
    current_state = STATE_OFF;
    entry_actions[current_state]();
}

void sm_handle_event(event_t event)
{
    state_t new_state = next_state(current_state, event);
    if (new_state != current_state)
    {
        exit_actions[current_state]();
        current_state = new_state;
        entry_actions[current_state]();
    }
}

state_t sm_get_state(void)
{
    return current_state;
}

uint32_t sm_get_timer(void)
{
    return state_timer;
}

void sm_tick(void)
{
    state_timer++;
    actions[current_state]();
}
