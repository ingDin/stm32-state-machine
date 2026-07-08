#include <stdio.h>

#include "stm32f4xx.h"
#include "button.h"


void EXTI0_IRQHandler(void)
{
	printf("DEBUG: Expected interrupt triggered\n");
    EXTI_IMR &= ~(1 << 0);   // disable EXTI0 temporarily

    EXTI_PR = (1 << 0);      // clear interrupt flag

    unsigned long raw_state = (GPIOA_IDR >> 0) & 1;

    button_isr_handler(raw_state);

    EXTI_IMR |= (1 << 0);    // re-enable EXTI0
}
