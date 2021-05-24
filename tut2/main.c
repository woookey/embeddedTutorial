#include "clocks/clocks.h"
#include "stm32f4xx_hal.h"

#include <stdbool.h>

int main(void)
{
    /// Enable PLL, and clock for an LED
    clocks_initialise();

    GPIO_InitTypeDef led_gpio = {
        .Pin = GPIO_PIN_12,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLDOWN,
        .Speed = GPIO_SPEED_FREQ_HIGH,
    };
    HAL_GPIO_Init(GPIOD, &led_gpio);
	while(1) {
        HAL_GPIO_TogglePin(GPIOD, led_gpio.Pin);
        HAL_Delay(500);
	}
}

void SysTick_Handler(void) {
    HAL_IncTick();
}