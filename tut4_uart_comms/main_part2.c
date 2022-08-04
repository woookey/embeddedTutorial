#include "clocks/clocks.h"
#include "comms/comms.h"
#include "stm32f4xx_hal.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/// variables and definitions for running tasks
#define TASKS_FREQUENCY_IN_MS (uint16_t)10
volatile uint16_t time_counter = 0;
volatile bool background_processed = false;

static GPIO_InitTypeDef led_blinky_gpio = {
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

int main(void) {
    /// enable PLL, and clock for an LED
    clocks_initialise();

    comms_init();

    /// initialise relevant GPIOs
    HAL_GPIO_Init(GPIOD, &led_blinky_gpio);
    HAL_GPIO_Init(GPIOC, &measure_gpio);
    HAL_GPIO_Init(GPIOD, &led_blue_gpio);

    while(1) {
        if (!background_processed) {
            /// run all background tasks at TASKS_FREQUENCY_IN_MS frequency
            HAL_GPIO_TogglePin(GPIOC, measure_gpio.Pin);
            HAL_GPIO_TogglePin(GPIOD, led_blinky_gpio.Pin);
            comms_handle();
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

void comms_process_cmd(comms_cmd_t* current_cmd) {
    /// assert to check for non-null
    switch(current_cmd->id) {
        case COMMS_CMD_TURN_LED_ON: {
            HAL_GPIO_WritePin(GPIOD, led_blue_gpio.Pin, GPIO_PIN_SET);
            break;
        }
        case COMMS_CMD_TURN_LED_OFF: {
            HAL_GPIO_WritePin(GPIOD, led_blue_gpio.Pin, GPIO_PIN_RESET);
            break;
        }
        default: {
            /// unhandled
        }
    }
}