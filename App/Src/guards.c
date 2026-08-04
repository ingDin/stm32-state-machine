/**
 * @file tguard.c
 * @brief Default FSM transition guard implementation.
 *
 * This module provides the simplest possible guard function: an
 * unconditional check that always returns true. It keeps transitions
 * unrestricted and allows the FSM to operate deterministically while
 * preserving the guard mechanism for future extensions.
 */

#include <stdbool.h>

/**
 * @brief Unconditional guard function.
 *
 * Always returns true, allowing all transitions defined in the FSM table.
 */
bool tguard(void)
{
    return true;
}
