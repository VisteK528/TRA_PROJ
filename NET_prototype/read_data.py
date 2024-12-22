import serial

# Configuration
PORT = '/dev/ttyACM0'  # Replace with your port name
BAUDRATE = 115200  # Must match STM32 configuration
OUTPUT_FILE = "audio_data.csv"

# Open serial port
ser = serial.Serial(PORT, BAUDRATE)


# Open file for saving received data
with open(OUTPUT_FILE, "w") as f:
    try:
        while True:
            # Read a chunk of data
            chunk = ser.read_until(b'\r\n')
            chunk = chunk.decode('utf-8')
            print(chunk, end='')
            #number, data = chunk.split(',')
            #
            #
            f.writelines(chunk)
            #
            # if number == 63999:
            #     break
    except KeyboardInterrupt:
        print("Reception stopped.")
    finally:
        ser.close()
