#include "stm32f4xx_hal.h"

#include "clocks/clocks.h"
#include "gpio/gpio.h"

#include <stdbool.h>

int main(void)
{
    /**
     * Enable PLL, and clock for an LED
     */
    clocks_initialise();

    gpio_init(gpio_led);
	while(1) {
        gpio_toggle(gpio_led);
        HAL_Delay(500);
	}
}

void SysTick_Handler(void) {
    HAL_IncTick();
}