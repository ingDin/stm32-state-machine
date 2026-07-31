#include "button.h"
#include "fake_button_isr.h"

#if defined(TEST)
extern button_state_t btn;

void fake_button_isr(int level)
{
    btn.raw_level = level;
    btn.event_flag = 1;
}
#endif
