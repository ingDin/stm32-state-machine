/**
 * @file led.c
 * @brief Periodic LED toggle implementation using HAL tick timing.
 *
 * This module implements the runtime behavior of the LED driver: it tracks
 * elapsed time using hal_get_tick() and toggles the LED when its configured
 * period expires. The logic is instance‑based and fully compatible with
 * fake_hal in TEST mode, ensuring deterministic unit testing.
 */

#include "led.h"
#include "hal_wrapper.h"

#if defined(TEST)
#include "fake_hal.h"
#endif

void led_init(Led *led, uint32_t period_ms)
{
    led->period_ms = period_ms;
    led->last_tick = hal_get_tick();

#if defined(TEST)
    fake_hal_reset_toggle_count();
#endif
}

void led_tick(Led *led)
{
    uint32_t now = hal_get_tick();

    if ((now - led->last_tick) >= led->period_ms)
    {
        led->last_tick = now;
        led_toggle();
    }
}

void led_toggle(void)
{
    hal_toggle_led();
}
