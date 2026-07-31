#include "button.h"

#if !defined(TEST)
#include "stm32f4xx.h"
#endif

#include <stddef.h>

#define DEBOUNCE_TICKS 3

#if defined(TEST)
button_state_t btn = {0}; // visible to fake_button_isr.c
#else
static button_state_t btn = {0}; // hidden in production
#endif

static button_event_cb_t event_cb = NULL;

void button_set_callback(button_event_cb_t cb)
{
    event_cb = cb;
}

void button_init(void)
{
    btn.raw_level = 0;
    btn.stable_state = 0;
    btn.debounce = 0;
    btn.event_flag = 0;
}

void button_isr_handler(void)
{
    btn.event_flag = 1;
}

void button_tick(void)
{
    if (btn.event_flag)
    {
        btn.event_flag = 0;
#if !defined(TEST)
        btn.raw_level = (GPIOA_IDR >> 0) & 1;
#endif
        /* In TEST mode, raw_level is set by fake_button_isr() */
    }

    if (btn.raw_level != btn.stable_state)
    {

        if (btn.debounce < DEBOUNCE_TICKS)
            btn.debounce++;

        if (btn.debounce >= DEBOUNCE_TICKS)
        {

            int previous = btn.stable_state;
            btn.stable_state = btn.raw_level;
            btn.debounce = 0;

            if (event_cb)
            {
                if (previous == 0 && btn.stable_state == 1)
                    event_cb(EVENT_BTN_PRESS);

                if (previous == 1 && btn.stable_state == 0)
                    event_cb(EVENT_BTN_RELEASE);
            }
        }
    }
    else
    {
        btn.debounce = 0;
    }
}

bool button_is_pressed(void)
{
    return btn.stable_state == 1;
}
