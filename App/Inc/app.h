/**
 * @file app.h
 * @brief High‑level application interface.
 *
 * The application module coordinates system initialization and the main
 * execution loop. It integrates lower‑level modules such as:
 *
 *   - LED driver (heartbeat LED)
 *   - Finite state machine (LED behavior control)
 *   - User button handler (event generation)
 *   - HAL wrapper (hardware abstraction)
 *
 * The app layer provides a clean separation between hardware setup
 * performed in main.c and the continuous processing performed in app_run().
 *
 * Functions:
 *   - app_init() : Initializes all application modules.
 *   - app_tick() : Optional periodic tick (unused in current design).
 *   - app_run()  : Main application loop.
 */

#ifndef APP_H
#define APP_H

/**
 * @brief Initialize all application modules.
 *
 * Sets up heartbeat LED, initializes the finite state machine,
 * and prepares input modules such as the user button.
 */
void app_init(void);

/**
 * @brief Optional periodic tick for the application.
 *
 * Currently unused, but reserved for future expansion (e.g., timers,
 * cooperative scheduling, or background tasks).
 */
void app_tick(void);

/**
 * @brief Main application loop.
 *
 * Runs indefinitely. Processes:
 *   - heartbeat LED ticks
 *   - state machine ticks
 *   - user button debouncing and event generation
 *
 * This function does not return.
 */
void app_run(void);

#endif /* APP_H */
