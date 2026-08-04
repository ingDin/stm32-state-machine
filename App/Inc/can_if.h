/**
 * @file can_if.h
 * @brief Generic CAN interface abstraction.
 *
 * This module defines the standard CAN frame format and the abstract
 * driver interface used by all CAN backends. Higher‑level code (FSM,
 * application layer) interacts only with can_driver_t, allowing seamless
 * switching between loopback, hardware CAN, or mock implementations.
 */

#ifndef CAN_IF_H
#define CAN_IF_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Standard CAN frame structure.
 *
 * Represents a single CAN message with identifier, payload length,
 * and up to 8 bytes of data.
 */
typedef struct {
    uint32_t id;      /**< CAN identifier (standard or extended) */
    uint8_t  dlc;     /**< Number of valid bytes in the payload (0–8) */
    uint8_t  data[8]; /**< Payload buffer */
} can_frame_t;

/**
 * @brief Abstract CAN driver interface.
 *
 * Each backend must implement the following operations:
 *   - init()    : initialize the CAN subsystem
 *   - send()    : transmit a CAN frame
 *   - receive() : retrieve a pending CAN frame (non‑blocking)
 *
 * The application and FSM use this interface without knowing the
 * underlying driver type.
 */
typedef struct {
    bool (*init)(void);
    bool (*send)(const can_frame_t *frame);
    bool (*receive)(can_frame_t *out);
} can_driver_t;

#endif /* CAN_IF_H */
