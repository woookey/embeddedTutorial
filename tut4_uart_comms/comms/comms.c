#include "comms.h"

#include "stm32f407xx.h"
#include "stm32f4xx_ll_usart.h"

#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_uart.h"

#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_cortex.h"

#include <string.h>

typedef struct {
    /// transmission data
    uint8_t tx_bytes_left_to_send;
    uint8_t* tx_curr_data;
    /// reception data
    uint8_t rx_payload_size;
    uint8_t rx_buffer_items;
    uint8_t* rx_buffer;
    /// error data
    comms_error_t error;
} comms_t;

static void clean_rx_data(comms_t* comms_object);

#define RX_MAX_BUFFER_SIZE (uint8_t)20
static uint8_t rx_data[RX_MAX_BUFFER_SIZE];
static comms_t comms_data = {
    /// set up transmission data
    .tx_bytes_left_to_send = 0,
    .tx_curr_data = NULL,
    /// set up reception data
    .rx_payload_size = 0,
    .rx_buffer_items = 0,
    .rx_buffer = rx_data,
    /// error feedback
    .error = COMMS_ERROR_NONE,
};

static const UART_InitTypeDef uart1_config = {
    .BaudRate = (uint32_t)115200,
    .WordLength = UART_WORDLENGTH_8B,
    .StopBits = UART_STOPBITS_1,
    .Parity = UART_PARITY_NONE,
    .Mode = UART_MODE_TX_RX,
    .OverSampling = UART_OVERSAMPLING_16
};

static UART_HandleTypeDef uart1_instance = {
    .Instance = USART1,
    .Init = uart1_config,
};

bool comms_initialise(comms_config_t config) {
    bool init_result;
    /// prepare comms data
    memset(rx_data, 0, sizeof(uint8_t)*RX_MAX_BUFFER_SIZE);
    comms_data.rx_payload_size = config.payload_size;

    /// initialise UART
    init_result = (bool)HAL_UART_Init(&uart1_instance);
    SET_BIT(uart1_instance.Instance->CR1, USART_CR1_RXNEIE);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    return (init_result == HAL_OK);
}

void comms_send_data(uint8_t* data, uint8_t data_size) {
    /// only send data if transmission buffer is empty, otherwise report an error
    if (comms_data.tx_bytes_left_to_send) {
        comms_error_cb(COMMS_ERROR_TX_BUSY);
    } else {
        comms_data.tx_curr_data = data;
        comms_data.tx_bytes_left_to_send = data_size;
        CLEAR_BIT(uart1_instance.Instance->SR, USART_SR_TC);
        SET_BIT(uart1_instance.Instance->CR1, USART_CR1_TXEIE);
    }
}

void USART1_IRQHandler(void) {
    volatile uint32_t status_reg   = READ_REG(uart1_instance.Instance->SR);
    volatile uint32_t cr1its     = READ_REG(uart1_instance.Instance->CR1);

    /// handle interrupt-driven UART transmission
    if ((status_reg & USART_SR_TXE) && (cr1its & USART_CR1_TXEIE)) {
        if (comms_data.tx_bytes_left_to_send) {
            /// load next byte to data register
            uart1_instance.Instance->DR = (*comms_data.tx_curr_data++ & (uint8_t)0xFF);
            if (--comms_data.tx_bytes_left_to_send == 0) {
                /// disable TXE and wait for TC
                CLEAR_BIT(uart1_instance.Instance->CR1, USART_CR1_TXEIE);
                SET_BIT(uart1_instance.Instance->CR1, USART_CR1_TCIE);
            }
        }
    } else if ((status_reg & USART_SR_TC) && (cr1its & USART_CR1_TCIE)) {
        comms_data.tx_bytes_left_to_send = 0;
        comms_data.tx_curr_data = NULL;
        CLEAR_BIT(uart1_instance.Instance->CR1, USART_CR1_TCIE);
    }

    /// handle interrupt-driven UART reception, check for reception errors
    /// check if any error (noise, frame, overrun, parity) is active
    if (status_reg & UART_FLAG_RXNE) {
        if ((status_reg & UART_FLAG_FE) || (status_reg & UART_FLAG_NE)
            || (status_reg & UART_FLAG_ORE) || (status_reg & UART_FLAG_PE)) {
            /// find an error
            if (status_reg & UART_FLAG_FE) {
                comms_data.error = COMMS_ERROR_FRAMING;
            } else if (status_reg & UART_FLAG_NE) {
                comms_data.error = COMMS_ERROR_NOISE_DETECTED;
            } else if (status_reg & UART_FLAG_ORE) {
                comms_data.error = COMMS_ERROR_OVERRUN;
            } else if (status_reg & UART_FLAG_PE) {
                comms_data.error = COMMS_ERROR_PARITY;
            }
            /// disable reception
            CLEAR_BIT(uart1_instance.Instance->CR1, USART_CR1_RXNEIE);
            /// report a specific error in a callback
            comms_error_cb(comms_data.error);
            /// clean recent reception related data
        } else {
            /// insert new data into the buffer unless it is full
            if (comms_data.rx_buffer_items < comms_data.rx_payload_size) {
                *(comms_data.rx_buffer + comms_data.rx_buffer_items) =
                        READ_REG(uart1_instance.Instance->DR);
                /// handle incoming payload in a callback when it is full
                if (++comms_data.rx_buffer_items == comms_data.rx_payload_size) {
                    comms_handle_data_cb(comms_data.rx_buffer, comms_data.rx_payload_size);
                    clean_rx_data(&comms_data);
                }
            }
        }
    }
}

inline void clean_rx_data(comms_t* comms_object) {
    memset(comms_object->rx_buffer, 0, sizeof(uint8_t)*comms_object->rx_payload_size);
    comms_object->rx_buffer_items = 0;
}