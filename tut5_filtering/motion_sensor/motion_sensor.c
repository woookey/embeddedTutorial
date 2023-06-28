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

#include "motion_sensor.h"
#include "motion_sensor_driver/motion_sensor_driver.h"

#include "filters/RCFilter/RCFilter.h"
#include "filters/MAFilter/MAFilter.h"
#include "filters/RMAFilter/RMAFilter.h"
#include "filters/LPFilter/LPFilter.h"
#include "filters/KalmanFilter1D/KalmanFilter1D.h"

#include <stdint.h>
#include <string.h>

static void process_motion_data(uint8_t* buffer, uint16_t buffer_length);

#define SENSITIVITY_2G_RANGE (float)0.06f

/// RC filter
#define F_CUT_OFF   (float)1.0f
#define SAMPLING_T  (float)0.01f
#define N (uint8_t)10U
#define N_5 (uint8_t)5U
#define N_10 (uint8_t)10U
#define N_20 (uint8_t)20U
#define N_40 (uint8_t)40U

#define LP_alpha_0_9 (float)0.9f
#define LP_alpha_0_7 (float)0.7f
#define LP_alpha_0_5 (float)0.5f
#define LP_alpha_0_3 (float)0.3f

/// Kalman filter definitions
#define X_INIT              (float)500.0f
#define EST_ERROR_INIT      (float)10.0f
#define MEAS_ERROR_INIT     (float)100.0f
#define PROCESS_NOISE       (float)10.0f

typedef enum {
    motion_state_uninitialised,
    motion_state_powered_up,
    motion_state_configuring,
    motion_state_operational,
} motion_state_machine_t;

static float N_5_average = 0.2f;
static float N_10_average = 0.1f;
static float N_20_average = 0.05f;
static float N_40_average = 0.025f;

typedef struct {
    float acc_X;
    float acc_Y;
    float acc_Z;

    float acc_Z_est;
    float acc_Z_samples[N];

    /// RC filters
    RCFilter_t RC_filter_X;
    RCFilter_t RC_filter_Y;
    RCFilter_t RC_filter_Z;

    /// moving average for Z
    float MA_samples_copy_5[N_5];
    float MA_samples_copy_10[N_10];
    float MA_samples_copy_20[N_20];
    float MA_samples_copy_40[N_40];
    MAFilter_t moving_average_filter_Z_5;
    MAFilter_t moving_average_filter_Z_10;
    MAFilter_t moving_average_filter_Z_20;
    MAFilter_t moving_average_filter_Z_40;

    /// recursive moving average for Z
    float RMA_sample_copy_5[N_5+1];
    RMAFilter_t RMA_filter_Z_5;

    /// 1st order low pass filter for Z
    LPFilter_t LP_filter_Z_alpha_0_9;
    LPFilter_t LP_filter_Z_alpha_0_7;
    LPFilter_t LP_filter_Z_alpha_0_5;
    LPFilter_t LP_filter_Z_alpha_0_3;

    /// Simple Kalman filter for Z
    KalmanFilter1D_t kalman_filter_Z;

    uint8_t sample_k;
    motion_sensor_driver_cmd_t cmd_being_handled;
    motion_state_machine_t state;
    uint16_t counter;
} motion_state_t;
static motion_state_t motion_data;

void motion_sensor_init(void) {
    /// initialise motion sensor channel
    motion_sensor_driver_init(process_motion_data);

    /// initialise filters
    RCFilter_init(&motion_data.RC_filter_X, SAMPLING_T, F_CUT_OFF);
    RCFilter_init(&motion_data.RC_filter_Y, SAMPLING_T, F_CUT_OFF);
    RCFilter_init(&motion_data.RC_filter_Z, SAMPLING_T, F_CUT_OFF);
    MAFilter_init(&motion_data.moving_average_filter_Z_5, motion_data.MA_samples_copy_5, N_5, N_5_average);
    MAFilter_init(&motion_data.moving_average_filter_Z_10, motion_data.MA_samples_copy_10, N_10, N_10_average);
    MAFilter_init(&motion_data.moving_average_filter_Z_20, motion_data.MA_samples_copy_20, N_20, N_20_average);
    MAFilter_init(&motion_data.moving_average_filter_Z_40, motion_data.MA_samples_copy_40, N_40, N_40_average);
    RMAFilter_init(&motion_data.RMA_filter_Z_5, motion_data.RMA_sample_copy_5, N_5, N_5_average);
    LPFilter_init(&motion_data.LP_filter_Z_alpha_0_9, LP_alpha_0_9);
    LPFilter_init(&motion_data.LP_filter_Z_alpha_0_7, LP_alpha_0_7);
    LPFilter_init(&motion_data.LP_filter_Z_alpha_0_5, LP_alpha_0_5);
    LPFilter_init(&motion_data.LP_filter_Z_alpha_0_3, LP_alpha_0_3);
    KalmanFilter1D_init(&motion_data.kalman_filter_Z, X_INIT, EST_ERROR_INIT, MEAS_ERROR_INIT, PROCESS_NOISE);

    /// initialise internal data
    motion_data.state = motion_state_uninitialised;
}

