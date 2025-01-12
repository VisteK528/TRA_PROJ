import tensorflow_datasets as tfds
import tensorflow as tf
import librosa
import numpy as np
from scipy.signal import butter, lfilter
from DSP_prototype import DSP_algorithms

sample_rate = 8000
dictionary_classes = {1: 'go', 2:'left', 3:'no', 6:'right', 7:'stop',9: 'yes',10:'silence', 11:'unknown' }
all_worlds = 0
good_claffified = 0

def split_data_and_labels(dataset, length):
    data = []
    labels = []
    i = 100
    for feature, label in dataset:
        if i == length+100:
            break
        pcm = feature.numpy().astype(np.float32)
        pcm = librosa.resample(pcm, orig_sr=16000, target_sr=8000)
        pcm_padded = np.pad(pcm, (0, sample_rate - len(pcm)), 'constant', constant_values=0)
        data.append(pcm_padded)
        labels.append(int(label.numpy()))
        i += 1
    return np.vstack(data), np.vstack(labels)

def get_spectrogram(pcm):
    D, f, t = DSP_algorithms.stft(pcm, 8000, 256, 128)
    return np.abs(D)


if __name__ == "__main__":
    # Load the Speech Commands dataset
    dataset, info = tfds.load('speech_commands', with_info=True, as_supervised=True)

    train_data = dataset['train']
    print(f"Train data length: {len(train_data)}")

    train_data, train_labels = split_data_and_labels(train_data, length=5000)

    train_data_max = np.max(np.abs(train_data))
    for i in range(len(train_data)):
        train_data[i] /= train_data_max
        

    train_data = np.array([get_spectrogram(x) for x in train_data])

    interpreter = tf.lite.Interpreter(model_path="test_new_shape_8kHz_0.288b.tflite")
    interpreter.allocate_tensors()

    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()

    label_strings = ["go", "left", "no", "right", "stop", "yes", "silence", "unknown"]
    
    for idx, feature in enumerate(train_data):
        feature = feature.reshape(1, feature.shape[0], feature.shape[1], 1).astype(np.float32)
        interpreter.set_tensor(input_details[0]['index'], feature)
        interpreter.invoke()
        output_data = interpreter.get_tensor(output_details[0]['index'])

        predicted_class = np.argmax(output_data)
        predicted_label = label_strings[predicted_class]
        # if not(train_labels[idx][0] == 11 and predicted_label == 'unknown'):
        if train_labels[idx][0] in dictionary_classes.keys():
            # print(f"Predicted word: {predicted_label}/{train_labels[idx]}")
            all_worlds += 1
            if dictionary_classes[train_labels[idx][0]] == predicted_label:
                good_claffified += 1

            print(f"Predicted word: {predicted_label}/{train_labels[idx]}")

    print(all_worlds)
    print(good_claffified)
    print(good_claffified/all_worlds)
