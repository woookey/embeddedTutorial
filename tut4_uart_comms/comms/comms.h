#ifndef TUT4_COMMS_H
#define TUT4_COMMS_H

#include <stdint.h>
#include <stdbool.h>

bool comms_initialise(void);
void comms_send_data(uint8_t* data, uint8_t data_size);
void comms_handle_data(void);

#endif
