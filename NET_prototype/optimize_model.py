import tensorflow_datasets as tfds
import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np
import librosa
import os
from IPython import display
from tensorflow.keras import layers
from tensorflow.keras import models
from tensorflow.keras.utils import plot_model

sample_rate = 16000
selected_labels = []


def split_data_and_labels(dataset, length):
    data = []
    labels = []
    i = 0
    for feature, label in dataset:
        if i == length:
            break

        pcm = feature.numpy()
        pcm_padded = np.pad(pcm, (0, sample_rate - len(pcm)), 'constant', constant_values=0)
        pcm_padded = pcm_padded.astype(np.float32)
        data.append(pcm_padded)
        labels.append(int(label.numpy()))
        i += 1
    return np.vstack(data), np.vstack(labels)

def get_spectrogram(pcm):
    D = librosa.stft(pcm, n_fft=256, hop_length=128, window='hann')
    return np.abs(D)

if __name__ == "__main__":
    # Load the Speech Commands dataset
    dataset, info = tfds.load('speech_commands', with_info=True, as_supervised=True)

    train_data = dataset['train']

    print(f"Train data length: {len(train_data)}")

    train_data, train_labels = split_data_and_labels(train_data, length=100)

    train_data_max = np.max(np.abs(train_data))

    for i in range(len(train_data)):
        train_data[i] /= train_data_max
        print(i)

    train_data = np.array([get_spectrogram(x) for x in train_data])
    train_data = train_data.reshape(len(train_data), train_data[0].shape[0], train_data[0].shape[1], 1)

    model = tf.keras.models.load_model("tra_prototype_model_87test_acc_balanced_classes.h5")
    model.summary()
    plot_model(model, to_file='images/model.png', show_shapes=True)


    def representative_dataset():
        for input_value in train_data[:100]:
            yield [tf.expand_dims(input_value, 0)]


    # Convert the model.
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    converter.representative_dataset = representative_dataset
    converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
    converter.inference_input_type = tf.int8
    converter.inference_output_type = tf.int8
    tflite_model = converter.convert()

    with open('/models/model_quantized2.tflite', 'wb') as f:
        f.write(tflite_model)

    model_Size = os.path.getsize("/models/model_quantized2.tflite")
    print(f"Model ma {model_Size / 1e6} bajtów")