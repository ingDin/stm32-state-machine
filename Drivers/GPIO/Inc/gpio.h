#ifndef LED_H
#define LED_H

void gpio_init(void);
void gpio_toggle(void);
void gpio_write(int state);

#endif
