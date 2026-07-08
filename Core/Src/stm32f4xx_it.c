#include <stdio.h>

#include "stm32f4xx.h"

volatile int button_pressed = 0;

void EXTI0_IRQHandler(void)
{
    if (EXTI_PR & 1)
    {
    	printf("B1 pressed (EXTI0)!\n");
        EXTI_PR = 1;
        button_pressed = 1;
    }
}
