#include "clocks/clocks.h"
#include "comms_driver/comms_driver.h"
#include "stm32f4xx_hal.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/// variables and definitions for running tasks
#define TASKS_FREQUENCY_IN_MS (uint16_t)10
volatile uint16_t time_counter = 0;
volatile bool background_processed = false;

static GPIO_InitTypeDef led_gpio = {
    .Pin = GPIO_PIN_12,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_PULLDOWN,
    .Speed = GPIO_SPEED_FREQ_HIGH
};

static GPIO_InitTypeDef led_blue_gpio = {
        .Pin = GPIO_PIN_15,
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

/// data exchange
#define INCOMING_PAYLOAD_SIZE (uint8_t)2
#define COMMS_BAUDRATE (uint32_t)115200
#define CMD_TURN_BLUE_LED_ON (uint16_t)0x5841
#define CMD_TURN_BLUE_LED_OFF (uint16_t)0x4659
static char msg[] = {"Beep!\r\n"};

int main(void) {
    /// enable PLL, and clock for an LED
    clocks_initialise();

    /// configure comms driver
    HAL_GPIO_Init(GPIOB, &uart1_tx_gpio);
    HAL_GPIO_Init(GPIOB, &uart1_rx_gpio);
    comms_driver_config_t comms_driver_config = {
        .payload_size = INCOMING_PAYLOAD_SIZE,
        .baudrate = COMMS_BAUDRATE,
        .parity = COMMS_DRIVER_PARITY_ODD,
        .mode = COMMS_DRIVER_MODE_DMA_IT,
    };
    comms_driver_initialise(comms_driver_config);

    /// initialise relevant GPIOs
    HAL_GPIO_Init(GPIOD, &led_gpio);
    HAL_GPIO_Init(GPIOC, &measure_gpio);
    HAL_GPIO_Init(GPIOD, &led_blue_gpio);

    while(1) {
        if (!background_processed) {
            /// run all background tasks at TASKS_FREQUENCY_IN_MS frequency
            HAL_GPIO_TogglePin(GPIOC, measure_gpio.Pin);
            HAL_GPIO_TogglePin(GPIOD, led_gpio.Pin);
            comms_driver_send_data((uint8_t*)&msg, sizeof(msg));
            background_processed = true;
        }
    }
}

void SysTick_Handler(void) {
    /// toggle C1 to measure SysTick frequency on the scope
    if (++time_counter == TASKS_FREQUENCY_IN_MS) {
        time_counter = 0;
        background_processed = false;
    }
}

void HardFault_Handler(void) {
    clocks_system_reset();
}

void comms_driver_error_cb(comms_driver_error_t error) {
    (void)error;
}

void comms_driver_handle_data_cb(uint8_t* payload, uint8_t payload_size) {
    static uint16_t cmd;
    if (payload_size == INCOMING_PAYLOAD_SIZE) {
        cmd = ((payload[0] << 8) | payload[1]);
        switch (cmd) {
            case CMD_TURN_BLUE_LED_ON: {
                /// correspond to "XA"
                HAL_GPIO_WritePin(GPIOD, led_blue_gpio.Pin, GPIO_PIN_SET);
                break;
            }
            case CMD_TURN_BLUE_LED_OFF: {
                /// correspond to "FY"
                HAL_GPIO_WritePin(GPIOD, led_blue_gpio.Pin, GPIO_PIN_RESET);
                break;
            }
        }
    }
}