#ifndef TUT5_FILTERS_RMA_FILTER_H
#define TUT5_FILTERS_RMA_FILTER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float output;
    float prev_output;
    uint8_t N;
    float N_inverse;
    float* samples;
    uint8_t index;
    bool first_samples_in;
} RMAFilter_t;

void RMAFilter_init(RMAFilter_t* filter, float* samples_array, uint8_t filter_size, float filter_size_inverse);

float RMAFilter_update(RMAFilter_t* filter, float new_sample);

#endif