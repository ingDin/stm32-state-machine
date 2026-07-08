/**
 * @file fake_hal.c
 * @brief Minimal HAL simulation for unit testing.
 *
 * Provides deterministic replacements for:
 *   - hal_get_tick()
 *   - hal_toggle_led()
 *
 * Used by the state machine tests to control time progression
 * and count LED toggles without hardware dependencies.
 */

#include <stdint.h>
#include "fake_hal.h"
#include "hal_wrapper.h"

// ---------------------------------------------------------
// Fake tick implementation
// ---------------------------------------------------------

static uint32_t fake_tick = 0;

uint32_t hal_get_tick(void)
{
    return fake_tick;
}

void fake_hal_set_tick(uint32_t t)
{
    fake_tick = t;
}

// ---------------------------------------------------------
// Fake LED toggle tracking
// ---------------------------------------------------------

static int fake_toggle_count = 0;

// Reset fake HAL state (tick + toggle counter)
void fake_hal_reset(void)
{
    fake_tick = 0;
    fake_hal_reset_toggle_count();
}

void fake_hal_reset_toggle_count(void)
{
    fake_toggle_count = 0;
}

// Fake GPIO toggle function
void hal_toggle_led(void)
{
    fake_toggle_count++;
}

// Getter for toggle count
int fake_hal_get_toggle_count(void)
{
    return fake_toggle_count;
}

void hal_write_led(int state)
{
    // no-action
}