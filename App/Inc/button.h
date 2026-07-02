// button.h
#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>

void button_init(void);
void button_isr_handler(int raw_level);   // called from ISR
void button_tick(void);                    // called periodically (e.g., in sm_tick)
bool button_is_pressed(void);              // stable state

#endif
