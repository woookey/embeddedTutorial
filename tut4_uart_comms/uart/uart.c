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
        .Parity = UART_PARITY_NONE,//UART_PARITY_EVEN
        .Mode = UART_MODE_TX_RX,
        .OverSampling = UART_OVERSAMPLING_16
};

static UART_HandleTypeDef uart2_instance = {
        .Instance = USART2,
        .Init = uart2_config,
};
static uint8_t bytes_left_to_send = 0;
//static uint8_t tx_data_size = 0;
static uint8_t* curr_data = NULL;

bool uart_initialise(void) {
    bool init_result;
    init_result = (bool)HAL_UART_Init(&uart2_instance);
    //__HAL_UART_ENABLE_IT(&uart2_instance, UART_IT_TXE);
    //__HAL_UART_ENABLE_IT(&uart2_instance, UART_IT_TC);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    return (init_result == HAL_OK);
}

void uart_send_data(uint8_t* data, uint8_t data_size) {
    curr_data = data;
    bytes_left_to_send = data_size;
    CLEAR_BIT(uart2_instance.Instance->SR, USART_SR_TC);
    SET_BIT(uart2_instance.Instance->CR1, USART_CR1_TXEIE);
}

void USART2_IRQHandler(void) {
    volatile uint32_t isrflags   = READ_REG(uart2_instance.Instance->SR);
    volatile uint32_t cr1its     = READ_REG(uart2_instance.Instance->CR1);

    /// handle interrupt-driven UART transmission
    if ((isrflags & USART_SR_TXE) && (cr1its & USART_CR1_TXEIE)) {
        if (bytes_left_to_send) {
            /// load next byte to data register
            uart2_instance.Instance->DR = (*curr_data++ & (uint8_t)0xFF);
            if (--bytes_left_to_send == 0) {
                /// disable TXE and wait for TC
                CLEAR_BIT(uart2_instance.Instance->CR1, USART_CR1_TXEIE);
                SET_BIT(uart2_instance.Instance->CR1, USART_CR1_TCIE);
            }
        }
    } else if((isrflags & USART_SR_TC) && (cr1its & USART_CR1_TCIE)) {
        bytes_left_to_send = 0;
        curr_data = NULL;
        CLEAR_BIT(uart2_instance.Instance->CR1, USART_CR1_TCIE);
    }
}

//* @param  __INTERRUPT__: specifies the UART interrupt source to enable.
//*          This parameter can be one of the following values:
//*            @arg UART_IT_CTS:  CTS change interrupt
//*            @arg UART_IT_LBD:  LIN Break detection interrupt
//*            @arg UART_IT_TXE:  Transmit Data Register empty interrupt
//*            @arg UART_IT_TC:   Transmission complete interrupt
//*            @arg UART_IT_RXNE: Receive Data register not empty interrupt
//*            @arg UART_IT_IDLE: Idle line detection interrupt
//*            @arg UART_IT_PE:   Parity Error interrupt
//*            @arg UART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
//* @retval None
//*/
//#define UART_IT_MASK  0x0000FFFFU
//#define __HAL_UART_ENABLE_IT(__HANDLE__, __INTERRUPT__)
