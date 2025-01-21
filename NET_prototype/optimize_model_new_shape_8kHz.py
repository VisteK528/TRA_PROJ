import tensorflow_datasets as tfds
import tensorflow as tf
import keras
import numpy as np
import librosa
import os
import sys
sys.path.append("../")
from DSP_prototype.DSP_algorithms import stft
import csv
from tensorflow_model_optimization.python.core.quantization.keras import \
        quantize_layer
from tensorflow_model_optimization.python.core.quantization.keras import \
    quantize_wrapper
from tensorflow_model_optimization.python.core.quantization.keras import \
    quantize_config
from tensorflow_model_optimization.python.core.quantization.keras import quantizers
import tensorflow_model_optimization as tfmot

original_sr = 16000
label_strings = ["down", "go", "left", "no", "off", "on", "right", "stop", "up", "yes", "silence", "unknown"]
selected_labels = [1, 2, 6, 7, 10, 11]
selected_labels_dict = {x:i for i, x in enumerate(selected_labels)}
NUM_CLASSES = len(selected_labels)
commands = [label_strings[x] for x in selected_labels]
selected_labels_str = [label_strings[x] for x in selected_labels]
prediction_dict = {i: label for i, label in enumerate(selected_labels_str)}
prediction_dict_reversed = {label: i for i, label in enumerate(selected_labels_str)}


def split_data_and_labels(dataset, length):
    data = []
    labels = []
    i = 0
    unknown_counter = 0
    for feature, label in dataset:
        if i == length:
            break

        pcm = feature.numpy()
        pcm_padded = np.pad(pcm, (0, original_sr - len(pcm)), 'constant',
                            constant_values=0)
        pcm_padded = pcm_padded.astype(np.float32)
        pcm_padded = librosa.resample(pcm_padded, orig_sr=16e3, target_sr=8e3)
        label = int(label.numpy())
        if label in selected_labels:
            if label == 11 and unknown_counter < 5:
                unknown_counter += 1

            if (label == 11 and unknown_counter == 5) or label != 11:
                data.append(pcm_padded)
                labels.append(selected_labels_dict.get(label))
                i += 1
                unknown_counter = 0
    return np.vstack(data), np.vstack(labels)


def get_spectrogram(pcm):
    D, f, t = stft(pcm, 8000, 256, 128)
    return np.abs(D)


class DefaultBNQuantizeConfig(tfmot.quantization.keras.QuantizeConfig):

    def get_weights_and_quantizers(self, layer):
        return []

    def get_activations_and_quantizers(self, layer):
        return []

    def set_quantize_weights(self, layer, quantize_weights):
        pass

    def set_quantize_activations(self, layer, quantize_activations):
        pass

    def get_output_quantizers(self, layer):
        return [tfmot.quantization.keras.quantizers.MovingAverageQuantizer(
            num_bits=8, per_axis=False, symmetric=False, narrow_range=False)]

    def get_config(self):
        return {}


if __name__ == "__main__":
    # Convert the model.

    # Register the custom objects used by quantization layers
    custom_objects = {
        'QuantizeLayer': quantize_layer.QuantizeLayer,
        'QuantizeWrapperV2': quantize_wrapper.QuantizeWrapperV2,
        'QuantizeBatchNormalization': quantize_wrapper.QuantizeWrapperV2,
        'MovingAverageQuantizer': quantizers.MovingAverageQuantizer,
    }

    with tf.keras.utils.custom_object_scope(custom_objects):
        model = keras.models.load_model("tra_qat_model.h5")
        model.summary()
    #
    # # Load the Speech Commands dataset
    # dataset, info = tfds.load('speech_commands', with_info=True, as_supervised=True)
    #
    # train_data = dataset['train']
    #
    # print(f"Train data length: {len(train_data)}")
    #
    # train_data, train_labels = split_data_and_labels(train_data, length=20000)
    #
    # print(max(train_labels))
    #
    # print(f"New length {len(train_data)}")
    directory_path = "../Data_acquisition/data/audio/8kHz_sampling"
    file_list = [f for f in os.listdir(directory_path) if
                 os.path.isfile(os.path.join(directory_path, f))]

    samples = []
    for file_name in file_list:
        with open(os.path.join(directory_path, file_name), 'r') as file:
            file_data = []
            reader = csv.DictReader(file)
            for row in reader:
                file_data.append(float(row['y']))  # Convert to int if y is numeric

            if len(file_data) == 8e3:
                samples.append(file_data)

    samples = np.array(samples)

    #train_data += np.random.normal(0, 0.01, train_data.shape)
    train_data = np.array([get_spectrogram(x) for x in samples])
    train_data = train_data.reshape(len(train_data), train_data[0].shape[0], train_data[0].shape[1], 1)
    train_data = tf.image.resize(train_data, [32, 32])



    def representative_dataset_gen():
        for input_value in train_data[:2000]:
            yield [tf.expand_dims(input_value, 0)]


    # Wrap the conversion in a custom_object_scope
    with tf.keras.utils.custom_object_scope(custom_objects):
        converter = tf.lite.TFLiteConverter.from_keras_model(model)
        converter.optimizations = [tf.lite.Optimize.DEFAULT]
        converter.representative_dataset = representative_dataset_gen
        converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
        converter.inference_input_type = tf.float32  # Optional, can be set to tf.int8 if required
        converter.inference_output_type = tf.float32  # Optional, can be set to tf.int8 if required
        tflite_model = converter.convert()


    with open('to_be_deployed/tra_qat_model.tflite', 'wb') as f:
        f.write(tflite_model)

    model_Size = os.path.getsize("to_be_deployed/tra_qat_model.tflite")
    print(f"Model ma {model_Size / 1e3} kB")