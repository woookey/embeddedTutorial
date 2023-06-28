#include "KalmanFilter1D.h"

void KalmanFilter1D_init(KalmanFilter1D_t* filter, float est_init, float est_error_init, float meas_error_init,
                         float process_noise) {
    filter->kalman_gain = 0.0f;
    filter->estimate = est_init;
    filter->estimate_error = est_error_init;
    filter->measurement_error = meas_error_init;
    filter->process_noise = process_noise;
}

float KalmanFilter1D_update(KalmanFilter1D_t* filter, float new_sample) {
    filter->kalman_gain = (filter->estimate_error+filter->process_noise)/(filter->estimate_error+filter->process_noise+filter->measurement_error);
    filter->estimate = filter->estimate + filter->kalman_gain*(new_sample - filter->estimate);
    filter->estimate_error = (1-filter->kalman_gain)*filter->estimate_error;
    return filter->estimate;
}