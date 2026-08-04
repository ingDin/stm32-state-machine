/**
 * @file button.h
 * @brief Debounced user button interface with event callback delivery.
 *
 * The button module samples the raw input level, applies debounce filtering,
 * tracks a stable pressed/released state, and notifies external modules
 * (typically the FSM) when transitions occur. In TEST mode, the internal
 * state is exposed for deterministic unit testing.
 *
 * Integrated behavior:
 *   - Debounce logic based on periodic sampling
 *   - Stable state tracking (pressed / released)
 *   - Mandatory event callback for press/release notifications
 *   - TEST mode support via exported button_state_t instance
 *
 * The module is driven by:
 *   - button_isr_handler(): captures raw input changes
 *   - button_tick(): performs debounce and event dispatch
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include "state_machine.h"   /**< For event_t */

/**
 * @brief Internal button state structure.
 *
 * Tracks raw input level, debounced stable state, debounce counter,
 * and an event flag indicating that a new raw sample is available.
 */
typedef struct
{
    int raw_level;            /**< Last sampled raw GPIO level */
    int stable_state;         /**< Debounced stable state (0 or 1) */
    int debounce;             /**< Debounce counter */
    volatile int event_flag;  /**< Set when a new raw sample must be processed */
} button_state_t;

#if defined(TEST)
/**
 * @brief Exposed button instance for TEST mode.
 *
 * Allows unit tests to inspect and manipulate internal button state.
 */
extern button_state_t btn;
#endif

/**
 * @brief Callback type for delivering button events.
 *
 * External modules (typically the FSM) must register a callback to receive
 * EVENT_BTN_PRESS and EVENT_BTN_RELEASE notifications.
 */
typedef void (*button_event_cb_t)(event_t event);

/**
 * @brief Initialize the button module.
 *
 * Resets internal state and prepares the module for sampling and event
 * generation.
 */
void button_init(void);

/**
 * @brief Interrupt handler for the button EXTI line.
 *
 * Captures raw input changes and sets the event flag for later processing
 * inside button_tick().
 */
void button_isr_handler(void);

/**
 * @brief Periodic button processing.
 *
 * Performs debounce filtering, detects stable transitions, and invokes the
 * registered callback when press/release events occur.
 */
void button_tick(void);

/**
 * @brief Check whether the button is currently pressed.
 *
 * @return true if the debounced stable state is pressed, false otherwise.
 */
bool button_is_pressed(void);

/**
 * @brief Register the mandatory callback for button events.
 *
 * Without a callback, no events are propagated to external modules.
 *
 * @param cb Callback function receiving event_t values.
 */
void button_set_callback(button_event_cb_t cb);

#endif /* BUTTON_H */
