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
} STFT_solver;

void STFT_Init(STFT_solver *stft_solver, uint16_t fft_size, uint16_t hop_size, uint16_t signal_length, const float *window);
void STFT_Free(STFT_solver *stft_solver);
void STFT_Process(STFT_solver *stft_solver, const float *signal);

#endif
