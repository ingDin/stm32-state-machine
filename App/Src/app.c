/**
 * @file app.c
 * @brief Application layer implementation.
 *
 * This module wires together the system components (button, FSM, CAN) and
 * drives them through a periodic tick. It initializes all modules, processes
 * debounced button events, executes FSM actions, and polls the CAN backend.
 *
 * The application layer contains no hardware‑specific logic; all low‑level
 * operations are abstracted through hal_wrapper or fake_hal in TEST mode.
 */

#include "app.h"
#include "button.h"
#include "state_machine.h"
#include "can_if.h"
#include "can_loopback.h"

#if defined(TEST)
#include "fake_hal.h"
#endif

static const can_driver_t *can = &CAN_LOOPBACK;

void app_init(void)
{
#if defined(TEST)
    fake_hal_reset();
#endif

    can->init();
    button_init();
    button_set_callback(sm_handle_event);
    sm_init();
}

void app_tick(void)
{
    button_tick();
    sm_tick();

    can_frame_t f;
    // CRX CAN
    while (can->receive(&f))
    {
        /* CAN RX frame available */
    }
}

void app_run(void)
{
    while (1)
        app_tick();
}
