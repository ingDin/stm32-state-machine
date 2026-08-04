/**
 * @file guards.h
 * @brief Guard function interface for FSM transitions.
 *
 * Guard functions provide optional boolean conditions evaluated during
 * state machine transitions. A transition is permitted only if:
 *   - the current state matches the transition rule
 *   - the triggering event matches
 *   - the guard function returns true
 *
 * This mechanism enables flexible and testable transition logic without
 * embedding additional conditions directly into the FSM table.
 */

#ifndef GUARDS_H
#define GUARDS_H

#include <stdbool.h>

/**
 * @brief Transition guard for button‑driven FSM events.
 *
 * Determines whether a transition triggered by a button press should be
 * allowed. The implementation in guards.c may validate conditions such as:
 *   - debounced button state
 *   - timing constraints
 *   - application‑specific rules
 *
 * @return true if the transition is permitted, false otherwise.
 */
bool tguard(void);

#endif /* GUARDS_H */
