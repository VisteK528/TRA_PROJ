import serial
import os

# Configuration
PORT = '/dev/ttyACM0'
BAUDRATE = 115200
OUTPUT_FILE = "audio_data.csv"

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


def get_unique_filename(directory, base_name, extension):
    # Combine the base name and extension
    file_name = f"{base_name}.{extension}"
    counter = 1

    # Loop until a unique file name is found
    while file_name in os.listdir(directory):
        file_name = f"{base_name}_{counter}.{extension}"
        counter += 1

    return os.path.join(directory, file_name)

if __name__ == "__main__":
    # Open serial port
    ser = serial.Serial(PORT, BAUDRATE)

    # Data acquisition setup
    print("What word will be recorded?")
    _, chosen_word = choose_from_available(selected_labels_dict)
    # print()
    # print("At what sampling frequency the word will be recorded?")
    # _, chosen_sampling = choose_from_available(sampling_str7ings_dict)

    chosen_sampling = "8kHz"

    OUTPUT_FILE_BASE = f"{chosen_word}_fs={chosen_sampling}"
    OUTPUT_FILE = get_unique_filename(f"data/audio/{chosen_sampling}_sampling/", OUTPUT_FILE_BASE, "csv")



    if chosen_sampling == "16kHz":
        break_value = 15999
    else:
        break_value = 7999

    # Open file for saving received data
    with open(OUTPUT_FILE, "w") as f:
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