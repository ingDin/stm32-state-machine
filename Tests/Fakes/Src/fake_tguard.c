/**
 * @file fake_tguard.c
 * @brief Test double for tguard() used in unit testing.
 *
 * Allows tests to control the guard value via fake_tguard_set() and reset it
 * with fake_tguard_reset(), ensuring deterministic timing behavior.
 */

#include "fake_tguard.h"

static bool fake_value = true;

void fake_tguard_set(bool value)
{
    fake_value = value;
}

void fake_tguard_reset(void)
{
    fake_value = true;
}

bool tguard(void)
{
    return fake_value;
}
