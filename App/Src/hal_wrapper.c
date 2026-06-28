#include "hal_wrapper.h"
#include "stm32l4xx_hal.h"

void hal_toggle_led(void)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
}
