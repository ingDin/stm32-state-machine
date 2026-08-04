/**
 * @file led.h
 * @brief LED driver with periodic toggle functionality using HAL tick timing.
 *
 * This module provides a configurable LED instance that tracks elapsed time
 * and toggles the LED when its period expires. It is designed to be modular,
 * testable, and compatible with the existing hal_wrapper abstraction.
 *
 * Features:
 *   - Instance-based LED driver (no global state)
 *   - Configurable toggle period per LED
 *   - Deterministic tick-based behavior
 *   - Compatible with fake_hal for unit testing
 */

#ifndef LED_H
#define LED_H

#include <stdint.h>

/**
 * @brief LED instance structure.
 *
 * Holds timing information for periodic LED toggling.
 */
typedef struct
{
    uint32_t last_tick;   /**< Timestamp of last toggle */
    uint32_t period_ms;   /**< Toggle period in milliseconds */
} Led;

/**
 * @brief Initialize an LED instance.
 *
 * @param led        Pointer to LED instance.
 * @param period_ms  Toggle period in milliseconds.
 *
 * Captures the current tick and resets test counters if TEST mode is enabled.
 */
void led_init(Led *led, uint32_t period_ms);

/**
 * @brief Periodic tick processing for LED.
 *
 * @param led Pointer to LED instance.
 *
 * Must be called frequently (main loop or timer interrupt). Toggles the LED
 * when the configured period has elapsed.
 */
void led_tick(Led *led);

/**
 * @brief Toggle the LED using HAL abstraction.
 *
 * This function directly calls hal_toggle_led() and does not modify the
 * instance timing. It is used internally by led_tick().
 */
void led_toggle(void);

#endif // LED_H
