/**
 * @file hal_wrapper.c
 * @brief Thin wrapper around HAL GPIO operations for LED control.
 *
 * Provides a single abstraction function, hal_toggle_led(), which toggles
 * the LED pin using the STM32 HAL. This keeps hardware access isolated
 * from higher‑level application logic.
 */

#include "hal_wrapper.h"

#include <stm32l4xx_hal.h>

void hal_toggle_led(void)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
}
