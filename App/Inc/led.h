/**
 * @file led.h
 * @brief Periodic LED driver using HAL tick timing.
 *
 * This module implements a simple, instance‑based LED driver that toggles
 * the LED when a configurable period has elapsed. Timing is derived from
 * hal_get_tick(), allowing deterministic behavior in both hardware and
 * TEST mode (via fake_hal).
 *
 * Features:
 *   - Instance‑based LED configuration
 *   - Millisecond‑period toggle timing
 *   - Deterministic tick‑driven behavior
 *   - Compatible with hal_wrapper abstraction
 */

#ifndef LED_H
#define LED_H

#include <stdint.h>

/**
 * @brief LED instance structure.
 *
 * Stores timing information required for periodic toggling.
 */
typedef struct
{
    uint32_t last_tick;   /**< Timestamp of last toggle */
    uint32_t period_ms;   /**< Toggle period in milliseconds */
} Led;

/**
 * @brief Initialize an LED instance.
 *
 * Captures the current tick and configures the toggle period.
 *
 * @param led        Pointer to LED instance.
 * @param period_ms  Toggle period in milliseconds.
 */
void led_init(Led *led, uint32_t period_ms);

/**
 * @brief Periodic LED processing.
 *
 * Must be called frequently (main loop or timer interrupt). Toggles the LED
 * when the configured period has elapsed since last_tick.
 *
 * @param led Pointer to LED instance.
 */
void led_tick(Led *led);

/**
 * @brief Toggle the LED using HAL abstraction.
 *
 * Calls hal_toggle_led(). Used internally by led_tick().
 */
void led_toggle(void);

#endif /* LED_H */
