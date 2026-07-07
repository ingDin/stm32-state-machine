#include <stdio.h>

#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"

void EXTI0_IRQHandler(void)
{
    if (EXTI_PR & (1 << 0))   // Check pending flag
    {
        EXTI_PR |= (1 << 0);  // Clear pending flag

        printf("B1 pressed (EXTI0)!\n");

        led_toggle();
        delay_ms(2000);
    }
}
