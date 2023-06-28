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

#include "clocks/clocks.h"
#include "comms/comms.h"
#include "comms/comms_telemetry.h"
#include "motion_sensor/motion_sensor.h"

#include "stm32f4xx_hal.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

static void telemetry_update(void);

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

static GPIO_InitTypeDef user_button = {
        .Pin = GPIO_PIN_0,
        .Mode = GPIO_MODE_INPUT,
        .Pull = GPIO_PULLDOWN,
        .Speed = GPIO_SPEED_FREQ_HIGH
};

static GPIO_InitTypeDef measure_gpio = {
    .Pin = GPIO_PIN_1,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_PULLDOWN,
    .Speed = GPIO_SPEED_FREQ_HIGH
};

static float ax, ay, az, a;
static float roll, pitch;

static float ax_filtered, ay_filtered, az_filtered, a_filtered;
static float roll_filtered, pitch_filtered;

int main(void) {
    /// enable PLL, and clock for an LED
    clocks_initialise();

    comms_init();
    motion_sensor_init();

    /// initialise relevant GPIOs
    HAL_GPIO_Init(GPIOD, &led_blinky_gpio);
    HAL_GPIO_Init(GPIOC, &measure_gpio);
    HAL_GPIO_Init(GPIOD, &led_blue_gpio);
    HAL_GPIO_Init(GPIOA, &user_button);

    /// enable SysTick
    clocks_system_start();

    while(1) {
        if (!background_processed) {
            /// run all background tasks at TASKS_FREQUENCY_IN_MS frequency
            HAL_GPIO_TogglePin(GPIOC, measure_gpio.Pin);
            HAL_GPIO_TogglePin(GPIOD, led_blinky_gpio.Pin);
            /// get sensor data
            motion_sensor_handle();

            /// calculate roll & pitch - based on raw data
            ax = motion_sensor_get_data(motion_sensor_data_acceleration_X);
            ay = motion_sensor_get_data(motion_sensor_data_acceleration_Y);
            az = motion_sensor_get_data(motion_sensor_data_acceleration_Z);
            a = sqrtf(ax*ax+ay*ay+az*az);
            pitch = asinf(ax/a);
            roll = atan2f(ay,az);

            /// calculate roll & pitch - based on filtered data
            ax_filtered = motion_sensor_get_data(motion_sensor_data_acceleration_X_RC);
            ay_filtered = motion_sensor_get_data(motion_sensor_data_acceleration_Y_RC);
            az_filtered = motion_sensor_get_data(motion_sensor_data_acceleration_Z_RC);
            a_filtered = sqrtf(ax_filtered*ax_filtered+ay_filtered*ay_filtered+az_filtered*az_filtered);
            pitch_filtered = asinf(ax_filtered/a_filtered);
            roll_filtered = atan2f(ay_filtered,az_filtered);

            /// update telemetry and send data
            telemetry_update();
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

void telemetry_update(void) {
    static telemetry_t data;
    data.uid = TELEMETRY_UID;
    data.cookie++;
    data.switch_on = (uint8_t)HAL_GPIO_ReadPin(GPIOA, user_button.Pin);
    data.acc_X = motion_sensor_get_data(motion_sensor_data_acceleration_X);
    data.acc_Y = motion_sensor_get_data(motion_sensor_data_acceleration_Y);
    data.acc_Z = motion_sensor_get_data(motion_sensor_data_acceleration_Z);
    data.acc_X_filtered = motion_sensor_get_data(motion_sensor_data_acceleration_X_RC);
    data.acc_Y_filtered = motion_sensor_get_data(motion_sensor_data_acceleration_Y_RC);
    data.acc_Z_filtered = motion_sensor_get_data(motion_sensor_data_acceleration_Z_RC);
    data.roll = roll;
    data.pitch = pitch;
    data.roll_filtered = roll_filtered;
    data.pitch_filtered = pitch_filtered;
    comms_update_telemetry(data);
}