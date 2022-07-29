#include "clocks.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_cortex.h"

static RCC_ClkInitTypeDef rccClkInstance = {
    .ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
    .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,//100MHz
    .AHBCLKDivider = RCC_SYSCLK_DIV1,
    .APB1CLKDivider = RCC_HCLK_DIV4,//25 MHz
    .APB2CLKDivider = RCC_HCLK_DIV2,//50 MHz
};

static RCC_OscInitTypeDef clock_setup = {
    .OscillatorType = RCC_OSCILLATORTYPE_HSE,
    .HSEState = RCC_CR_HSEON,
    .PLL = {
        .PLLState = RCC_PLL_ON,
        .PLLSource = RCC_PLLSOURCE_HSE,
        .PLLM = 8,
        .PLLN = 200,
        .PLLP = RCC_PLLP_DIV2,
    }
};

void clocks_initialise(void) {
    /// configure oscillators
    HAL_RCC_OscConfig(&clock_setup);
    HAL_RCC_ClockConfig(&rccClkInstance, 5);

    /// configure SysTick
    HAL_NVIC_DisableIRQ(SysTick_IRQn);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
    HAL_NVIC_EnableIRQ(SysTick_IRQn);

    /// enable Port D (for GPIO D12 - LED)
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /// enable Port C (for GPIO C1 - measuring SysTick)
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /// enable clocks for USART1, USART1_TX(PA9) and USART1_RX(PA10)
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    /// enable DMA2
    __HAL_RCC_DMA2_CLK_ENABLE();

}

void clocks_system_reset(void) {
    /// reset the system
    HAL_NVIC_SystemReset();
}

