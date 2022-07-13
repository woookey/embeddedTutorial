#include "clocks/clocks.h"
#include "comms/comms.h"
#include "stm32f4xx_hal.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

volatile uint16_t time_counter = 0;
volatile bool background_processed = false;

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

static GPIO_InitTypeDef uart1_tx_gpio = {
    .Pin = GPIO_PIN_6,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
    .Alternate = GPIO_AF7_USART1
};

static GPIO_InitTypeDef uart1_rx_gpio = {
    .Pin = GPIO_PIN_7,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
    .Alternate = GPIO_AF7_USART1
};

static char msg[] = {"Disco says hello!\r\n"};

#define TASKS_FREQUENCY_IN_MS 1000U
#define TASKS_FREQUENCY_THRESHOLD (TASKS_FREQUENCY_IN_MS-1)

int main(void)
{
    /// enable PLL, and clock for an LED
    clocks_initialise();

    /// configure uart
    HAL_GPIO_Init(GPIOB, &uart1_tx_gpio);
    HAL_GPIO_Init(GPIOB, &uart1_rx_gpio);
    comms_config_t comms_config = {
        .payload_size = 10
    };
    comms_initialise(comms_config);

    /// initialise relevant GPIOs
    HAL_GPIO_Init(GPIOD, &led_gpio);
    HAL_GPIO_Init(GPIOC, &measure_gpio);

    while(1) {
        if ((time_counter == TASKS_FREQUENCY_THRESHOLD) & !background_processed) {
            /// run all background tasks
            HAL_GPIO_TogglePin(GPIOD, led_gpio.Pin);
            comms_send_data((uint8_t*)&msg, sizeof(msg));
            background_processed = true;
        }
    }
}

void SysTick_Handler(void) {
    /// toggle C1 to measure SysTick frequency on the scope
    HAL_GPIO_TogglePin(GPIOC, measure_gpio.Pin);
    if (++time_counter == 1000) {
        time_counter = 0;
        background_processed = false;
    }
}

void HardFault_Handler(void) {
    clocks_system_reset();
}

void comms_error_cb(comms_error_t error) {
    (void)error;
}

void comms_handle_data_cb(uint8_t* payload, uint8_t payload_size) {
    (void)payload;
    (void)payload_size;
}
