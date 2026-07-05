#include "unity.h"
#include "led.h"
#include "fake_hal.h"

// ---------------------------------------------------------
// Parameter structures
// ---------------------------------------------------------

typedef struct {
    uint32_t initial_tick;
    uint32_t expected_tick_after_init;
    int expected_toggle_count;
} led_init_params_t;

typedef struct {
    uint32_t tick_value;
    int expected_toggle_count;
} led_tick_params_t;

// ---------------------------------------------------------
// Parameterized test: led_init()
// ---------------------------------------------------------

void test_led_init_parameterized(void)
{

    led_init_params_t tests[] = {
        { .initial_tick = 0,    .expected_tick_after_init = 0, .expected_toggle_count = 0 },
        { .initial_tick = 1234, .expected_tick_after_init = 1234, .expected_toggle_count = 0 },
        { .initial_tick = 9999, .expected_tick_after_init = 9999, .expected_toggle_count = 0 }
    };

    const int num_tests = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < num_tests; i++)
    {
        // ARRANGE: prepare fake HAL state
        fake_hal_reset();
        fake_hal_set_tick(tests[i].initial_tick);

        // ACT: initialize LED module
        led_init();

        // ASSERT: verify tick reset and toggle counter
        TEST_ASSERT_EQUAL_UINT32(tests[i].expected_tick_after_init, HAL_GetTick());
        TEST_ASSERT_EQUAL_INT(tests[i].expected_toggle_count, led_get_toggle_count());
    }
}

// ---------------------------------------------------------
// Parameterized test: led_tick()
// ---------------------------------------------------------

void test_led_tick_parameterized(void)
{
    led_tick_params_t tests[] = {
        { .tick_value = 0,    .expected_toggle_count = 0 },
        { .tick_value = 100,  .expected_toggle_count = 0 },
        { .tick_value = 499,  .expected_toggle_count = 0 },
        { .tick_value = 500,  .expected_toggle_count = 1 },
        { .tick_value = 750,  .expected_toggle_count = 1 },
        { .tick_value = 1000, .expected_toggle_count = 2 },
        { .tick_value = 1500, .expected_toggle_count = 3 }
    };

    const int num_tests = sizeof(tests) / sizeof(tests[0]);

    // ARRANGE: initialize LED module once
    fake_hal_reset();
    led_init();

    for (int i = 0; i < num_tests; i++)
    {
        // ARRANGE: set fake tick value
        fake_hal_set_tick(tests[i].tick_value);

        // ACT: process LED tick
        led_tick();

        // ASSERT: verify expected toggle count
        TEST_ASSERT_EQUAL_INT(tests[i].expected_toggle_count, led_get_toggle_count());
    }
}
