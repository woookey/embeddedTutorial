#include "clocks.h"
#include "stm32f4xx_hal_rcc.h"

static RCC_OscInitTypeDef clock_setup = {
        .OscillatorType = RCC_OSCILLATORTYPE_HSE,
        .HSEState = RCC_CR_HSEON,
        .PLL = {
                .PLLState = RCC_PLL_ON,
                .PLLSource = RCC_PLLSOURCE_HSE,
                .PLLM = 8,
                .PLLN = 50,
                .PLLP = RCC_PLLP_DIV2,
        }
};

void clocks_initialise(void) {
    HAL_RCC_OscConfig(&clock_setup);
}