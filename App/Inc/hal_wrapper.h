/**
 * @file hal_wrapper.h
 * @brief Minimal hardware abstraction layer for timing and LED control.
 *
 * This module provides a small, stable API that hides STM32‑specific
 * details behind simple functions. Higher‑level modules (FSM, LED driver,
 * button handler) interact only with this wrapper, allowing seamless
 * replacement with fake_hal.c in TEST mode.
 *
 * Responsibilities:
 *   - Provide a unified millisecond tick source
 *   - Abstract LED GPIO operations (write + toggle)
 *   - Keep hardware access isolated from application logic
 */

#ifndef HAL_WRAPPER_H
#define HAL_WRAPPER_H

#include <stdint.h>

/**
 * @brief Retrieve the current system tick in milliseconds.
 *
 * Backed by the SysTick counter maintained in time.c. Used by timing‑based
 * modules such as the LED driver and FSM.
 *
 * @return Current tick count in milliseconds.
 */
uint32_t hal_get_tick(void);

/**
 * @brief Toggle the LED GPIO pin.
 *
 * Delegates to gpio_toggle(). Used by blink logic in the LED driver and FSM.
 */
void hal_toggle_led(void);

/**
 * @brief Write a specific state to the LED GPIO pin.
 *
 * @param state 0 = OFF, non‑zero = ON
 *
 * Delegates to gpio_write(). Used by FSM states such as STATE_ON and STATE_OFF.
 */
void hal_write_led(int state);

#endif /* HAL_WRAPPER_H */
