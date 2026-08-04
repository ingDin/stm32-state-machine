/**
 * @file button.h
 * @brief User button handling with debouncing and event generation.
 *
 * This module provides a debounced button interface that integrates with the
 * finite state machine (FSM). It tracks raw input level, stable debounced
 * state, and generates events when transitions occur (press/release).
 *
 * Features:
 *   - Debounce logic using periodic sampling
 *   - Stable state tracking
 *   - Optional callback injection for event delivery
 *   - TEST mode support via exported button_state_t instance
 *
 * The button module is designed to be called from:
 *   - EXTI interrupt handler (button_isr_handler)
 *   - main loop tick (button_tick)
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include "state_machine.h" /**< For event_t */

/**
 * @brief Internal button state structure.
 *
 * Tracks raw input level, debounced stable state, debounce counter,
 * and an event flag used to signal transitions.
 */
typedef struct
{
    int raw_level;        /**< Last sampled raw GPIO level */
    int stable_state;     /**< Debounced stable state (0 or 1) */
    int debounce;         /**< Debounce counter */
    volatile int event_flag; /**< Flag set when a button event occurs */
} button_state_t;

#ifdef TEST
/**
 * @brief Exposed button instance for unit testing.
 *
 * Allows test code to inspect and manipulate internal button state.
 */
extern button_state_t btn;
#endif

/**
 * @brief Callback type for delivering button events.
 *
 * Allows injection of custom event handlers (e.g., forwarding events
 * directly to the FSM or logging them).
 */
typedef void (*button_event_cb_t)(event_t event);

/**
 * @brief Initialize the button module.
 *
 * Resets internal state, clears debounce counters, and prepares the
 * module for periodic sampling and interrupt handling.
 */
void button_init(void);

/**
 * @brief Interrupt handler for button EXTI line.
 *
 * Called from the actual hardware interrupt. Captures raw input level
 * and sets event flags for later processing in button_tick().
 */
void button_isr_handler(void);

/**
 * @brief Periodic button processing.
 *
 * Must be called frequently (e.g., from app_run()). Performs debouncing,
 * detects stable transitions, and triggers event callbacks or flags.
 */
void button_tick(void);

/**
 * @brief Check whether the button is currently pressed.
 *
 * @return true if stable debounced state is pressed, false otherwise.
 */
bool button_is_pressed(void);

/**
 * @brief Inject a callback for button events.
 *
 * Allows external modules (e.g., FSM) to receive events directly.
 *
 * @param cb Callback function receiving event_t values.
 */
void button_set_callback(button_event_cb_t cb);

#endif /* BUTTON_H */
