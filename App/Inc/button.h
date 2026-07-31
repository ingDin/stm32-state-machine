#ifndef BUTTON_H
#define BUTTON_H
#include <stdbool.h>
#include "state_machine.h" // for event_t

typedef struct
{
    int raw_level;
    int stable_state;
    int debounce;
    volatile int event_flag;
} button_state_t;

#ifdef TEST
extern button_state_t btn;
#endif

typedef void (*button_event_cb_t)(event_t event);

void button_init(void);
void button_isr_handler();
void button_tick(void);
bool button_is_pressed(void);

/* NEW: callback injection */
void button_set_callback(button_event_cb_t cb);
#endif
