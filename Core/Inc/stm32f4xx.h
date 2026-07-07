#ifndef STM32F4XX_H
#define STM32F4XX_H

/* -------------------- BASE ADDRESSES -------------------- */

#define PERIPH_BASE           0x40000000U
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000U)

/* GPIO */
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00U)
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000U)

/* RCC */
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800U)

/* -------------------- RCC REGISTERS -------------------- */

#define RCC_AHB1ENR           (*(volatile unsigned long *)(RCC_BASE + 0x30))
#define RCC_APB2ENR           (*(volatile unsigned long *)(RCC_BASE + 0x44))

/* -------------------- GPIO REGISTERS -------------------- */

#define GPIOD_MODER           (*(volatile unsigned long *)(GPIOD_BASE + 0x00))
#define GPIOD_OTYPER          (*(volatile unsigned long *)(GPIOD_BASE + 0x04))
#define GPIOD_OSPEEDR         (*(volatile unsigned long *)(GPIOD_BASE + 0x08))
#define GPIOD_PUPDR           (*(volatile unsigned long *)(GPIOD_BASE + 0x0C))
#define GPIOD_ODR             (*(volatile unsigned long *)(GPIOD_BASE + 0x14))

#define GPIOA_MODER           (*(volatile unsigned long *)(GPIOA_BASE + 0x00))

/* -------------------- SYSTICK BASE -------------------- */

#define SYSTICK_BASE          0xE000E010U

/* -------------------- SYSTICK REGISTERS -------------------- */

#define SYSTICK_CTRL          (*(volatile unsigned long *)(SYSTICK_BASE + 0x00))
#define SYSTICK_LOAD          (*(volatile unsigned long *)(SYSTICK_BASE + 0x04))
#define SYSTICK_VAL           (*(volatile unsigned long *)(SYSTICK_BASE + 0x08))

/* -------------------- SYSCFG -------------------- */

#define SYSCFG_BASE           0x40013800U
#define SYSCFG_EXTICR1        (*(volatile unsigned long *)(SYSCFG_BASE + 0x08))

/* -------------------- EXTI -------------------- */

#define EXTI_BASE             0x40013C00U
#define EXTI_IMR              (*(volatile unsigned long *)(EXTI_BASE + 0x00))
#define EXTI_RTSR             (*(volatile unsigned long *)(EXTI_BASE + 0x08))
#define EXTI_FTSR             (*(volatile unsigned long *)(EXTI_BASE + 0x0C))
#define EXTI_PR               (*(volatile unsigned long *)(EXTI_BASE + 0x14))

/* -------------------- NVIC -------------------- */

#define NVIC_ISER0            (*(volatile unsigned long *)(0xE000E100))

#endif
