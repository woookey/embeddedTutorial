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

#include "motion_sensor_driver.h"

#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_ll_spi.h"

#include <stdbool.h>

static GPIO_InitTypeDef spi1_sck_gpio = {
    .Pin = GPIO_PIN_5,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
    .Alternate = GPIO_AF5_SPI1,
};

static GPIO_InitTypeDef spi1_mosi_gpio = {
    .Pin = GPIO_PIN_7,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
    .Alternate = GPIO_AF5_SPI1,
};

static GPIO_InitTypeDef spi1_miso_gpio = {
    .Pin = GPIO_PIN_6,
    .Mode = GPIO_MODE_AF_PP,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
    .Alternate = GPIO_AF5_SPI1,
};

static GPIO_InitTypeDef spi1_cs_gpio = {
    .Pin = GPIO_PIN_3,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
};

static SPI_HandleTypeDef spi_handle = {
    .Instance = SPI1,
    .Init = {
            .Mode = SPI_MODE_MASTER,
            .Direction = SPI_DIRECTION_2LINES,
            .DataSize = SPI_DATASIZE_8BIT,
            .CLKPolarity = SPI_POLARITY_LOW,
            .CLKPhase = SPI_PHASE_1EDGE,
            .NSS = SPI_NSS_SOFT,
            .BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2,
            .FirstBit = SPI_FIRSTBIT_MSB,
            .TIMode = SPI_TIMODE_DISABLE,
            .CRCCalculation = SPI_CRCCALCULATION_DISABLE,
            .CRCPolynomial = 0,
    },
};

/*static DMA_HandleTypeDef spi_dma_rx_handle = {
        .Instance = DMA1_Stream0,
        .Init = {
            .Channel = DMA_CHANNEL_0,
            .Direction = DMA_PERIPH_TO_MEMORY,
            .PeriphInc = DMA_PINC_DISABLE,
            .MemInc = DMA_MINC_ENABLE,
            .PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
            .MemDataAlignment = DMA_MDATAALIGN_BYTE,
            .Mode = DMA_NORMAL,
            .Priority = DMA_PRIORITY_VERY_HIGH,
            .FIFOMode = DMA_FIFOMODE_DISABLE,
            .FIFOThreshold = DMA_FIFO_THRESHOLD_FULL,
            .MemBurst = DMA_MBURST_SINGLE,
            .PeriphBurst = DMA_PBURST_SINGLE,
        }
};*/


typedef enum {
    operation_mode_none = 0U,
    operation_mode_interrupts,
    operation_mode_DMA,
} operation_mode_t;

#define TX_BUFFER_SIZE 10U
#define RX_BUFFER_SIZE 10U
typedef struct {
    bool initialised;
    bool busy;
    void (*read_cb)(uint8_t* data, uint16_t data_length);
    operation_mode_t operation_mode;
    uint16_t tx_to_send;
    uint16_t tx_iterator;
    uint8_t tx_buffer[TX_BUFFER_SIZE];
    uint16_t rx_to_receive;
    uint16_t rx_iterator;
    uint8_t rx_buffer[RX_BUFFER_SIZE];
} motion_sensor_driver_t;
static motion_sensor_driver_t motion_sensor_data;

void motion_sensor_driver_init(void (*read_callback)(uint8_t* data, uint16_t data_length)) {
    /// enable SPI1 interface clock
    __HAL_RCC_SPI1_CLK_ENABLE();

    /// enable clocks for GPIOA and GPIOE
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    /// initialise SPI1 pins
    HAL_GPIO_Init(GPIOA, &spi1_sck_gpio);
    HAL_GPIO_Init(GPIOA, &spi1_mosi_gpio);
    HAL_GPIO_Init(GPIOA, &spi1_miso_gpio);
    HAL_GPIO_Init(GPIOE, &spi1_cs_gpio);
    HAL_GPIO_WritePin(GPIOE, spi1_cs_gpio.Pin, GPIO_PIN_SET);


    /// configure DMA channels (todo)

    /// configure SPI1 interrupts
    /**
     * void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
void HAL_NVIC_EnableIRQ(IRQn_Type IRQn);
     */
    //HAL_NVIC_SetPriority(SPI1_IRQn, 1, 0);
    //SET_BIT(spi_handle.Instance->CR2, SPI_CR2_RXNEIE);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);

    /// initialise SPI
    if(HAL_SPI_Init(&spi_handle) == HAL_OK) {
        motion_sensor_data.initialised = true;
        motion_sensor_data.operation_mode = operation_mode_interrupts;
        motion_sensor_data.read_cb = read_callback;
    }
}

