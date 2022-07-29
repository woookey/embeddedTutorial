#ifndef TUT4_COMMS_DRIVER_H
#define TUT4_COMMS_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

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

bool comms_driver_initialise(comms_driver_config_t config);
void comms_driver_send_data(uint8_t* data, uint8_t data_size);

extern void comms_driver_error_cb(comms_driver_error_t error);
extern void comms_driver_handle_data_cb(uint8_t* payload, uint8_t payload_size);

#endif
