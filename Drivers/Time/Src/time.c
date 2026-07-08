#include "stm32f4xx.h"
#include "time.h"

#include <stdint.h>

volatile uint32_t tick_count = 0;

/*
 * SysTick interrupt handler
 * Called every 1 ms. Increments the global tick counter.
 */
void SysTick_Handler(void)
{
    tick_count++;
}

/*
 * Initialize SysTick to generate a 1 ms interrupt.
 * Assumes CPU clock is 16 MHz.
 */
void time_init(void)
{
    // 16 MHz / 1000 = 16000 ticks for 1 ms
    SYSTICK_LOAD = 16000 - 1;

    // Clear current SysTick value
    SYSTICK_VAL = 0;

    // Enable SysTick:
    // - Clock source = CPU
    // - Interrupt enabled
    // - Counter enabled
    SYSTICK_CTRL = (1 << 2) | (1 << 1) | (1 << 0);
}

/*
 * Return the current tick count in milliseconds.
 */
uint32_t get_tick_count(void)
{
    return tick_count;
}

/*
 * Millisecond delay using the tick counter.
 * This does not rely on COUNTFLAG and works regardless of SysTick reload value.
 */
void delay_ms(uint32_t ms)
{
    uint32_t start = get_tick_count();
    while ((get_tick_count() - start) < ms);
}
