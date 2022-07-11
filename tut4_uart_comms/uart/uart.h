#ifndef TUT4_UART_H
#define TUT4_UART_H

#include <stdint.h>
#include <stdbool.h>

bool uart_initialise(void);
void uart_send_data(uint8_t* data, uint8_t data_size);

#endif
