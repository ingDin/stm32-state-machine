/**
 * @file led.c
 * @brief Periodic LED toggle module using HAL tick timing.
 *
 * Tracks elapsed time using HAL_GetTick() and toggles the LED every
 * LED_PERIOD_MS milliseconds. Provides initialization, periodic tick
 * processing, and access to the fake HAL toggle counter for testing.
 */

 #include "led.h"
#include "hal_wrapper.h"
#include "fake_hal.h"

static uint32_t last_tick = 0;
static const uint32_t LED_PERIOD_MS = 500;

void led_init(void)
{
    last_tick = HAL_GetTick();
    fake_hal_reset_toggle_count();   // reset toggle counter
}

void led_tick(void)
{
    uint32_t now = HAL_GetTick();
    if (now - last_tick >= LED_PERIOD_MS)
    {
        last_tick = now;
        led_toggle();
    }
}

void led_toggle(void)
{
    hal_toggle_led();
}

int led_get_toggle_count(void)
{
    return fake_hal_get_toggle_count();
}
