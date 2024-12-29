#ifndef FIR_FILTER_H
#define FIR_FILTER_H

#include <stdint.h>

typedef struct {
    float* circ_buffer;
    uint16_t buffer_length;
    uint16_t buffer_index;

    float* filter_impulse_response;
    uint16_t fir_filter_length;

    float out;
} FIR_filter;

void FIRFilterInit(FIR_filter* filter, float* circ_buffer, uint16_t buffer_length, float* filter_impulse_response, uint16_t fir_filter_length);
float FIRFilterUpdate(FIR_filter* filter, float input);

#endif //FIR_FILTER_H
