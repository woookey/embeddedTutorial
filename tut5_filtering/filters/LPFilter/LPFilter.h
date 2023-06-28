#ifndef TUT5_FILTERS_LP_FILTER_H
#define TUT5_FILTERS_LP_FILTER_H

#include <stdint.h>

typedef struct {
    float estimate;
    float prev_estimate;
    float alpha;
} LPFilter_t;

void LPFilter_init(LPFilter_t* filter, float alpha);

float LPFilter_update(LPFilter_t* filter, float new_sample);

#endif