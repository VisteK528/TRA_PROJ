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

original_sr = 16000
dictionary_classes = {1: 'go', 2: 'left', 3: 'no', 6: 'right', 7: 'stop', 9: 'yes', 10: 'silence', 11: 'unknown'}
label_strings = ["down", "go", "left", "no", "off", "on", "right", "stop", "up", "yes", "silence", "unknown"]
selected_labels = [1, 2, 3, 6, 7, 9, 10, 11]
selected_labels_dict = {x:i for i, x in enumerate(selected_labels)}


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


if __name__ == "__main__":
    model = keras.models.load_model("tra_model_for_optimization5.h5")
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


    # Convert the model.
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.DEFAULT]
    converter.representative_dataset = representative_dataset_gen
    converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
    converter.inference_input_type = tf.float32
    converter.inference_output_type = tf.float32
    tflite_model = converter.convert()


    with open('tra_model_for_optimization5.tflite', 'wb') as f:
        f.write(tflite_model)

    model_Size = os.path.getsize("tra_model_for_optimization5.tflite")
    print(f"Model ma {model_Size / 1e3} kB")