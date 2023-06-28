#include "RMAFilter.h"
#include <string.h>

void RMAFilter_init(RMAFilter_t* filter, float* samples_array, uint8_t filter_size, float filter_size_inverse) {
    filter->samples = samples_array;
    filter->output = 0.0f;
    filter->prev_output = 0.0f;
    filter->N = filter_size;
    filter->N_inverse = filter_size_inverse;
    filter->index = 0;
    filter->first_samples_in = false;
    /// clean samples array
    memset(filter->samples, 0, sizeof(float)*(filter->N+1));
}

float RMAFilter_update(RMAFilter_t* filter, float new_sample) {
    /// handle the first N samples
    if (!filter->first_samples_in) {
        /// add new sample
        filter->samples[filter->index] = new_sample;
        if (++filter->index == filter->N) {
            /// calculate a simple mean average for the first Nth sample, and
            /// continue with recursive calculations for next samples
            for (uint8_t i = 0; i < filter->N; i++) {
                filter->output += filter->samples[i]*filter->N_inverse;
            }
            filter->first_samples_in = true;
        }
    } else {
        /// add new sample
        filter->samples[filter->index] = new_sample;

        /// calculate recursive moving average
        filter->output = filter->prev_output + (new_sample-filter->samples[0])*filter->N_inverse;
        /// shift samples array to the left, and discard the oldest sample within N+1 window
        memcpy(&filter->samples[0], &filter->samples[1], filter->N*sizeof(float));
    }

    filter->prev_output = filter->output;
    return filter->output;
}