#include "MAFilter.h"
#include <string.h>

void MAFilter_init(MAFilter_t* filter, float* samples_array, uint8_t filter_size, float filter_size_inverse) {
    filter->samples = samples_array;
    filter->output = 0.0f;
    filter->N = filter_size;
    filter->N_inverse = filter_size_inverse;
    filter->index = 0;
    /// clean samples array
    memset(filter->samples, 0, sizeof(float)*filter->N);
}

float MAFilter_update(MAFilter_t* filter, float new_sample) {
    /// add new sample
    filter->samples[filter->index] = new_sample;
    /// increment and wrap index
    if (++filter->index == filter->N) {
        filter->index = 0;
    }
    /// calculate moving average
    float sum = 0.0f;
    uint8_t sum_index = filter->index;
    for (uint8_t i = 0; i < filter->N; i++) {
        /// keep decrementing summing index and wrap around the filter size
        if (sum_index > 0) {
            sum_index--;
        } else {
            sum_index = filter->N-1;
        }
        sum += filter->N_inverse*filter->samples[sum_index];
    }
    filter->output = sum;
    return filter->output;
}