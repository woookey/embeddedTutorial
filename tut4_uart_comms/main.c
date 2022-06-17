#include "clocks/clocks.h"
#include "stm32f4xx_hal.h"

#include <stdbool.h>
#include <stddef.h>

#define RESERVED_MEMORY (*(uint32_t*)0x08100000)

volatile uint16_t time_counter = 0;
static GPIO_InitTypeDef led_gpio = {
        .Pin = GPIO_PIN_12,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLDOWN,
        .Speed = GPIO_SPEED_FREQ_HIGH,
};

static GPIO_InitTypeDef measure_gpio = {
        .Pin = GPIO_PIN_1,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLDOWN,
        .Speed = GPIO_SPEED_FREQ_HIGH,
};

int main(void)
{
    /// enable PLL, and clock for an LED
    clocks_initialise();

    /// initialise LED GPIO (D12)
    HAL_GPIO_Init(GPIOD, &led_gpio);
    HAL_GPIO_Init(GPIOC, &measure_gpio);
    while(1) {}
}

void SysTick_Handler(void) {
    /// toggle C1 to measure SysTick frequency on the scope
    HAL_GPIO_TogglePin(GPIOC, measure_gpio.Pin);
    if (++time_counter == 100) {
        HAL_GPIO_TogglePin(GPIOD, led_gpio.Pin);
        time_counter = 0;
    }
    /// forbidden write to reserved memory
    /// uncomment line below to inject a bug
    /// RESERVED_MEMORY = NULL;
}

void HardFault_Handler(void) {
    clocks_system_reset();
}
