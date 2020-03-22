#include "gpio.h"
#include <stdint.h>
#include "stm32f4xx_hal_gpio.h"

struct gpio_def_t {
    GPIO_TypeDef *port;
    GPIO_InitTypeDef config;
};

static struct gpio_def_t gpio_led_object = {
        .port = GPIOD,
        .config = {
                .Pin = GPIO_PIN_12,
                .Mode = GPIO_MODE_OUTPUT_PP,
                .Pull = GPIO_PULLDOWN,
                .Speed = GPIO_SPEED_FREQ_HIGH,
        },
};
gpio_t gpio_led = (struct gpio_def_t*)&gpio_led_object;

void gpio_init(const gpio_t gpio_object) {
    HAL_GPIO_Init(gpio_object->port, &gpio_object->config);
}

void gpio_switch(const gpio_t gpio_object, bool turn_on) {
    GPIO_PinState set_state = turn_on ? GPIO_PIN_SET : GPIO_PIN_RESET;
    HAL_GPIO_WritePin(gpio_object->port, gpio_object->config.Pin, set_state);
}

void gpio_toggle(const gpio_t gpio_object) {
    HAL_GPIO_TogglePin(gpio_object->port, gpio_object->config.Pin);
}