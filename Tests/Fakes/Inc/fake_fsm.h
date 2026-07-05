#ifndef FAKE_FSM_H
#define FAKE_FSM_H

#include "state_machine.h"   // for event_t

void fake_fsm_reset(void);
void fake_fsm_push_event(event_t e);
event_t fake_fsm_last_event(void);

#endif // FAKE_FSM_H
