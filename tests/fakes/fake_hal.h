/**
 * @file fake_hal.h
 * @brief Interface for the fake HAL used in unit tests.
 *
 * Provides controlled test doubles for:
 *   - HAL_GetTick()
 *   - HAL_GPIO_TogglePin()
 *
 * Allows tests to simulate time progression and count LED toggles
 * without relying on STM32 hardware.
 */
 
 #ifndef FAKE_HAL_H
#define FAKE_HAL_H

#include <stdint.h>

void fake_hal_set_tick(uint32_t t);
void fake_hal_reset(void);
void HAL_GPIO_TogglePin(void*, uint16_t);
int fake_hal_get_toggle_count(void);

#endif
