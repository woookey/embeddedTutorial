#ifndef TUT5_FILTERS_MA_FILTER_H
#define TUT5_FILTERS_MA_FILTER_H

#include <stdint.h>

typedef struct {
    float output;
    uint8_t N;
    float N_inverse;
    float* samples;
    uint8_t index;
} MAFilter_t;

void MAFilter_init(MAFilter_t* filter, float* samples_array, uint8_t filter_size, float filter_size_inverse);

float MAFilter_update(MAFilter_t* filter, float new_sample);

#endif