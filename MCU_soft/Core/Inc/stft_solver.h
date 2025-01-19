#ifndef STFT_SOLVER_H
#define STFT_SOLVER_H

#ifndef __FPU_PRESENT
#define __FPU_PRESENT 1
#endif

#include <stdint.h>
#include <stdlib.h>
#include <arm_math.h>


typedef struct {
    uint16_t fft_size;
    uint16_t hop_size;
    uint16_t signal_length;
    const float *window;
    uint16_t out_length;
    float *out;
    uint8_t stft_filter_mask_length;
    const float *stft_filter_mask;
} STFT_with_filter_solver;

void STFT_Init(STFT_with_filter_solver *stft_solver, uint16_t fft_size, uint16_t hop_size, uint16_t signal_length,
			   const float *window, uint8_t stft_filer_mask_length, const float *stft_filter_mask, float* out_buffer);
void STFT_Free(STFT_with_filter_solver *stft_solver);

uint8_t STFT_Process_ColumnMajor(STFT_with_filter_solver *stft_solver, const float *signal);
uint8_t STFT_Process_RowMajor(STFT_with_filter_solver *stft_solver, const float *signal);

#endif
