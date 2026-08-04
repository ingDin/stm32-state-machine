/**
 * @file app.c
 * @brief Core application loop integrating button and FSM updates.
 *
 * The application layer coordinates the interaction between:
 *   - the debounced button module (button.c)
 *   - the finite state machine controlling LED behavior (state_machine.c)
 *   - optional HAL test stubs (fake_hal.c)
 *
 * Responsibilities:
 *   - Initialize all modules used by the application
 *   - Provide a periodic tick function that updates button and FSM logic
 *   - Run the main loop in a deterministic and testable manner
 *
 * This layer intentionally contains no hardware-specific code; all hardware
 * access is abstracted through hal_wrapper.c or fake_hal.c (in TEST mode).
 */

#include "app.h"
#include "button.h"
#include "state_machine.h"

#if defined(TEST)
#include "fake_hal.h"
#endif

/**
 * @brief Initialize all application modules.
 *
 * Steps:
 *   - Reset fake HAL state (in TEST mode)
 *   - Initialize button module (debounce + ISR integration)
 *   - Register FSM as the button event callback
 *   - Initialize the finite state machine
 *
 * This function must be called once before app_run().
 */
void app_init(void)
{
#if defined(TEST)
    fake_hal_reset(); /**< Reset simulated hardware state for unit tests */
#endif

    button_init(); /**< Prepare button debounce and ISR state */

    /**
     * Connect button events directly to the FSM.
     * When the button module detects a press/release event,
     * it will call sm_handle_event(event).
     */
    button_set_callback(sm_handle_event);

    sm_init(); /**< Initialize FSM and execute entry action for initial state */
}

/**
 * @brief Single application tick.
 *
 * Performs:
 *   - button_tick(): debouncing + event generation
 *   - sm_tick(): per-state periodic FSM actions
 *
 * This function is called repeatedly inside app_run().
 */
void app_tick(void)
{
    button_tick(); /**< Debounce + push events into FSM */
    sm_tick();     /**< Execute periodic FSM actions */
}

/**
 * @brief Main application loop.
 *
 * Runs indefinitely. Calls app_tick() continuously to ensure:
 *   - button events are processed
 *   - FSM actions are executed
 *
 * This function does not return.
 */
void app_run(void)
{
    while (1)
    {
        app_tick();
    }
}
