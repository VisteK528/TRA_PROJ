#include "stft_solver.h"

static float hannWindow(int n, int N){
    return 0.5f * (1.f - cosf(2.f * (float)M_PI * (float)n / ((float)N - 1.f)));
}

void STFT_Init(STFT_with_filter_solver *stft_solver, uint16_t fft_size, uint16_t hop_size, uint16_t signal_length,
			   const float *window, uint8_t stft_filer_mask_length, const float *stft_filter_mask, float* out_buffer) {
    stft_solver->fft_size = fft_size;
    stft_solver->hop_size = hop_size;
    stft_solver->signal_length = signal_length;
    stft_solver->window = window;
    stft_solver->stft_filter_mask_length = stft_filer_mask_length;
    stft_solver->stft_filter_mask = stft_filter_mask;

    uint16_t num_frames = (signal_length - fft_size) / hop_size + 1;
    uint16_t frame_length = fft_size / 2 + 1;
    stft_solver->out_length = num_frames * frame_length;

    stft_solver->out = out_buffer;
    memset(stft_solver->out, 0, sizeof(out_buffer));
}

void STFT_Free(STFT_with_filter_solver *stft_solver) {
    if (stft_solver->out) {
        free(stft_solver->out);
        stft_solver->out = NULL;
    }
}

uint8_t STFT_Process(STFT_with_filter_solver *stft_solver, const float *signal) {
    uint16_t num_frames = (stft_solver->signal_length - stft_solver->fft_size) / stft_solver->hop_size + 1;
    uint16_t frame_length = stft_solver->fft_size / 2 + 1;
    float32_t fft_output[stft_solver->fft_size];

    arm_rfft_fast_instance_f32 fft_instance;
    arm_rfft_fast_init_f32(&fft_instance, stft_solver->fft_size);

    for (uint16_t frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        const float *frame = &signal[frame_idx * stft_solver->hop_size];

        // Apply window function and normalize
        for (uint16_t sample_idx = 0; sample_idx < stft_solver->fft_size; sample_idx++) {
            fft_output[sample_idx] = frame[sample_idx] * stft_solver->window[sample_idx];
        }

        // Perform FFT
        arm_rfft_fast_f32(&fft_instance, fft_output, fft_output, 0);

        // Scale by FFT length and compute magnitude
        for (uint16_t sample_idx = 0; sample_idx < frame_length; sample_idx++) {
            float32_t real = fft_output[2 * sample_idx];
            float32_t imag = fft_output[2 * sample_idx + 1];

            stft_solver->out[sample_idx * num_frames + frame_idx] =
                    sqrtf(real * real + imag * imag);
        }
    }
    return 0;
}


uint8_t STFT_ProcessComplex(STFT_with_filter_solver *stft_solver, const float *signal) {
    uint16_t num_frames = (stft_solver->signal_length - stft_solver->fft_size) / stft_solver->hop_size + 1;
    uint16_t num_bins = stft_solver->fft_size / 2 + 1;

    float32_t fft_output[stft_solver->fft_size];

    arm_rfft_fast_instance_f32 fft_instance;
    arm_rfft_fast_init_f32(&fft_instance, stft_solver->fft_size);

    for (uint16_t frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        const float *frame = &signal[frame_idx * stft_solver->hop_size];

        for (uint16_t sample_idx = 0; sample_idx < stft_solver->fft_size; sample_idx++) {
            fft_output[sample_idx] = frame[sample_idx] * stft_solver->window[sample_idx];
        }

        arm_rfft_fast_f32(&fft_instance, fft_output, fft_output, 0);

        // Store interwoven real and imaginary parts
        for (uint16_t bin = 0; bin < num_bins; bin++) {
            float32_t real = fft_output[2 * bin];     // Real part
            float32_t imag = fft_output[2 * bin + 1]; // Imaginary part

            uint16_t out_idx = (bin * num_frames) + frame_idx;
            stft_solver->out[2*out_idx] = real;
            stft_solver->out[2*out_idx + 1] = imag;
        }
    }

    return 0;
}


