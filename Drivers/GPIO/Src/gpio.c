#include "stm32f4xx.h"
#include "gpio.h"

#include <stdio.h>

void gpio_init(void)
{
    printf("INFO: LED12 activated\n");

    // Enable clock for GPIOD
    RCC_AHB1ENR |= (1 << 3);

    // Configure PD12 as output
    GPIOD_MODER &= ~(3 << (12 * 2));
    GPIOD_MODER |=  (1 << (12 * 2));
}

void gpio_write(int state)
{
    if (state)
    {
        printf("INFO: LED12 set HIGH\n");
        GPIOD_ODR |=  (1 << 12);   // LED12 ON
    }
    else
    {
        printf("INFO: LED12 set LOW\n");
        GPIOD_ODR &= ~(1 << 12);   // LED12 OFF
    }
}


void gpio_toggle(void)
{
    printf("INFO: LED12 activated\n");

    // Toggle PD12
    GPIOD_ODR ^= (1 << 12);
}

