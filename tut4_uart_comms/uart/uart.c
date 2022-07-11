#include "uart.h"

#include "stm32f407xx.h"
#include "stm32f4xx_ll_usart.h"

#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_uart.h"

#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_cortex.h"

static const UART_InitTypeDef uart2_config = {
        .BaudRate = (uint32_t)115200,
        .WordLength = UART_WORDLENGTH_8B,
        .StopBits = UART_STOPBITS_1,
        .Parity = UART_PARITY_EVEN,
        .Mode = UART_MODE_TX_RX,
        .OverSampling = UART_OVERSAMPLING_16
};

static UART_HandleTypeDef uart2_instance = {
        .Instance = USART2,
        .Init = uart2_config,
};

bool uart_initialise(void) {
    bool init_result;
    init_result = (bool)HAL_UART_Init(&uart2_instance);
    return (init_result == HAL_OK);
}

void uart_send_data(uint8_t* data, uint8_t data_size) {
    HAL_UART_Transmit(&uart2_instance, data, data_size, 1);
}
