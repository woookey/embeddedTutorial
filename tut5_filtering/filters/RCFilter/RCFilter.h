#ifndef TUT5_FILTERS_RC_FILTER_H
#define TUT5_FILTERS_RC_FILTER_H

#include <stdint.h>

typedef struct {
    float output;
    float prev_output;
    float coeff[2];
} RCFilter_t;

void RCFilter_init(RCFilter_t* filter, float T, float f_cutoff);

float RCFilter_update(RCFilter_t* filter, float new_sample);

#endif