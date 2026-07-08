#ifndef BUTTON_H
#define BUTTON_H
#include <stdbool.h>
#include "state_machine.h"   // for event_t

typedef void (*button_event_cb_t)(event_t event);

void button_init(void);
void button_isr_handler(int level);
void button_tick(void);
bool button_is_pressed(void);

/* NEW: callback injection */
void button_set_callback(button_event_cb_t cb);
#endif