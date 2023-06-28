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

#ifndef TUT5_MOTION_SENSOR_H
#define TUT5_MOTION_SENSOR_H

typedef enum {
    motion_sensor_data_acceleration_X,
    motion_sensor_data_acceleration_Y,
    motion_sensor_data_acceleration_Z,
    motion_sensor_data_acceleration_X_est,
    motion_sensor_data_acceleration_Y_est,
    motion_sensor_data_acceleration_Z_est,
    motion_sensor_data_acceleration_Z_MA_5,
    motion_sensor_data_acceleration_Z_MA_10,
    motion_sensor_data_acceleration_Z_MA_20,
    motion_sensor_data_acceleration_Z_MA_40,
    motion_sensor_data_acceleration_Z_RMA_5,
    motion_sensor_data_acceleration_Z_LP_0_9,
    motion_sensor_data_acceleration_Z_LP_0_7,
    motion_sensor_data_acceleration_Z_LP_0_5,
    motion_sensor_data_acceleration_Z_LP_0_3,
    motion_sensor_data_acceleration_Z_Kalman,
    motion_sensor_data_acceleration_X_RC,
    motion_sensor_data_acceleration_Y_RC,
    motion_sensor_data_acceleration_Z_RC,
} motion_sensor_data_t;

void motion_sensor_init(void);

void motion_sensor_handle(void);

float motion_sensor_get_data(motion_sensor_data_t data_request);

#endif