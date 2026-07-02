#include "fake_fsm.h"

static event_t last_event = EVENT_NONE;

void fake_fsm_reset(void)
{
    last_event = EVENT_NONE;
}

void fake_fsm_push_event(event_t e)
{
    last_event = e;
}

event_t fake_fsm_last_event(void)
{
    return last_event;
}
