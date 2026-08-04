/**
 * @file test_led.c
 * @brief Parameterized Unity tests for the LED timing module (instance-based).
 *
 * This test suite verifies two behaviors:
 *   - led_init(): correctly initializes an LED instance without toggling.
 *   - led_tick(): toggles the LED based on the configured period and fake HAL tick.
 *
 * The tests use parameterized inputs to simulate different system tick values
 * and ensure deterministic LED behavior under timing conditions.
 */

#include "unity.h"
#include "led.h"
#include "fake_hal.h"

// ---------------------------------------------------------
// Parameter structures
// ---------------------------------------------------------

typedef struct
{
    uint32_t initial_tick;
    uint32_t expected_tick_after_init;
    int expected_toggle_count;
} led_init_params_t;

typedef struct
{
    uint32_t tick_value;
    int expected_toggle_count;
} led_tick_params_t;

// ---------------------------------------------------------
// Parameterized test: led_init()
// ---------------------------------------------------------

void test_led_init_parameterized(void)
{
    led_init_params_t tests[] = {
        {.initial_tick = 0,    .expected_tick_after_init = 0,    .expected_toggle_count = 0},
        {.initial_tick = 1234, .expected_tick_after_init = 1234, .expected_toggle_count = 0},
        {.initial_tick = 9999, .expected_tick_after_init = 9999, .expected_toggle_count = 0}
    };

    const int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        // ARRANGE
        fake_hal_reset();
        fake_hal_set_tick(tests[i].initial_tick);

        Led led;

        // ACT
        led_init(&led, 500);  // period doesn't matter for init test

        // ASSERT
        TEST_ASSERT_EQUAL_UINT32(tests[i].expected_tick_after_init, led.last_tick);
        TEST_ASSERT_EQUAL_INT(tests[i].expected_toggle_count, fake_hal_get_toggle_count());
    }
}

// ---------------------------------------------------------
// Parameterized test: led_tick()
// ---------------------------------------------------------

void test_led_tick_parameterized(void)
{
    led_tick_params_t tests[] = {
        {.tick_value = 0,    .expected_toggle_count = 0},
        {.tick_value = 100,  .expected_toggle_count = 0},
        {.tick_value = 499,  .expected_toggle_count = 0},
        {.tick_value = 500,  .expected_toggle_count = 1},
        {.tick_value = 750,  .expected_toggle_count = 1},
        {.tick_value = 1000, .expected_toggle_count = 2},
        {.tick_value = 1500, .expected_toggle_count = 3}
    };

    const int num_tests = sizeof(tests) / sizeof(tests[0]);

    // ARRANGE: initialize LED instance once
    fake_hal_reset();

    Led led;
    led_init(&led, 500);  // 500ms blink period

    for (int i = 0; i < num_tests; i++)
    {
        // ARRANGE
        fake_hal_set_tick(tests[i].tick_value);

        // ACT
        led_tick(&led);

        // ASSERT
        TEST_ASSERT_EQUAL_INT(tests[i].expected_toggle_count, fake_hal_get_toggle_count());
    }
}
