#include "decimation_filter.h"

void DecimationFilterInit(DecimationFilter* filter, FIR_filter* fir, uint8_t M) {
    filter->aa_filter = fir;
    filter->downsampling_counter = 0;

    filter->out = 0.0f;
    filter->M = M;
}

uint8_t DecimationFilterUpdate(DecimationFilter* filter, const float in) {
    FIRFilterUpdate(filter->aa_filter, in);

    filter->downsampling_counter++;

    uint8_t data_ready = 0;

    if(filter->downsampling_counter == filter->M) {
        filter->downsampling_counter = 0;

        filter->out = filter->aa_filter->out;

        data_ready = 1;
    }
    return data_ready;
}