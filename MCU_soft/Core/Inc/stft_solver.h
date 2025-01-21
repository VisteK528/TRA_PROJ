#ifndef STFT_SOLVER_H
#define STFT_SOLVER_H

#ifndef __FPU_PRESENT
#define __FPU_PRESENT 1
#endif

#include <stdint.h>
#include <stdlib.h>
#include <arm_math.h>
#include <stdbool.h>

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

typedef struct {
    uint16_t fft_size;               // Size of the FFT (and IFFT)
    uint16_t hop_size;               // Hop size between consecutive frames
    uint16_t signal_length;          // Length of the original signal
    uint16_t num_frames;             // Number of frames
    uint32_t reconstruction_length;  // Length of the reconstructed signal
    const float *window;             // Pointer to the window function array
    float *reconstruction;           // Pointer to the reconstructed signal buffer
} ISTFT_with_filter_solver;


void STFT_Init(STFT_with_filter_solver *stft_solver, uint16_t fft_size, uint16_t hop_size, uint16_t signal_length,
			   const float *window, uint8_t stft_filer_mask_length, const float *stft_filter_mask, float* out_buffer);
void STFT_Free(STFT_with_filter_solver *stft_solver);

uint8_t STFT_Process(STFT_with_filter_solver *stft_solver, const float *signal);
uint8_t STFT_ProcessComplex(STFT_with_filter_solver *stft_solver, const float *signal);

void ISTFT_Init(ISTFT_with_filter_solver *istft_solver, uint16_t fft_size, uint16_t hop_size, uint16_t signal_length,
                const float *window, float *reconstruction_buffer);
uint8_t ISTFT_Process(ISTFT_with_filter_solver *istft_solver, const float *stft_data);

void ISTFT_Free(ISTFT_with_filter_solver *istft_solver);

#endif
