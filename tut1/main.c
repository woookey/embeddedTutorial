#include <stm32f407xx.h>

static void switchLEDOn(void);

int main(void)
{
	switchLEDOn();
	return 0;
}

static void switchLEDOn(void)
{
	// Enable clock for GPIOD
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	// Set GPIOD Pin 12 to Output and switch it on
	GPIOD->MODER |= GPIO_MODER_MODE12_0;
	GPIOD->ODR |= GPIO_ODR_OD12;
}
