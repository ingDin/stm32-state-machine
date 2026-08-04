/**
 * @file can_loopback.h
 * @brief RAM‑based CAN loopback driver for testing.
 *
 * This module provides a simple CAN backend that stores transmitted frames
 * in an internal ring buffer and returns them through receive(). It allows
 * higher‑level code (FSM, application layer) to exercise CAN logic without
 * requiring hardware support.
 *
 * Typical usage:
 *   - CAN_LOOPBACK.init()    resets the internal buffer
 *   - CAN_LOOPBACK.send()    enqueues a CAN frame
 *   - CAN_LOOPBACK.receive() dequeues the oldest pending frame
 *
 * The driver implements the generic can_driver_t interface defined in
 * can_if.h and can be substituted for hardware CAN in TEST builds.
 */

#ifndef CAN_LOOPBACK_H
#define CAN_LOOPBACK_H

#include "can_if.h"

/**
 * @brief Loopback CAN driver instance.
 *
 * Provides init(), send(), and receive() operations backed by an internal
 * RAM buffer. Suitable for deterministic unit testing.
 */
extern const can_driver_t CAN_LOOPBACK;

#endif /* CAN_LOOPBACK_H */
