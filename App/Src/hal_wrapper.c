/**
 * @file hal_wrapper.c
 * @brief Hardware abstraction for LED control and timing.
 *
 * This module implements the runtime behavior of the HAL wrapper: it
 * forwards LED GPIO operations to gpio.c and retrieves the millisecond
 * tick from time.c. Higher‑level modules use this layer to remain fully
 * independent of STM32‑specific HAL/LL details. In TEST mode, the same
 * API is provided by fake_hal.c for deterministic unit testing.
 */

#include "hal_wrapper.h"
#include "gpio.h"   /**< GPIO abstraction for LED control */
#include "time.h"   /**< Timing abstraction (SysTick-based) */

uint32_t hal_get_tick(void)
{
    return get_tick_count();
}

void hal_toggle_led(void)
{
    gpio_toggle();
}

void hal_write_led(int state)
{
    gpio_write(state);
}