void ISTFT_Init(ISTFT_with_filter_solver *istft_solver, uint16_t fft_size, uint16_t hop_size, uint16_t signal_length,
                const float *window, float *reconstruction_buffer) {
    istft_solver->fft_size = fft_size;
    istft_solver->hop_size = hop_size;
    istft_solver->signal_length = signal_length;
    istft_solver->window = window;

    uint16_t num_frames = (signal_length - fft_size) / hop_size + 1;
    istft_solver->num_frames = num_frames;
    istft_solver->reconstruction_length = signal_length;

    istft_solver->reconstruction = reconstruction_buffer;
    memset(istft_solver->reconstruction, 0, sizeof(float) * signal_length);
}


uint8_t ISTFT_Process(ISTFT_with_filter_solver *istft_solver, const float *stft_data) {
    uint16_t fft_size = istft_solver->fft_size;
    uint16_t half_fft_size = fft_size / 2;
    uint16_t frame_length = fft_size;
    float32_t ifft_output[512];
    float32_t ifft_real_output[fft_size];
    float hannWindowArray[fft_size];
    float hannWindowCoefficients[8000];

    // Initialize Hann window
    for (uint16_t i = 0; i < fft_size; ++i) {
        hannWindowArray[i] = hannWindow(i, fft_size);
    }

    memset(istft_solver->reconstruction, 0, istft_solver->signal_length * sizeof(float));
    memset(hannWindowCoefficients, 0, 8000 * sizeof(float));

    arm_cfft_instance_f32 cfft_instance;
    if (arm_cfft_init_f32(&cfft_instance, fft_size) != ARM_MATH_SUCCESS) {
        return 1;
    }

    for (uint16_t frame_idx = 0; frame_idx < istft_solver->num_frames; frame_idx++) {
        memset(ifft_output, 0, fft_size*2*sizeof(float));
        memset(ifft_real_output, 0, fft_size*sizeof(float));


        for (uint16_t freq_idx = 0; freq_idx < half_fft_size; freq_idx++) {
            uint16_t input_idx = (freq_idx * istft_solver->num_frames) + frame_idx;

            ifft_output[2 * freq_idx] = stft_data[2 * input_idx];         // Real part
            ifft_output[2 * freq_idx + 1] = stft_data[2 * input_idx + 1]; // Imaginary part
        }

        // Enforce conjugate symmetry for IFFT
        for (uint16_t freq_idx = half_fft_size; freq_idx < fft_size; freq_idx++) {
            uint16_t mirrored_idx = fft_size - freq_idx;
            ifft_output[2 * freq_idx] = ifft_output[2 * mirrored_idx];       // Real part
            ifft_output[2 * freq_idx + 1] = -ifft_output[2 * mirrored_idx + 1]; // Imaginary part
        }

        arm_cfft_f32(&cfft_instance, ifft_output, 1, 1);

        // Extract real part from IFFT output and apply the Hann window
        for (uint16_t sample_idx = 0; sample_idx < frame_length; sample_idx++) {
            ifft_real_output[sample_idx] = ifft_output[2 * sample_idx] * hannWindowArray[sample_idx];
        }

        uint16_t start = frame_idx * istft_solver->hop_size;
        for (uint16_t k = 0; k < fft_size; ++k) {
            istft_solver->reconstruction[start + k] += ifft_real_output[k];
            hannWindowCoefficients[start + k] += hannWindowArray[k];
        }
    }

    // Normalize by Hann window coefficients
    for (uint16_t j = 0; j < istft_solver->signal_length; ++j) {
        if (hannWindowCoefficients[j] != 0) {
            istft_solver->reconstruction[j] /= hannWindowCoefficients[j];
        }
    }

    return 0;
}


void ISTFT_Free(ISTFT_with_filter_solver *istft_solver) {
    if (istft_solver->reconstruction) {
        istft_solver->reconstruction = NULL;
    }
}


