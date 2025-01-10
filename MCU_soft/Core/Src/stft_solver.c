#include "stft_solver.h"

void STFT_Init(STFT_solver *stft_solver, uint16_t fft_size, uint16_t hop_size, uint16_t signal_length, const float *window) {
    stft_solver->fft_size = fft_size;
    stft_solver->hop_size = hop_size;
    stft_solver->signal_length = signal_length;
    stft_solver->window = window;

    uint16_t num_frames = (signal_length - fft_size) / hop_size + 1;
    uint16_t frame_length = fft_size / 2 + 1;
    stft_solver->out_length = num_frames * frame_length;

    stft_solver->out = (float *)calloc(stft_solver->out_length, sizeof(float));
    if (stft_solver->out == NULL) {
        while(1);
    }
}

void STFT_Free(STFT_solver *stft_solver) {
    if (stft_solver->out) {
        free(stft_solver->out);
        stft_solver->out = NULL;
    }
}

void STFT_Process(STFT_solver *stft_solver, const float *signal) {
    uint16_t num_frames = (stft_solver->signal_length - stft_solver->fft_size) / stft_solver->hop_size + 1;
    uint16_t frame_length = stft_solver->fft_size / 2 + 1;
    float32_t fft_output[stft_solver->fft_size];

    arm_rfft_fast_instance_f32 fft_instance;
    arm_rfft_fast_init_f32(&fft_instance, stft_solver->fft_size);

    for (uint16_t frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        const float *frame = &signal[frame_idx * stft_solver->hop_size];

        for (uint16_t sample_idx = 0; sample_idx < stft_solver->fft_size; sample_idx++) {
            fft_output[sample_idx] = frame[sample_idx] * stft_solver->window[sample_idx];
        }

        arm_rfft_fast_f32(&fft_instance, fft_output, fft_output, 0);
        for (uint16_t sample_idx = 0; sample_idx < frame_length; sample_idx++) {
            stft_solver->out[frame_idx * frame_length + sample_idx] = fft_output[sample_idx];
        }
    }
}
