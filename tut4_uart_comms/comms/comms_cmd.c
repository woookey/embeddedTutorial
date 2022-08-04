#include "comms_cmd.h"

/// CMD_TURN_BLUE_LED_ON  = "XA"
/// CMD_TURN_BLUE_LED_OFF = "FY"
#define CMD_TURN_BLUE_LED_ON  (uint16_t)0x5841
#define CMD_TURN_BLUE_LED_OFF (uint16_t)0x4659

/// 16-bit command construction:
/// CMD_BYTE_0 (MSB) | CMD_BYTE_1 (LSB)
#define CMD_BYTE_0 0U
#define CMD_BYTE_1 1U

comms_cmd_t comms_get_cmd(uint8_t* buffer, uint8_t size) {
    /// should be an assert here
    comms_cmd_t new_cmd;
    static uint16_t cmd_raw;
    cmd_raw = ((uint16_t)buffer[0] << 8 | buffer[1]);
    switch (cmd_raw) {
        case CMD_TURN_BLUE_LED_ON: {
            new_cmd.id = COMMS_CMD_TURN_LED_ON;
            break;
        }
        case CMD_TURN_BLUE_LED_OFF: {
            new_cmd.id = COMMS_CMD_TURN_LED_OFF;
            break;
        }
        default: {
            new_cmd.id = COMMS_CMD_INVALID;
        }
    }
    return new_cmd;
}