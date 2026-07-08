#ifndef FAKE_TGUARD_H
#define FAKE_TGUARD_H

#include <stdbool.h>

void fake_tguard_set(bool value);
void fake_tguard_reset(void);

bool tguard(void);

#endif // FAKE_TGUARD_H
