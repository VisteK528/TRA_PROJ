#ifndef DECIMATION_FILTER_H
#define DECIMATION_FILTER_H

#include "fir_filter.h"

typedef struct {
    FIR_filter* aa_filter;

    uint8_t M;

    uint8_t downsampling_counter;

    float out;
} DecimationFilter;

void DecimationFilterInit(DecimationFilter* filter, FIR_filter* fir, uint8_t M);
uint8_t DecimationFilterUpdate(DecimationFilter* filter, float in);

#endif //DECIMATION_FILTER_H
