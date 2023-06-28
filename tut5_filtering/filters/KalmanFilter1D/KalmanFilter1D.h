#ifndef TUT5_FILTERS_KALMAN_FILTER_1D_H
#define TUT5_FILTERS_KALMAN_FILTER_1D_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float estimate;
    float estimate_error;
    float measurement_error;
    float kalman_gain;
    float process_noise;
} KalmanFilter1D_t;

void KalmanFilter1D_init(KalmanFilter1D_t* filter, float est_init, float est_error_init, float meas_error_init,
                         float process_noise);

float KalmanFilter1D_update(KalmanFilter1D_t* filter, float new_sample);

#endif