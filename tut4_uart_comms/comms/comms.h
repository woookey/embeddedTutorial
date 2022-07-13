#ifndef TUT4_COMMS_H
#define TUT4_COMMS_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t payload_size;
} comms_config_t;

typedef enum {
    COMMS_ERROR_NONE = 0U,
    COMMS_ERROR_PARITY,
    COMMS_ERROR_FRAMING,
    COMMS_ERROR_NOISE_DETECTED,
    COMMS_ERROR_OVERRUN,
    COMMS_ERROR_TX_BUSY
} comms_error_t;

bool comms_initialise(comms_config_t config);
void comms_send_data(uint8_t* data, uint8_t data_size);

//void comms_handle_data(void);
//void comms_receive_data(void);
extern void comms_error_cb(comms_error_t error);
extern void comms_handle_data_cb(uint8_t* payload, uint8_t payload_size);

#endif
