#ifndef TUT2_GPIO_H
#define TUT2_GPIO_H

#include <stdbool.h>

typedef struct gpio_def_t* gpio_t;

extern gpio_t gpio_led;

void gpio_init(const gpio_t gpio_object);
void gpio_switch(const gpio_t gpio_object, bool turn_on);
void gpio_toggle(const gpio_t gpio_object);

#endif
