/**
 * @file button.c
 * @brief Debounced button implementation with event callback delivery.
 *
 * This module implements the runtime behavior of the button interface:
 * it captures raw input changes (via ISR or TEST mode), applies debounce
 * filtering, tracks a stable pressed/released state, and generates events
 * when transitions occur. Events are forwarded to the registered callback,
 * typically the FSM.
 *
 * In production mode, raw input is sampled from GPIOA (PA0). In TEST mode,
 * raw_level is driven by fake_button_isr() to enable deterministic unit tests.
 */

#include "button.h"

#if !defined(TEST)
#include "stm32f4xx.h"   /**< Hardware register access for GPIOA */
#endif

#include <stddef.h>

#define DEBOUNCE_TICKS 3 /**< Number of consecutive samples required to confirm a state change */

#if defined(TEST)
button_state_t btn = {0}; /**< Exposed in TEST mode for inspection by fake_button_isr.c */
#else
static button_state_t btn = {0}; /**< Hidden in production mode */
#endif

/**
 * @brief Callback invoked when a button event is detected.
 *
 * This callback is mandatory for correct system behavior. The FSM relies
 * on it to receive EVENT_BTN_PRESS and EVENT_BTN_RELEASE notifications.
 */
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
    /* New raw sample available */
    if (btn.event_flag)
    {
        btn.event_flag = 0;

#if !defined(TEST)
        /* Read PA0 input level */
        btn.raw_level = (GPIOA_IDR >> 0) & 1;
#endif
        /* In TEST mode, raw_level is set by fake_button_isr() */
    }

    /* Debounce logic */
    if (btn.raw_level != btn.stable_state)
    {
        if (btn.debounce < DEBOUNCE_TICKS)
            btn.debounce++;

        if (btn.debounce >= DEBOUNCE_TICKS)
        {
            int previous = btn.stable_state;
            btn.stable_state = btn.raw_level;
            btn.debounce = 0;

            /* Generate events */
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
