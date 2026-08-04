/**
 * @file hal_wrapper.c
 * @brief Thin abstraction layer for hardware LED and timing operations.
 *
 * This module isolates all hardware‑specific GPIO and timing operations
 * behind a minimal API. Higher‑level modules (FSM, LED driver, button handler)
 * interact only with this wrapper, never directly with STM32 HAL or LL calls.
 *
 * Benefits:
 *   - clean separation between logic and hardware
 *   - easier unit testing (fake_hal.c overrides these functions)
 *   - consistent timing and LED control API across the application
 *
 * Functions provided:
 *   - hal_get_tick()   : returns system tick in milliseconds
 *   - hal_toggle_led() : toggles LED GPIO pin
 *   - hal_write_led()  : writes ON/OFF state to LED GPIO pin
 *
 * The actual hardware operations are delegated to gpio.c and time.c,
 * keeping this wrapper extremely lightweight.
 */

#include "hal_wrapper.h"
#include "gpio.h"   /**< GPIO abstraction for LED control */
#include "time.h"   /**< Timing abstraction (SysTick-based) */

/**
 * @brief Software tick counter incremented from SysTick_Handler.
 *
 * Note: The actual tick value is retrieved via get_tick_count() from time.c.
 * This variable is kept for compatibility and clarity, but not used directly.
 */
static volatile uint32_t tick_ms = 0;

/**
 * @brief Retrieve current system tick in milliseconds.
 *
 * Delegates to time.c, which maintains a millisecond counter updated
 * by SysTick_Handler. Used by LED driver and FSM for timing logic.
 *
 * @return Current tick count in milliseconds.
 */
uint32_t hal_get_tick(void)
{
    return get_tick_count();
}

/**
 * @brief Toggle the LED GPIO pin.
 *
 * Delegates to gpio_toggle(), which abstracts the actual GPIO operation.
 * Used by blink logic in both the LED driver and FSM.
 */
void hal_toggle_led(void)
{
    gpio_toggle();
}

/**
 * @brief Write a specific state to the LED GPIO pin.
 *
 * @param state 0 = OFF, non‑zero = ON
 *
 * Delegates to gpio_write(), which sets the LED pin to the desired state.
 * Used by FSM states such as STATE_ON and STATE_OFF.
 */
void hal_write_led(int state)
{
    gpio_write(state);
}
