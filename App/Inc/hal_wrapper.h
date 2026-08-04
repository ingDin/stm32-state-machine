/**
 * @file hal_wrapper.h
 * @brief Hardware abstraction layer for LED and timing operations.
 *
 * This module provides a minimal abstraction over STM32 HAL or LL functions,
 * allowing higher‑level modules (FSM, LED driver, button handler) to operate
 * without depending directly on hardware registers or HAL internals.
 *
 * Benefits:
 *   - decouples application logic from hardware specifics
 *   - simplifies unit testing (fake_hal can override these functions)
 *   - provides a stable API for timing and LED control
 *
 * Functions:
 *   - hal_get_tick()   : retrieve system tick in milliseconds
 *   - hal_toggle_led() : toggle LED GPIO pin
 *   - hal_write_led()  : set LED GPIO pin to ON/OFF
 */

#ifndef HAL_WRAPPER_H
#define HAL_WRAPPER_H

#include <stdint.h>

/**
 * @brief Retrieve the current system tick in milliseconds.
 *
 * Typically backed by SysTick or LL_GetTick(), depending on the hardware
 * configuration. Used for timing operations in the LED driver and FSM.
 *
 * @return Current tick count in milliseconds.
 */
uint32_t hal_get_tick(void);

/**
 * @brief Toggle the LED GPIO pin.
 *
 * Used by blink logic and LED driver. The implementation is hardware‑specific
 * and may use HAL_GPIO_TogglePin(), LL_GPIO_TogglePin(), or direct register
 * access depending on the platform.
 */
void hal_toggle_led(void);

/**
 * @brief Write a specific state to the LED GPIO pin.
 *
 * @param state 0 for OFF, non‑zero for ON.
 *
 * Used by FSM states such as STATE_ON and STATE_OFF. Abstracts away the
 * underlying GPIO implementation.
 */
void hal_write_led(int state);

#endif /* HAL_WRAPPER_H */
