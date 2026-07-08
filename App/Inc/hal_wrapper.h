#ifndef HAL_WRAPPER_H
#define HAL_WRAPPER_H

#include <stdint.h>

uint32_t hal_get_tick(void);
void hal_toggle_led(void);
void hal_write_led(int state);

#endif
