#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum {
    STATE_OFF,
    STATE_ON
} state_t;

void sm_init(void);
state_t sm_get_state(void);

#endif
