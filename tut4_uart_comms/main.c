#include "clocks/clocks.h"
#include "uart/uart.h"
#include "stm32f4xx_hal.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

volatile uint16_t time_counter = 0;

static GPIO_InitTypeDef led_gpio = {
    .Pin = GPIO_PIN_12,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_PULLDOWN,
    .Speed = GPIO_SPEED_FREQ_HIGH
};

static GPIO_InitTypeDef measure_gpio = {
    .Pin = GPIO_PIN_1,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_PULLDOWN,
    .Speed = GPIO_SPEED_FREQ_HIGH
};

static GPIO_InitTypeDef uart2_tx_gpio = {
    .Pin = GPIO_PIN_2,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
    .Alternate = GPIO_AF7_USART2
};

static GPIO_InitTypeDef uart2_rx_gpio = {
    .Pin = GPIO_PIN_3,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
    .Alternate = GPIO_AF7_USART2
};

//static char msg[] = "SAMPLE_DATA\n\r";
static char msg[] = {0b01100011};
//static char msg[] = {'A'};

int main(void)
{
    /// enable PLL, and clock for an LED
    clocks_initialise();
    HAL_GPIO_Init(GPIOA, &uart2_tx_gpio);
    HAL_GPIO_Init(GPIOA, &uart2_rx_gpio);
    uart_initialise();

    /// initialise relevant GPIOs
    HAL_GPIO_Init(GPIOD, &led_gpio);
    HAL_GPIO_Init(GPIOC, &measure_gpio);

    while(1) {}
}

void SysTick_Handler(void) {
    /// toggle C1 to measure SysTick frequency on the scope
    HAL_GPIO_TogglePin(GPIOC, measure_gpio.Pin);
    if (++time_counter == 1000) {
        HAL_GPIO_TogglePin(GPIOD, led_gpio.Pin);
        uart_send_data((uint8_t*)&msg, sizeof(msg));
        time_counter = 0;
    }
}

void HardFault_Handler(void) {
    clocks_system_reset();
}
