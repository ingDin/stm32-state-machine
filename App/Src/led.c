/**
 * @file led.c
 * @brief Periodic LED toggle module using HAL tick timing.
 *
 * Tracks elapsed time using hal_get_tick() and toggles the LED every
 * LED_PERIOD_MS milliseconds. Provides initialization, periodic tick
 * processing, and access to the fake HAL toggle counter for testing.
 */

#include "led.h"
#include "hal_wrapper.h"

#if defined(TEST)
#include "fake_hal.h"
#endif

static uint32_t last_tick = 0;
static const uint32_t LED_PERIOD_MS = 500;

void led_init(void)
{
    last_tick = hal_get_tick();
#if defined(TEST)
    fake_hal_reset_toggle_count(); // reset toggle counter
#endif
}

void led_tick(void)
{
    uint32_t now = hal_get_tick();
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
