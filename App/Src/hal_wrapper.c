/**
 * @file hal_wrapper.c
 * @brief Thin wrapper around HAL GPIO operations for LED control.
 *
 * Provides a single abstraction function, hal_toggle_led(), which toggles
 * the LED pin using the STM32 HAL. This keeps hardware access isolated
 * from higher‑level application logic.
 */

#include "hal_wrapper.h"
#include "gpio.h"
#include "time.h"

// un tick software, incrementat din SysTick_Handler
static volatile uint32_t tick_ms = 0;

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
