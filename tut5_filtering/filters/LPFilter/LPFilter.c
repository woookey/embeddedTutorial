#include "LPFilter.h"
#include <string.h>

void LPFilter_init(LPFilter_t* filter, float alpha) {
    memset(filter, 0, sizeof(LPFilter_t));
    filter->alpha = alpha;
}

float LPFilter_update(LPFilter_t* filter, float new_sample) {
    filter->estimate = filter->alpha*filter->prev_estimate + (1-filter->alpha)*new_sample;
    filter->prev_estimate = filter->estimate;
    return filter->estimate;
}