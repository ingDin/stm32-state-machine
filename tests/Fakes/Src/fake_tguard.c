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
