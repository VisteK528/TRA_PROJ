#include "stft_solver.h"

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

uint8_t STFT_Process_ColumnMajor(STFT_with_filter_solver *stft_solver, const float *signal) {
    uint16_t num_frames = (stft_solver->signal_length - stft_solver->fft_size) / stft_solver->hop_size + 1;
    uint16_t frame_length = stft_solver->fft_size / 2 + 1; // tylko częstotliwości do Nyquista
    float32_t fft_output[stft_solver->fft_size]; // Wynik FFT w formacie przeplatanym Re, Im

    arm_rfft_fast_instance_f32 fft_instance;
    arm_rfft_fast_init_f32(&fft_instance, stft_solver->fft_size);

    for (uint16_t frame_idx = 0; frame_idx < num_frames; frame_idx++) {
        const float *frame = &signal[frame_idx * stft_solver->hop_size];

        // Zastosowanie okna do ramki
        for (uint16_t sample_idx = 0; sample_idx < stft_solver->fft_size; sample_idx++) {
            fft_output[sample_idx] = frame[sample_idx] * stft_solver->window[sample_idx];
        }

        // Obliczanie FFT
        arm_rfft_fast_f32(&fft_instance, fft_output, fft_output, 0);

        // Obliczanie modułu spektrum dla częstotliwości do Nyquista
        for (uint16_t sample_idx = 0; sample_idx < frame_length; sample_idx++) {
            float32_t real = fft_output[2 * sample_idx];     // Część rzeczywista
            float32_t imag = fft_output[2 * sample_idx + 1]; // Część urojona

            if (sample_idx < stft_solver->stft_filter_mask_length) {
        		stft_solver->out[frame_idx * frame_length + sample_idx] =
                        sqrtf(real * real + imag * imag) * stft_solver->stft_filter_mask[sample_idx];
			} else {
				stft_solver->out[frame_idx * frame_length + sample_idx] = sqrtf(real * real + imag * imag);
			}
        }
    }
    return 0;
}

uint8_t STFT_Process_RowMajor(STFT_with_filter_solver *stft_solver, const float *signal) {
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


uint8_t STFT_Process_RowMajor_Complex(STFT_with_filter_solver *stft_solver, const float *signal) {
    uint16_t num_frames = (stft_solver->signal_length - stft_solver->fft_size) / stft_solver->hop_size + 1;
    uint16_t frame_length = stft_solver->fft_size / 2 + 1; // Number of frequency bins
    uint16_t output_length = frame_length * 2;            // Interwoven real + imag

    float32_t fft_output[stft_solver->fft_size]; // Temporary buffer for FFT output

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

        // Store interwoven real and imaginary parts, scaled by FFT length
        for (uint16_t sample_idx = 0; sample_idx < frame_length; sample_idx++) {
            float32_t real = fft_output[2 * sample_idx];     // Real part
            float32_t imag = fft_output[2 * sample_idx + 1]; // Imaginary part

            uint16_t out_idx = (sample_idx * num_frames) + frame_idx;
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


uint8_t ISTFT_Process(ISTFT_with_filter_solver *istft_solver, const float *stft_data, bool is_complex) {
    uint16_t fft_size = istft_solver->fft_size;
    uint16_t half_fft_size = fft_size / 2 + 1;
    uint16_t frame_length = fft_size;
    float32_t ifft_output[fft_size * 2];  // Complex output (interleaved real + imag)
    float32_t ifft_real_output[fft_size]; // Real-valued signal after IFFT

    // Initialize the Radix-2 CFFT instance
    arm_cfft_radix2_instance_f32 cfft_instance;
    if (arm_cfft_radix2_init_f32(&cfft_instance, fft_size, 1, 1) != ARM_MATH_SUCCESS) {
        return 1; // Initialization failed
    }

    // Precompute normalization factor
    float normalization_factor = 0.0f;
    for (uint16_t i = 0; i < frame_length; i++) {
        normalization_factor += istft_solver->window[i] * istft_solver->window[i];
    }
    normalization_factor = 1.0f / normalization_factor;

    for (uint16_t frame_idx = 0; frame_idx < istft_solver->num_frames; frame_idx++) {
        // Prepare the IFFT input (complex values, interleaved real and imaginary)
        for (uint16_t freq_idx = 0; freq_idx < half_fft_size; freq_idx++) {
            uint16_t input_idx = frame_idx * half_fft_size * 2 + freq_idx * 2;
            ifft_output[2 * freq_idx] = stft_data[input_idx];       // Real part
            ifft_output[2 * freq_idx + 1] = is_complex ? stft_data[input_idx + 1] : 0.0f; // Imaginary part
        }

        // Enforce conjugate symmetry for IFFT
        for (uint16_t freq_idx = half_fft_size; freq_idx < fft_size; freq_idx++) {
            uint16_t mirrored_idx = fft_size - freq_idx;
            ifft_output[2 * freq_idx] = ifft_output[2 * mirrored_idx];       // Real part
            ifft_output[2 * freq_idx + 1] = -ifft_output[2 * mirrored_idx + 1]; // Imaginary part
        }

        // Perform inverse FFT (complex-to-complex IFFT)
        arm_cfft_radix2_f32(&cfft_instance, ifft_output);

        // Extract real part from IFFT output
        for (uint16_t sample_idx = 0; sample_idx < frame_length; sample_idx++) {
            ifft_real_output[sample_idx] = ifft_output[2 * sample_idx] / fft_size; // Normalize by FFT size
        }

        // Overlap-add and apply the Hann window
        for (uint16_t sample_idx = 0; sample_idx < frame_length; sample_idx++) {
            uint32_t output_idx = frame_idx * istft_solver->hop_size + sample_idx;
            if (output_idx < istft_solver->signal_length) {
                istft_solver->reconstruction[output_idx] +=
                        (ifft_real_output[sample_idx] * istft_solver->window[sample_idx]) * normalization_factor;
            }
        }
    }

    return 0;
}







void ISTFT_Free(ISTFT_with_filter_solver *istft_solver) {
    if (istft_solver->reconstruction) {
        istft_solver->reconstruction = NULL;
    }
}


