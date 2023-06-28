#include "RCFilter.h"
#include <string.h>
#include <math.h>

/*typedef struct {
    float prev_estimate;
    float output;
    float prev_output;
    float coeff[2];
} RCFilter_t;*/

void RCFilter_init(RCFilter_t* filter, float T, float f_cutoff) {
    float RC = 1/(2*M_PI*f_cutoff);
    filter->coeff[0] = T/(T+RC);
    filter->coeff[1] = RC/(T+RC);
}

float RCFilter_update(RCFilter_t* filter, float new_sample) {
    filter->output = filter->coeff[0]*new_sample+filter->coeff[1]*filter->prev_output;
    filter->prev_output = filter->output;
    return filter->output;
}