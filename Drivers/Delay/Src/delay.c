#include <stdio.h>

#include "stm32f4xx.h"
#include "delay.h"

/*
 * Initialize SysTick to generate a 1ms tick.
 * This is done one time in main().
 */
void delay_init(void)
{
    // 16 MHz / 1000 = 16000 ticks pentru 1ms
    SYSTICK_LOAD = 16000 - 1;

    // Reset counter
    SYSTICK_VAL = 0;

    // Enable SysTick: clock source = CPU + counter enable
    SYSTICK_CTRL = (1 << 2) | (1 << 0);
}

/*
 * Delay în milisecunde folosind COUNTFLAG.
 * Funcția presupune că delay_init() a fost apelată înainte.
 */
void delay_ms(unsigned int ms)
{
	printf("delay(%d)!\n", ms);
    for (unsigned int i = 0; i < ms; i++)
    {
        while ((SYSTICK_CTRL & (1 << 16)) == 0);  // Așteaptă COUNTFLAG
    }
}
