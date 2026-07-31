#include <stdio.h>

#include "stm32f4xx.h"
#include "button.h"


void EXTI0_IRQHandler(void)
{
    // Clear the interrupt pending flag for EXTI line 0
    EXTI_PR = (1 << 0);

    // Notify the button module that an interrupt occurred
    button_isr_handler();
}