#define READ_OPERATION 0x80
#define WRITE_OPERATION 0x00

#define REG_WHO_AM_I  0x0F
#define REG_CTRL_REG4   0x20
#define REG_OUT_X_L   0x28
#define REG_OUT_X_H   0x29
#define REG_OUT_Y_L   0x2A
#define REG_OUT_Y_H   0x2B
#define REG_OUT_Z_L   0x2C
#define REG_OUT_Z_H   0x2D


#define DUMMY_VALUE 0x00
bool motion_sensor_driver_send_cmd(motion_sensor_driver_cmd_t cmd) {
    bool operation_success = false;
    if (motion_sensor_data.busy) {
        return operation_success;
    }
    if (cmd == motion_sensor_driver_cmd_read_XYZ) {
        /// create data packet
        motion_sensor_data.tx_buffer[0] = READ_OPERATION | REG_OUT_X_L;
        motion_sensor_data.tx_buffer[1] = DUMMY_VALUE;
        motion_sensor_data.tx_buffer[2] = DUMMY_VALUE;
        motion_sensor_data.tx_buffer[3] = DUMMY_VALUE;
        motion_sensor_data.tx_buffer[4] = DUMMY_VALUE;
        motion_sensor_data.tx_buffer[5] = DUMMY_VALUE;
        motion_sensor_data.tx_buffer[6] = DUMMY_VALUE;
        motion_sensor_data.tx_to_send = 7;
        motion_sensor_data.tx_iterator = 0;
        motion_sensor_data.rx_iterator = 0;
        motion_sensor_data.rx_to_receive = 7;
    } else if (cmd == motion_sensor_driver_cmd_enable) {
        motion_sensor_data.tx_buffer[0] = WRITE_OPERATION | REG_CTRL_REG4;
        motion_sensor_data.tx_buffer[1] = 0x87;
        motion_sensor_data.tx_to_send = 2;
        motion_sensor_data.tx_iterator = 0;
        motion_sensor_data.rx_iterator = 0;
        motion_sensor_data.rx_to_receive = 2;
    }


    /// initiate data exchange
    if(motion_sensor_data.operation_mode == operation_mode_interrupts) {

        __HAL_SPI_ENABLE_IT(&spi_handle, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));
        /// set CS
        HAL_GPIO_WritePin(GPIOE, spi1_cs_gpio.Pin, GPIO_PIN_RESET);
        /// enable SPI channel
        SET_BIT(spi_handle.Instance->CR1, SPI_CR1_SPE);
        //spi_handle.Instance->DR = motion_sensor_data.tx_buffer[motion_sensor_data.tx_iterator++];
        //HAL_SPI_TransmitReceive_IT(&spi_handle, motion_sensor_data.tx_buffer,
        //                           motion_sensor_data.rx_buffer, motion_sensor_data.tx_to_send);
        motion_sensor_data.busy = true;
        operation_success = true;
    }
    return operation_success;
}

void SPI1_IRQHandler(void) {
    volatile uint8_t status_reg;
    status_reg = spi_handle.Instance->SR;

    if (status_reg & 0x1) {
        /// rx buffer not empty; read data
        motion_sensor_data.rx_buffer[motion_sensor_data.rx_iterator++] = spi_handle.Instance->DR;
    }

    if (status_reg & 0x2) {
        if (motion_sensor_data.tx_iterator < motion_sensor_data.tx_to_send) {
            spi_handle.Instance->DR = motion_sensor_data.tx_buffer[motion_sensor_data.tx_iterator++];
        } else if ((motion_sensor_data.tx_iterator == motion_sensor_data.tx_to_send)
        && !(status_reg & 0x40)) {
            /// all data exchanged and SPI not busy
            CLEAR_BIT(spi_handle.Instance->CR1, SPI_CR1_SPE);
            HAL_GPIO_WritePin(GPIOE, spi1_cs_gpio.Pin, GPIO_PIN_SET);
            /// callback with data
            motion_sensor_data.read_cb(&motion_sensor_data.rx_buffer[1], motion_sensor_data.rx_to_receive-1);
            motion_sensor_data.busy = false;
        }
    }
}