/**
 * @file can_loopback.c
 * @brief RAM‑based CAN loopback driver implementation.
 *
 * This module implements a simple CAN backend that stores transmitted
 * frames in an internal ring buffer and returns them through receive().
 * It provides deterministic behavior for unit tests and allows higher‑level
 * modules to exercise CAN logic without requiring hardware support.
 *
 * The driver conforms to the generic can_driver_t interface and is used
 * as a drop‑in replacement for hardware CAN in TEST builds.
 */

#include "can_if.h"

#define CAN_BUFFER_SIZE 16

static can_frame_t buffer[CAN_BUFFER_SIZE];
static uint8_t head = 0;
static uint8_t tail = 0;

static bool loop_init(void)
{
    head = tail = 0;
    return true;
}

static bool loop_send(const can_frame_t *frame)
{
    uint8_t next = (head + 1) % CAN_BUFFER_SIZE;
    if (next == tail)
        return false; // buffer full

    buffer[head] = *frame;
    head = next;
    return true;
}

static bool loop_receive(can_frame_t *out)
{
    if (tail == head)
        return false;

    *out = buffer[tail];
    tail = (tail + 1) % CAN_BUFFER_SIZE;
    return true;
}

const can_driver_t CAN_LOOPBACK = {
    .init = loop_init,
    .send = loop_send,
    .receive = loop_receive
};
