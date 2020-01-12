#include "clocks.h"
#include "stm32f4xx_hal_rcc.h"
static RCC_ClkInitTypeDef rccClkInstance =
        {
                .ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
                .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
                .AHBCLKDivider = RCC_SYSCLK_DIV1,
                .APB1CLKDivider = RCC_HCLK_DIV4,
                .APB2CLKDivider = RCC_HCLK_DIV2,
        };

static RCC_OscInitTypeDef clock_setup = {
        .OscillatorType = RCC_OSCILLATORTYPE_HSE,
        .HSEState = RCC_CR_HSEON,
        .PLL = {
                .PLLState = RCC_PLL_ON,
                .PLLSource = RCC_PLLSOURCE_HSE,
                .PLLM = 8,
                .PLLN = 336,
                .PLLP = RCC_PLLP_DIV2,
        }
};

void clocks_initialise(void) {
    HAL_RCC_OscConfig(&clock_setup);
    HAL_RCC_ClockConfig(&rccClkInstance, 5);
}

