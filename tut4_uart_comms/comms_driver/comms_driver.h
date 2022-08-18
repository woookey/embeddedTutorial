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

#ifndef TUT4_COMMS_DRIVER_H
#define TUT4_COMMS_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

/// \brief comms driver operation mode
///     \arg COMMS_DRIVER_MODE_UART_IT UART comms driven by interrupts
///     \arg COMMS_DRIVER_MODE_DMA_IT UART comms with DMA driven by DMA Rx/Tx interrupts
typedef enum {
    COMMS_DRIVER_MODE_UART_IT = 0U,
    COMMS_DRIVER_MODE_DMA_IT,
} comms_driver_mode_t;

/// \brief comms driver error
typedef enum {
    COMMS_DRIVER_ERROR_NONE = 0U,
    COMMS_DRIVER_ERROR_PARITY,
    COMMS_DRIVER_ERROR_FRAMING,
    COMMS_DRIVER_ERROR_NOISE_DETECTED,
    COMMS_DRIVER_ERROR_OVERRUN,
    COMMS_DRIVER_ERROR_TX_BUSY
} comms_driver_error_t;

/// \brief comms driver parity configuration
typedef enum {
    COMMS_DRIVER_PARITY_NONE = 0U,
    COMMS_DRIVER_PARITY_EVEN,
    COMMS_DRIVER_PARITY_ODD,
} comms_driver_parity_t;

/// \brief structure to configure comms driver
/// \param payload_size expected size of incoming payload in bytes
/// \param baudrate required communication speed in bps
/// \param parity parity setup
/// \param mode uart tx & rx operation mode
typedef struct {
    uint8_t payload_size;
    uint32_t baudrate;
    comms_driver_parity_t parity;
    comms_driver_mode_t mode;
} comms_driver_config_t;

/// \brief initialises comms driver along all logical and hardware facilities
/// \param config driver configuration
/// \return bool true when initialisation succeeded
bool comms_driver_initialise(comms_driver_config_t config);

/// \brief triggers sending data
/// \param data array of uint8_t packet
/// \param data_size size of packet in bytes
void comms_driver_send_data(uint8_t* data, uint8_t data_size);

/// callbacks to be implemented in the application
extern void comms_driver_error_cb(comms_driver_error_t error);
extern void comms_driver_handle_data_cb(uint8_t* payload, uint8_t payload_size);

#endif
