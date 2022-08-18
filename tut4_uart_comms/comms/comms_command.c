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

#include "comms_command.h"

/// CMD_TURN_BLUE_LED_ON  = "XA"
/// CMD_TURN_BLUE_LED_OFF = "FY"
#define CMD_TURN_BLUE_LED_ON  (uint16_t)0x5841
#define CMD_TURN_BLUE_LED_OFF (uint16_t)0x4659

/// 16-bit command construction:
/// CMD_BYTE_0 (MSB) | CMD_BYTE_1 (LSB)
#define CMD_LENGTH 2U
#define CMD_BYTE_0 0U
#define CMD_BYTE_1 1U

comms_cmd_t comms_get_cmd(uint8_t* buffer, uint8_t size) {
    /// \todo add an assert here for size
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