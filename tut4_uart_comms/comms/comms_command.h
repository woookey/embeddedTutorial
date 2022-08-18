#ifndef TUT4_COMMS_COMMAND_H
#define TUT4_COMMS_COMMAND_H

#include <stdint.h>

typedef enum {
    COMMS_CMD_TURN_LED_ON = 0U,
    COMMS_CMD_TURN_LED_OFF,
    COMMS_CMD_INVALID,
} comms_cmd_id_t;

typedef struct {
    comms_cmd_id_t id;
} comms_cmd_t;

comms_cmd_t comms_get_cmd(uint8_t* buffer, uint8_t size);

#endif
