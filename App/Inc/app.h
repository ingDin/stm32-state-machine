/**
 * @file app.h
 * @brief Application control layer: initialization and periodic updates.
 *
 * The application module coordinates the system’s high‑level behavior by
 * initializing all components and providing a periodic tick function that
 * updates button logic and the finite state machine (FSM).
 *
 * Integrated modules:
 *   - Button handler (debounce + event generation)
 *   - FSM (LED behavior control)
 *   - LED driver (ON/OFF/blink actions)
 *   - HAL or fake HAL (depending on build mode)
 *
 * All hardware access is abstracted through hal_wrapper.c or fake_hal.c
 * in TEST mode, keeping the app layer platform‑independent.
 */

#ifndef APP_H
#define APP_H

/**
 * @brief Initialize all application modules.
 *
 * Performs:
 *   - HAL reset (in TEST mode)
 *   - CAN driver initialization
 *   - Button initialization + FSM event callback registration
 *   - FSM initialization and entry action execution
 */
void app_init(void);

/**
 * @brief Single periodic application tick.
 *
 * Executes:
 *   - button_tick(): debouncing + event dispatch
 *   - sm_tick(): per‑state FSM actions
 *   - CAN RX polling via can->receive()
 */
void app_tick(void);

/**
 * @brief Main application loop.
 *
 * Repeatedly calls app_tick() to process button events and FSM actions.
 */
void app_run(void);

#endif /* APP_H */