void motion_sensor_handle(void) {
    switch (motion_data.state) {
        case motion_state_uninitialised: {
            if (++motion_data.counter == 5) {
                motion_data.state = motion_state_powered_up;
            }
            break;
        }
        case motion_state_powered_up: {
            /// start configuring sensor
            motion_data.cmd_being_handled = motion_sensor_driver_cmd_enable;
            motion_sensor_driver_send_cmd(motion_sensor_driver_cmd_enable);
            motion_data.state = motion_state_operational;
            break;
        }
        case motion_state_operational: {
            /// keep sending command to fetch X,Y,Z data
            motion_data.cmd_being_handled = motion_sensor_driver_cmd_read_XYZ;
            motion_sensor_driver_send_cmd(motion_sensor_driver_cmd_read_XYZ);
            break;
        }
        default: {
            break;
        }
    }
}

void process_motion_data(uint8_t* buffer, uint16_t buffer_length) {
    int16_t X_int16, Y_int16, Z_int16;
    if (motion_data.cmd_being_handled == motion_sensor_driver_cmd_read_XYZ &&
            (buffer_length == 6)) {
        X_int16 = ((int16_t)buffer[1] << 8U) | (int16_t)buffer[0];
        Y_int16 = ((int16_t)buffer[3] << 8U) | (int16_t)buffer[2];
        Z_int16 = ((int16_t)buffer[5] << 8U) | (int16_t)buffer[4];
        motion_data.acc_X = (float)X_int16*SENSITIVITY_2G_RANGE;
        motion_data.acc_Y = (float)Y_int16*SENSITIVITY_2G_RANGE;
        motion_data.acc_Z = (float)Z_int16*SENSITIVITY_2G_RANGE;

        /// RC Filtering
        RCFilter_update(&motion_data.RC_filter_X, motion_data.acc_X);
        RCFilter_update(&motion_data.RC_filter_Y, motion_data.acc_Y);
        RCFilter_update(&motion_data.RC_filter_Z, motion_data.acc_Z);

        /*MAFilter_update(&motion_data.moving_average_filter_Z_5, motion_data.acc_Z);
        MAFilter_update(&motion_data.moving_average_filter_Z_10, motion_data.acc_Z);
        MAFilter_update(&motion_data.moving_average_filter_Z_20, motion_data.acc_Z);
        MAFilter_update(&motion_data.moving_average_filter_Z_40, motion_data.acc_Z);*/
        /// filtering Z: recursive moving average
/*        RMAFilter_update(&motion_data.RMA_filter_Z_5, motion_data.acc_Z);
        /// filtering Z: 1st order low pass
        LPFilter_update(&motion_data.LP_filter_Z_alpha_0_9, motion_data.acc_Z);
        LPFilter_update(&motion_data.LP_filter_Z_alpha_0_7, motion_data.acc_Z);
        LPFilter_update(&motion_data.LP_filter_Z_alpha_0_5, motion_data.acc_Z);
        LPFilter_update(&motion_data.LP_filter_Z_alpha_0_3, motion_data.acc_Z);
        /// filtering Z: kalman filter
        KalmanFilter1D_update(&motion_data.kalman_filter_Z, motion_data.acc_Z);*/
    }
}

float motion_sensor_get_data(motion_sensor_data_t data_request) {
    switch (data_request) {
        case motion_sensor_data_acceleration_X: {
            return motion_data.acc_X;
        }
        case motion_sensor_data_acceleration_Y: {
            return motion_data.acc_Y;
        }
        case motion_sensor_data_acceleration_Z: {
            return motion_data.acc_Z;
        }
        case motion_sensor_data_acceleration_X_est: {
            return 0.0f;
        }
        case motion_sensor_data_acceleration_Y_est: {
            return 0.0f;
        }
        case motion_sensor_data_acceleration_Z_est: {
            return motion_data.acc_Z_est;
        }
        case motion_sensor_data_acceleration_Z_MA_5: {
            return motion_data.moving_average_filter_Z_5.output;
        }
        case motion_sensor_data_acceleration_Z_MA_10: {
            return motion_data.moving_average_filter_Z_10.output;
        }
        case motion_sensor_data_acceleration_Z_MA_20: {
            return motion_data.moving_average_filter_Z_20.output;
        }
        case motion_sensor_data_acceleration_Z_MA_40: {
            return motion_data.moving_average_filter_Z_40.output;
        }
        case motion_sensor_data_acceleration_Z_RMA_5: {
            return motion_data.RMA_filter_Z_5.output;
        }
        case motion_sensor_data_acceleration_Z_LP_0_9: {
            return motion_data.LP_filter_Z_alpha_0_9.estimate;
        }
        case motion_sensor_data_acceleration_Z_LP_0_7: {
            return motion_data.LP_filter_Z_alpha_0_7.estimate;
        }
        case motion_sensor_data_acceleration_Z_LP_0_5: {
            return motion_data.LP_filter_Z_alpha_0_5.estimate;
        }
        case motion_sensor_data_acceleration_Z_LP_0_3: {
            return motion_data.LP_filter_Z_alpha_0_3.estimate;
        }
        case motion_sensor_data_acceleration_Z_Kalman: {
            return motion_data.kalman_filter_Z.estimate;
        }
        case motion_sensor_data_acceleration_X_RC: {
            return motion_data.RC_filter_X.output;
        }
        case motion_sensor_data_acceleration_Y_RC: {
            return motion_data.RC_filter_Y.output;
        }
        case motion_sensor_data_acceleration_Z_RC: {
            return motion_data.RC_filter_Z.output;
        }
        default: {
            return 0.0f;
        }
    }
}