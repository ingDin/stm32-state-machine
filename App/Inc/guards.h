/**
 * @file guards.h
 * @brief Guard function interface for FSM transitions.
 *
 * Guards are optional boolean conditions evaluated during state machine
 * transitions. A transition is allowed only if:
 *
 *   - the current state matches the transition rule
 *   - the triggering event matches
 *   - the guard function returns true
 *
 * This mechanism enables flexible and testable transition logic.
 *
 * In the current design, tguard() is used to validate button‑driven
 * transitions (e.g., ensuring the button press is legitimate).
 */

#ifndef GUARDS_H
#define GUARDS_H

#include <stdbool.h>

/**
 * @brief Transition guard for button‑driven FSM events.
 *
 * This function determines whether a transition triggered by a button
 * press should be allowed. The exact logic is implemented in guards.c
 * and may include checks such as:
 *
 *   - debounced button state
 *   - timing constraints
 *   - additional application‑specific conditions
 *
 * @return true if the transition is permitted, false otherwise.
 */
bool tguard(void);

#endif /* GUARDS_H */
