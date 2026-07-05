/**
 * @file button.c
 * @brief Debounce logic and event generation for button input.
 *
 * Handles raw ISR updates, applies a 3‑tick debounce, emits press/release
 * events, and exposes the stable pressed state.
 */

#include "button.h"
#include "fake_fsm.h"

#define DEBOUNCE_TICKS 3

static int raw_level = 0;
static int stable_state = 0;
static int debounce = 0;

void button_init(void)
{
    raw_level = 0;
    stable_state = 0;
    debounce = 0;
}

void button_isr_handler(int level)
{
    raw_level = level;
}

void button_tick(void)
{
    if (raw_level != stable_state) {

        if (debounce < DEBOUNCE_TICKS)
            debounce++;

        if (debounce >= DEBOUNCE_TICKS) {

            int previous = stable_state;
            stable_state = raw_level;
            debounce = 0;

            if (previous == 0 && stable_state == 1)
                fake_fsm_push_event(EVENT_BTN_PRESS);

            if (previous == 1 && stable_state == 0)
                fake_fsm_push_event(EVENT_BTN_RELEASE);
        }
    }
    else {
        debounce = 0;
    }
}

bool button_is_pressed(void)
{
    return stable_state == 1;
}
