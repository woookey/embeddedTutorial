// MIT License
//
// Copyright (c) 2022 Actuated Robots Ltd., Lukasz Barczyk
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
