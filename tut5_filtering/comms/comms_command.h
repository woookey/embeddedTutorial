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
