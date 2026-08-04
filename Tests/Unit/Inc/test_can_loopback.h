/**
 * @file test_can_loopback.h
 * @brief Parameterized unit tests for the CAN loopback driver.
 *
 * Provides:
 *   - test cases for send/receive behavior
 *   - parameterized test runner
 *   - reusable test vectors
 */

#ifndef TEST_CAN_LOOPBACK_H
#define TEST_CAN_LOOPBACK_H

#include "can_if.h"
#include <stdbool.h>

/**
 * @brief Structure describing one parameterized CAN test case.
 *
 * id      : CAN identifier to send
 * dlc     : number of bytes in payload
 * payload : expected payload (0–8 bytes)
 */
typedef struct {
    uint32_t id;
    uint8_t  dlc;
    uint8_t  payload[8];
} can_test_case_t;

#endif
