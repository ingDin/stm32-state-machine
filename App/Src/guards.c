/**
 * @file tguard.c
 * @brief Default transition guard used in the FSM.
 *
 * A guard is a boolean condition evaluated during state transitions.
 * The FSM only performs a transition if:
 *
 *   - the current state matches the transition rule
 *   - the triggering event matches
 *   - the guard function returns true
 *
 * In this implementation, tguard() always returns true, meaning all
 * transitions are allowed. This keeps the FSM simple and predictable.
 *
 * In more advanced designs, tguard() could enforce timing constraints,
 * button stability, or other application-specific conditions.
 */

#include <stdbool.h>

/**
 * @brief Always-true guard function.
 *
 * @return true for all calls, allowing transitions unconditionally.
 */
bool tguard(void)
{
    return true;
}
