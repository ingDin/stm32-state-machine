#include <stdio.h>
#include "stm32f4xx.h"
#include "user_button.h"

void user_button_init(void)
{
    printf("INFO: User button init (B1: PA0 / EXTI0)\n");

    /* Enable clock for GPIOA */
    RCC_AHB1ENR |= (1 << 0);

    /* PA0 as input */
    GPIOA_MODER &= ~(3 << (0 * 2));

    /* Enable SYSCFG clock */
    RCC_APB2ENR |= (1 << 14);

    /* Connect PA0 to EXTI0 */
    SYSCFG_EXTICR1 &= ~0xF;   // Clear EXTI0 bits
    SYSCFG_EXTICR1 |= 0x0;    // PA0 -> EXTI0

    /* Configure EXTI0 */
    EXTI_IMR  |= (1 << 0);    // Unmask EXTI0
    EXTI_RTSR |= (1 << 0);    // Rising edge trigger
    EXTI_FTSR &= ~(1 << 0);   // No falling edge

    /* Enable EXTI0 interrupt in NVIC */
    NVIC_ISER0 |= (1 << 6);   // IRQ6 = EXTI0

    printf("EXTI0 enabled for PA0 (B1)\n");
}
