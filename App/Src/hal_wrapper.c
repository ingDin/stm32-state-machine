/**
 * @file hal_wrapper.c
 * @brief Thin wrapper around HAL GPIO operations for LED control.
 *
 * Provides a single abstraction function, hal_toggle_led(), which toggles
 * the LED pin using the STM32 HAL. This keeps hardware access isolated
 * from higher‑level application logic.
 */

#include "hal_wrapper.h"

// un tick software, incrementat din SysTick_Handler
static volatile uint32_t tick_ms = 0;

uint32_t hal_get_tick(void)
{
    return tick_ms;
}

void SysTick_Handler(void)
{
    tick_ms++;
}

void hal_toggle_led(void)
{
    // aici scrii tu codul bare-metal
    // exemplu pentru STM32F4:
    //GPIOA->ODR ^= (1 << 5);
}
