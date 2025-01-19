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
    float probability;
    for (int i = 0; i < length; i++) {
        probability = array[i];
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
            HD44780_PrintStr(&available_classes_str[i*6]);
        }
    }
}

//void noise_reduction_stft(float* signal, int n_fft){
//    float noise_est[n_fft / 2 + 1];
//    memcpy(noise_est, )
//}

void resize_image(const float *input_image, int input_height, int input_width, float *output_image, int target_height, int target_width){
    // Calculate scaling factors
    float scale_height = (float)input_height / target_height;
    float scale_width = (float)input_width / target_width;

    // Iterate over each pixel in the target image
    for (int y = 0; y < target_height; y++) {
        for (int x = 0; x < target_width; x++) {
            // Map target pixel to source coordinates
            float src_y = (y + 0.5f) * scale_height - 0.5f;
            float src_x = (x + 0.5f) * scale_width - 0.5f;

            // Calculate integer bounding box of the source coordinates
            int y0 = (int)floor(src_y);
            int x0 = (int)floor(src_x);
            int y1 = min(y0 + 1, input_height - 1);
            int x1 = min(x0 + 1, input_width - 1);

            // Handle edge cases for the coordinates
            y0 = max(0, y0);
            x0 = max(0, x0);

            // Compute interpolation weights
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

            // Ensure no negative values
            value = max(0.0f, value);

            // Assign to the output image
            //output_image[y * target_width + x] = value / 32768.f;
            output_image[y * target_width + x] = value;
        }
    }
}