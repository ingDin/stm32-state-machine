/**
 * @file led.c
 * @brief Periodic LED toggle module using HAL tick timing.
 *
 * This module implements a simple, instance‑based LED driver that toggles
 * an LED at a configurable period. It relies on hal_get_tick() for timing
 * and hal_toggle_led() for GPIO control.
 *
 * Features:
 *   - Instance-based LED timing (no global state)
 *   - Deterministic tick-driven behavior
 *   - Full compatibility with fake_hal for unit testing
 *   - Clean separation between hardware access and logic
 *
 * Typical usage:
 *   Led heartbeat;
 *   led_init(&heartbeat, 500);   // toggle every 500ms
 *
 *   while (1) {
 *       led_tick(&heartbeat);
 *   }
 */

#include "led.h"
#include "hal_wrapper.h"

#if defined(TEST)
#include "fake_hal.h"
#endif

/**
 * @brief Initialize an LED instance.
 *
 * @param led        Pointer to LED instance.
 * @param period_ms  Toggle period in milliseconds.
 *
 * Sets the initial tick timestamp and configures the toggle period.
 * In TEST mode, also resets the fake HAL toggle counter to ensure
 * deterministic unit-test behavior.
 */
void led_init(Led *led, uint32_t period_ms)
{
    led->period_ms = period_ms;
    led->last_tick = hal_get_tick();

#if defined(TEST)
    fake_hal_reset_toggle_count(); /**< Reset toggle counter for unit tests */
#endif
}

/**
 * @brief Periodic LED processing.
 *
 * @param led Pointer to LED instance.
 *
 * Must be called frequently (e.g., from app_tick() or app_run()).
 * When the configured period has elapsed, the LED is toggled and
 * the internal timestamp is updated.
 */
void led_tick(Led *led)
{
    uint32_t now = hal_get_tick();

    if ((now - led->last_tick) >= led->period_ms)
    {
        led->last_tick = now;
        led_toggle();
    }
}

/**
 * @brief Toggle the LED using the HAL wrapper.
 *
 * This function does not modify the LED instance timing; it simply
 * delegates the GPIO operation to hal_toggle_led().
 */
void led_toggle(void)
{
    hal_toggle_led();
}
