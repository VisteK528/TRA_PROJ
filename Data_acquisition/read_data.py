import serial
import os

from DSP_prototype.STFT_tests import stft_matrix

# Configuration
PORT = '/dev/ttyACM0'
BAUDRATE = 115200
audio_output_file = "audio_data.csv"

label_strings = [ "go", "left", "no", "right", "stop", "yes", "silence", "unknown"]
selected_labels_dict = {i:x for i, x in enumerate(label_strings, 1)}

sampling_strings = [ "8kHz", "16kHz"]
sampling_strings_dict = {i:x for i, x in enumerate(sampling_strings, 1)}

def choose_from_available(options: dict) -> tuple[int, str]:
    chosen_option_key = -1
    chosen_option_val = None
    chosen = False

    while not chosen:
        try:
            for key, value in options.items():
                print(f"{key:>2}. {value}")

            chosen_option_str = input("Choose one of the available options: ")
            chosen_option_key = int(chosen_option_str)

            assert chosen_option_key in options.keys()
            chosen_option_val = options.get(chosen_option_key)
            chosen = True
        except ValueError:
            print("Chosen value cannot be converted to int. Choose again...")
        except AssertionError:
            print("Chosen value not present in available options. Choose again...")

    return chosen_option_key, chosen_option_val


def get_unique_common_filenames(directory1, directory2, base_name1, base_name2, extension):
    # Combine the base name and extension
    file_name1 = f"{base_name1}.{extension}"
    file_name2 = f"{base_name2}.{extension}"
    counter = 1

    # Loop until a unique file name is found
    while file_name1 in os.listdir(directory1) and file_name2 in os.listdir(directory2):
        file_name1 = f"{base_name1}_{counter}.{extension}"
        file_name2 = f"{base_name2}_{counter}.{extension}"
        counter += 1

    return os.path.join(directory1, file_name1), os.path.join(directory2, file_name2)

if __name__ == "__main__":
    # Open serial port
    ser = serial.Serial(PORT, BAUDRATE)

    # Data acquisition setup
    print("What word will be recorded?")
    _, chosen_word = choose_from_available(selected_labels_dict)
    print()
    print("At what sampling frequency the word will be recorded?")
    _, chosen_sampling = choose_from_available(sampling_strings_dict)

    audio_output_file_base = f"{chosen_word}_fs={chosen_sampling}"
    stft_output_file_base = f"stft_{chosen_word}_fs={chosen_sampling}"
    audio_output_file, stft_output_file = get_unique_common_filenames(
        f"data/audio/{chosen_sampling}_sampling/", f"data/stft/{chosen_sampling}_sampling/",
        audio_output_file_base, stft_output_file_base, ".csv")


    if chosen_sampling == "16kHz":
        break_value = 15999
    else:
        break_value = 7999

    # Open file for saving received data
    with open(AUDIO_OUTPUT_FILE, "w") as f:
        f.writelines("index,y\n")
        try:
            while True:
                # Read a chunk of data
                chunk = ser.read_until(b'\r\n')
                chunk = chunk.decode('utf-8')
                print(chunk, end='')
                f.writelines(chunk)

                number, data = chunk.split(',')

                if int(number) == break_value:
                    break
        except KeyboardInterrupt:
            print("Reception stopped.")
        finally:
            ser.close()
