#ifndef GUARDS_H
#define GUARDS_H

#include <stdbool.h>

/**
 * @brief External guard used by the state machine.
 *
 * This function is referenced in the transition table inside state_machine.c.
 * The real implementation lives in guards.c.
 * The fake implementation (for unit tests) lives in fake_tguard.c.
 */
bool tguard(void);

#endif
