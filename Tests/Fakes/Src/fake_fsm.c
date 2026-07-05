/**
 * @file fake_fsm.c
 * @brief Test double for the FSM event queue used in unit tests.
 *
 * This module provides a minimal fake implementation of the FSM event
 * interface. Tests can push events with fake_fsm_push_event() and inspect
 * the last received event via fake_fsm_last_event(). The state can be reset
 * to EVENT_NONE using fake_fsm_reset().
 *
 * The real FSM is replaced by this predictable stub to ensure deterministic
 * event handling during unit testing.
 */

 
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
