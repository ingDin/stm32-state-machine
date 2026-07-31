/**
 * @file app.c
 * @brief Core application loop integrating button and FSM updates.
 *
 * Provides initialization and a periodic tick function used to process
 * debounced button events and state‑machine logic.
 */

#include "app.h"
#include "button.h"
#include "state_machine.h"

#if defined(TEST)
#include "fake_hal.h"
#endif

void app_init(void)
{
#if defined(TEST)
    fake_hal_reset();
#endif
    button_init();
    button_set_callback(sm_handle_event);
    sm_init();
}

void app_tick(void)
{
    button_tick(); // debounce + push events into fake_fsm
    sm_tick();     // FSM periodic actions
}

void app_run(void)
{
    while (1)
    {
        app_tick();
    }
}
