#ifndef TIME_H
#define TIME_H

#include <stdint.h>

void time_init(void);          // Initialize SysTick and tick counter
uint32_t get_tick_count(void); // Return current tick count in milliseconds
void delay_ms(uint32_t ms);    // Millisecond delay using tick counter

#endif
