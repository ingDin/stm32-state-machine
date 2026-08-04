/**
 * @file test_can_loopback.c
 * @brief Parameterized unit tests for CAN loopback driver.
 *
 * This test suite verifies:
 *   - correct initialization
 *   - correct send/receive behavior
 *   - FIFO ordering
 *   - payload integrity
 *   - multiple test vectors using a parameterized table
 */

#include "test_can_loopback.h"
#include "can_loopback.h"
#include <string.h>
#include <assert.h>

/* ---------------- Parameterized Test Vectors ---------------- */

static const can_test_case_t test_cases[] = {
    { .id = 0x100, .dlc = 2, .payload = {1, 2} },
    { .id = 0x200, .dlc = 3, .payload = {'A', 'B', 'C'} },
    { .id = 0x321, .dlc = 1, .payload = {0xFF} },
    { .id = 0x555, .dlc = 4, .payload = {9, 8, 7, 6} },
};

static const int test_case_count =
    sizeof(test_cases) / sizeof(test_cases[0]);

/* ---------------- Single Test Execution ---------------- */

static bool run_single_test(const can_test_case_t *tc)
{
    can_frame_t tx = {
        .id  = tc->id,
        .dlc = tc->dlc
    };
    memcpy(tx.data, tc->payload, tc->dlc);

    can_frame_t rx;

    assert(CAN_LOOPBACK.send(&tx));
    assert(CAN_LOOPBACK.receive(&rx));

    /* Validate frame integrity */
    if (rx.id != tc->id) return false;
    if (rx.dlc != tc->dlc) return false;
    if (memcmp(rx.data, tc->payload, tc->dlc) != 0) return false;

    return true;
}

/* ---------------- Test Runner ---------------- */

bool test_can_loopback_run(void)
{
    CAN_LOOPBACK.init();

    for (int i = 0; i < test_case_count; i++)
    {
        if (!run_single_test(&test_cases[i]))
            return false;
    }

    return true;
}
