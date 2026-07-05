#ifndef FAKE_HAL_H
#define FAKE_HAL_H

#include <stdint.h>

void fake_hal_set_tick(uint32_t t);
void fake_hal_reset(void);
void HAL_GPIO_TogglePin(void*, uint16_t);
int fake_hal_get_toggle_count(void);
void fake_hal_reset_toggle_count(void);

uint32_t HAL_GetTick(void);
void hal_toggle_led(void);

#endif // FAKE_HAL_H
