#include "fir_filter.h"

void FIRFilterInit(FIR_filter* filter, float* circ_buffer, uint16_t buffer_length, float* filter_impulse_response, uint16_t fir_filter_length){
    filter->circ_buffer = circ_buffer;
    filter->buffer_length = buffer_length;
    filter->buffer_index = 0;

    filter->filter_impulse_response = filter_impulse_response;
    filter->fir_filter_length = fir_filter_length;

    filter->out = 0.0f;

    for(uint16_t i = 0; i < filter->buffer_length; ++i) {
        filter->circ_buffer[i] = 0.0f;
    }
}


float FIRFilterUpdate(FIR_filter* filter, const float input) {
    filter->circ_buffer[filter->buffer_index] = input;

    filter->buffer_index++;

    if(filter->buffer_index == filter->fir_filter_length) {
        filter->buffer_index = 0;
    }

    filter->out = 0.0f;
    uint16_t sum_index = filter->buffer_index;

    for(uint16_t j = 0; j < filter->buffer_length; ++j) {
        if(sum_index > 0) {
            --sum_index;
        }
        else {
            sum_index = filter->fir_filter_length - 1;
        }

        filter->out += filter->filter_impulse_response[j]*filter->circ_buffer[sum_index];
    }

    return filter->out;
}