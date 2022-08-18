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

#include "comms.h"
#include "comms_telemetry.h"

#include "comms_driver/comms_driver.h"
#include <string.h>

/// data exchange
#define INCOMING_PAYLOAD_SIZE (uint8_t)2
#define COMMS_BAUDRATE (uint32_t)115200
#define PACKET_OUTGOING_MAX_SIZE 20U

static uint8_t packet[PACKET_OUTGOING_MAX_SIZE];
static comms_cmd_t last_cmd = {
    .id = COMMS_CMD_INVALID
};

void comms_init(void) {
    /// configure comms driver
    comms_driver_config_t comms_driver_config = {
        .payload_size = INCOMING_PAYLOAD_SIZE,
        .baudrate = COMMS_BAUDRATE,
        .parity = COMMS_DRIVER_PARITY_NONE,
        .mode = COMMS_DRIVER_MODE_DMA_IT,
    };
    comms_driver_initialise(comms_driver_config);
}

void comms_handle() {
    /// send current packet
    comms_driver_send_data((uint8_t*)&packet, sizeof(telemetry_t));

    /// process recent data
    comms_process_cmd(&last_cmd);
}

void comms_driver_handle_data_cb(uint8_t* payload, uint8_t payload_size) {
    last_cmd = comms_get_cmd(payload, payload_size);
}

void comms_driver_error_cb(comms_driver_error_t error) {
    (void)error;
}

void comms_update_telemetry(telemetry_t new_telemetry) {
    memcpy((void*)packet, (void*)&new_telemetry, sizeof(telemetry_t));
}
