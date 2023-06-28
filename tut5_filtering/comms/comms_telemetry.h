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

#ifndef TUT5_COMMS_TELEMETRY_H
#define TUT5_COMMS_TELEMETRY_H

#include <stdint.h>

#define TELEMETRY_UID (uint16_t)0x2A

typedef struct __attribute__((packed)) {
    uint8_t uid;
    uint32_t cookie;
    uint8_t switch_on;
    float acc_X;
    float acc_Y;
    float acc_Z;
    float acc_X_filtered;
    float acc_Y_filtered;
    float acc_Z_filtered;
    float roll;
    float pitch;
    float roll_filtered;
    float pitch_filtered;
} telemetry_t;

void comms_update_telemetry(telemetry_t new_telemetry);

#endif
