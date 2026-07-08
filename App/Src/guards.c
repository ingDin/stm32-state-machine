/**
 * @file tguard.c
 * @brief Always‑true timing guard for non‑test builds.
 */

#include <stdbool.h>

bool tguard(void)
{
    return true;
}
