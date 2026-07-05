#include "led.h"
#include "hal_wrapper.h"
#include "fake_hal.h"

static uint32_t last_tick = 0;
static const uint32_t LED_PERIOD_MS = 500;

void led_init(void)
{
    last_tick = HAL_GetTick();
    fake_hal_reset();   // resetăm și toggle counter-ul
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
