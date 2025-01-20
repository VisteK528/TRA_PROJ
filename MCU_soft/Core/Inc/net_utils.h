#ifndef MCU_SOFT_NET_UTILS_H
#define MCU_SOFT_NET_UTILS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "liquid_crystal_i2c.h"


float max(float a, float b);
float min(float a, float b);

int argmax(const float* array, size_t length);
void DisplayPredictedClass(int prediction, int num_classes, const char* available_classes_str);

void noise_reduction_stft(float* signal, int n_fft, int num_frames, bool complex_output);

/* Resize image using bilinear interpolation
 * */
void resize_image(const float *input_image, int input_height, int input_width, float *output_image, int target_height, int target_width);

#endif //MCU_SOFT_NET_UTILS_H
