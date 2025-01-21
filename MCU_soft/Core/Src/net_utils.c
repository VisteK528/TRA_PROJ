#include "net_utils.h"
#include "math.h"
#include "memory.h"


float max(const float a, const float b) {
    return a > b ? a : b;
}


float min(const float a, const float b) {
    return a < b ? a : b;
}

int argmax(const float* array, size_t length){
    int predicted_class = 0;
    float max_probability = array[0];
    for (int i = 0; i < length; i++) {
        float probability = array[i];
        if (probability > max_probability) {
            max_probability = probability;
            predicted_class = i;
        }
    }
    return predicted_class;
}

void DisplayPredictedClass(int prediction, int num_classes, const char* available_classes_str){
    for(int i = 0; i < num_classes; ++i){
        if(i == prediction){
            HD44780_SetCursor(0, 1);
            HD44780_PrintStr(&available_classes_str[i*7]);
        }
    }
}

void noise_reduction_stft(float* signal, int n_fft, int num_frames, bool complex_output){
    const int noise_frames = 10; // Number of frames used to estimate noise power
    int n_bins = n_fft / 2 + 1;  // Number of frequency bins per frame

    // Estimate noise power spectrum for each frequency bin
    float noise_power[129];
    for (int bin = 0; bin < n_bins; ++bin) {
        noise_power[bin] = 0.0f;
        for (int frame = 0; frame < noise_frames; ++frame) {
            float real_part = signal[bin * num_frames * 2 + frame * 2];
            float imag_part = signal[bin * num_frames * 2 + frame * 2 + 1];
            float magnitude_squared = real_part * real_part + imag_part * imag_part;

            // Accumulate magnitude squared
            noise_power[bin] += magnitude_squared;
        }
        // Average over noise_frames
        noise_power[bin] /= (float)noise_frames;
    }

    for (int frame = 0; frame < num_frames; ++frame) {
        for (int bin = 0; bin < n_bins; ++bin) {
            // Calculate SNR for this frequency bin
            float real_part = signal[bin * num_frames * 2 + frame * 2];
            float imag_part = signal[bin * num_frames * 2 + frame * 2 + 1];

            float magnitude = sqrtf(real_part * real_part + imag_part * imag_part);

            float phase = atan2f(imag_part, real_part);
            float noisy_power = magnitude*magnitude;
            float snr = fmaxf(noisy_power / (noise_power[bin] + 1e-6f) - 1.0f, 0.0f);

            // Compute gain function
            float gain = snr / (1.0f + snr);

            float out_real = magnitude*gain*cosf(phase);
            float out_imag = magnitude*gain*sinf(phase);

            // Apply gain to the magnitude spectrum
            if(complex_output == true){
                signal[bin * num_frames * 2 + frame * 2] = out_real;
                signal[bin * num_frames * 2 + frame * 2 + 1] = out_imag;
            }
            else{
                signal[bin * num_frames + frame] = sqrtf(out_real*out_real + out_imag*out_imag);
            }
        }
    }
}

void resize_image(const float *input_image, int input_height, int input_width, float *output_image, int target_height, int target_width){
    float scale_height = (float)input_height / target_height;
    float scale_width = (float)input_width / target_width;

    for (int y = 0; y < target_height; y++) {
        for (int x = 0; x < target_width; x++) {
            float src_y = (y + 0.5f) * scale_height - 0.5f;
            float src_x = (x + 0.5f) * scale_width - 0.5f;

            int y0 = (int)floor(src_y);
            int x0 = (int)floor(src_x);
            int y1 = min(y0 + 1, input_height - 1);
            int x1 = min(x0 + 1, input_width - 1);

            y0 = max(0, y0);
            x0 = max(0, x0);

            float dy = src_y - y0;
            float dx = src_x - x0;

            // Bilinear interpolation
            float v00 = input_image[y0 * input_width + x0];
            float v01 = input_image[y0 * input_width + x1];
            float v10 = input_image[y1 * input_width + x0];
            float v11 = input_image[y1 * input_width + x1];

            float value =
                    v00 * (1 - dx) * (1 - dy) +
                    v01 * dx * (1 - dy) +
                    v10 * (1 - dx) * dy +
                    v11 * dx * dy;

            value = max(0.0f, value);

            output_image[y * target_width + x] = value;
        }
    }
}