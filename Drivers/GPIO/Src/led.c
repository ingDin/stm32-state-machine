#include "stm32f4xx.h"
#include "led.h"

#include <stdio.h>

void led_init(void)
{
    printf("LED PD12 init!\n");

    // Enable clock for GPIOD
    RCC_AHB1ENR |= (1 << 3);

    // Configure PD12 as output
    GPIOD_MODER &= ~(3 << (12 * 2));
    GPIOD_MODER |=  (1 << (12 * 2));
}

void led_toggle(void)
{
    printf("LED PD12 toggle!\n");

    // Toggle PD12
    GPIOD_ODR ^= (1 << 12);
}
