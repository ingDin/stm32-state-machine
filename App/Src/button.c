/**
 * @file button.c
 * @brief Debounced button handler with event generation and callback support.
 *
 * This module implements a deterministic, testable button interface that
 * integrates with the finite state machine (FSM). It provides:
 *
 *   - raw GPIO sampling (via ISR or fake HAL)
 *   - debounce filtering
 *   - stable state tracking
 *   - press/release event generation
 *   - optional callback injection for event delivery
 *
 * In production mode, raw input is read from GPIOA (PA0). In TEST mode,
 * raw_level is controlled by fake_button_isr() for full unit-test coverage.
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
 * If NULL, events are ignored. Typically set to sm_handle_event().
 */
static button_event_cb_t event_cb = NULL;

/**
 * @brief Register a callback for button events.
 *
 * @param cb Function to call when EVENT_BTN_PRESS or EVENT_BTN_RELEASE occurs.
 */
void button_set_callback(button_event_cb_t cb)
{
    event_cb = cb;
}

/**
 * @brief Initialize internal button state.
 *
 * Resets raw level, stable state, debounce counter, and event flag.
 */
void button_init(void)
{
    btn.raw_level = 0;
    btn.stable_state = 0;
    btn.debounce = 0;
    btn.event_flag = 0;
}

/**
 * @brief ISR handler for button EXTI line.
 *
 * Sets event_flag so that button_tick() will process the new raw level.
 * In TEST mode, fake_button_isr() sets raw_level directly.
 */
void button_isr_handler(void)
{
    btn.event_flag = 1;
}

/**
 * @brief Periodic button processing (debounce + event generation).
 *
 * Must be called frequently (e.g., from app_tick()).
 *
 * Behavior:
 *   - If event_flag is set, read raw GPIO level (or use TEST-provided value)
 *   - Compare raw_level with stable_state
 *   - If different, increment debounce counter
 *   - Once debounce threshold is reached, update stable_state
 *   - Generate EVENT_BTN_PRESS or EVENT_BTN_RELEASE via callback
 */
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

/**
 * @brief Check whether the button is currently pressed.
 *
 * @return true if stable debounced state is 1, false otherwise.
 */
bool button_is_pressed(void)
{
    return btn.stable_state == 1;
}
