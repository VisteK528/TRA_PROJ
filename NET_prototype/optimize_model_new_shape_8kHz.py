import tensorflow_datasets as tfds
import tensorflow as tf
import numpy as np
import librosa
import os
import sys
sys.path.append("../")
from DSP_prototype.DSP_algorithms import stft

sample_rate = 8000
selected_labels = []
dictionary_classes = {1: 'go', 2: 'left', 3: 'no', 6: 'right', 7: 'stop', 9: 'yes', 10: 'silence', 11: 'unknown'}


def split_data_and_labels(dataset, length):
    data = []
    labels = []
    i = 0
    for feature, label in dataset:
        label_value = int(label.numpy())

        # Pomijanie przykładów z etykietą 3
        if label_value not in dictionary_classes.keys():
            continue
        if i == length:
            break
        pcm = feature.numpy()
        pcm = pcm.astype(np.float32)
        pcm = librosa.resample(pcm, orig_sr=16000, target_sr=8000)
        pcm_padded = np.pad(pcm, (0, sample_rate - len(pcm)), 'constant', constant_values=0)
        data.append(pcm_padded)
        labels.append(int(label.numpy()))
        i += 1
    return np.vstack(data), np.vstack(labels)


def get_spectrogram(pcm):
    D, f, t = stft(pcm, 8000, 256, 128)
    return np.abs(D)


if __name__ == "__main__":
    # Load the Speech Commands dataset
    dataset, info = tfds.load('speech_commands', with_info=True, as_supervised=True)

    train_data = dataset['train']

    print(f"Train data length: {len(train_data)}")

    train_data, train_labels = split_data_and_labels(train_data, length=1000)

    train_data_max = np.max(np.abs(train_data))

    # for i in range(len(train_data)):
    #     train_data[i] /= train_data_max

    train_data = np.array([get_spectrogram(x) for x in train_data])
    train_data = train_data.reshape(len(train_data), train_data[0].shape[0], train_data[0].shape[1], 1)
    train_data = train_data.astype(np.float32)

    model = tf.keras.models.load_model("tra_medium_model_86test_acc_8kHz_PioterSTFT.keras")
    model.summary()


    def representative_dataset_gen():
        for input_value in train_data[:20000]:
            yield [tf.expand_dims(input_value, 0)]


    # Convert the model.
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    converter.optimizations = [tf.lite.Optimize.OPTIMIZE_FOR_SIZE]
    converter.representative_dataset = representative_dataset_gen
    converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8, tf.float32]
    #converter.target_spec.supported_ops = [tf.float16]
    converter.inference_input_type = tf.float32
    converter.inference_output_type = tf.float32
    tflite_model = converter.convert()

    with open('tra_medium_model_86test_acc_8kHz_PioterSTFT_float_float_int.tflite', 'wb') as f:
        f.write(tflite_model)

    model_Size = os.path.getsize("tra_medium_model_86test_acc_8kHz_PioterSTFT_float_float_int.tflite")
    print(f"Model ma {model_Size / 1e3} kB")