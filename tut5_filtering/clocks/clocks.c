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

    /// enable Port D (for GPIO D12 - LED)
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /// enable Port A (for switch), only used for tut4-part2
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /// enable Port C (for GPIO C1 - measuring SysTick)
    __HAL_RCC_GPIOC_CLK_ENABLE();
}

void clocks_system_reset(void) {
    /// reset the system
    HAL_NVIC_SystemReset();
}

void clocks_system_start(void) {
    HAL_NVIC_EnableIRQ(SysTick_IRQn);
}

