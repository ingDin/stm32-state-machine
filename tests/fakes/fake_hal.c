/**
 * @file fake_hal.c
 * @brief Fake implementation of HAL_GetTick() for unit testing.
 *
 * This file provides a **FAKE** version of HAL_GetTick(), used when running
 * unit tests on a host machine (e.g., MinGW on Windows) where the real STM32
 * HAL is not available.
 *
 * What makes this a FAKE:
 * - It implements a **working, simplified substitute** for the real HAL tick
 *   counter.
 * - It maintains internal state (`fake_tick`) that behaves similarly to the
 *   real system tick.
 * - Tests can **manually control time progression** using fake_hal_set_tick().
 * - It enables deterministic, non-blocking timing tests (blink, debounce,
 *   timeouts) without requiring the actual hardware timer.
 *
 * Why this is not a stub:
 * - A stub would return a fixed value. This fake allows time to advance.
 *
 * Why this is not a strict mock:
 * - It does not verify expectations or call counts.
 * - It only provides functional behavior needed by the state machine.
 *
 * This approach follows standard embedded TDD practices: provide a lightweight,
 * deterministic replacement for hardware-dependent functions.
 */

#include <stdint.h>
#include "fake_hal.h"

// ---------------------------------------------------------
// Fake tick implementation
// ---------------------------------------------------------

static uint32_t fake_tick = 0;

uint32_t HAL_GetTick(void) {
    return fake_tick;
}

void fake_hal_set_tick(uint32_t t) {
    fake_tick = t;
}

// ---------------------------------------------------------
// Fake LED toggle tracking
// ---------------------------------------------------------

static int fake_toggle_count = 0;

// Reset fake HAL state (tick + toggle counter)
void fake_hal_reset(void) {
    fake_tick = 0;
    fake_toggle_count = 0;
}

// Fake GPIO toggle function
void HAL_GPIO_TogglePin(void* GPIOx, uint16_t GPIO_Pin) {
    fake_toggle_count++;
}

// Getter for toggle count
int fake_hal_get_toggle_count(void) {
    return fake_toggle_count;
}
