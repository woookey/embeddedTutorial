#include <stm32f407xx.h>
#include "clocks/clocks.h"

#include "stm32f4xx_hal.h"

static void switchLEDOn(void);
static void switchLEDOff(void);

int main(void)
{
    clocks_initialise();
    // Enable clock for GPIOD
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    GPIOD->MODER |= GPIO_MODER_MODE12_0;
	while(1) {
        switchLEDOn();
        HAL_Delay(500);
        switchLEDOff();
        HAL_Delay(500);
	}
	return 0;
}

static void switchLEDOn(void)
{
	// Set GPIOD Pin 12 to Output and switch it on
	GPIOD->ODR |= GPIO_ODR_OD12;
}

static void switchLEDOff(void) {
    // Set GPIOD Pin 12 to Output and switch it on
    GPIOD->ODR = 0;
}
