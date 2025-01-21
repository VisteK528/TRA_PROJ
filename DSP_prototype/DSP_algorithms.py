import numpy as np
from scipy.signal import stft as scipy_stft
from scipy.signal import istft


def gaussian_window(size, sigma=0.1):
    """
    Create a Gaussian window.

    Parameters:
        size (int): Length of the window.
        sigma (float): Standard deviation factor, controls the width of the Gaussian.

    Returns:
        np.array: Gaussian window.
    """
    n = np.arange(size)
    center = (size - 1) / 2
    return np.exp(-0.5 * ((n - center) / (sigma * center)) ** 2)

def stft(signal, fs: int, N: int, hop_size: int, window_type='hann', sigma=0.1, padding=False, verbose=False):
    """
    Perform Short-Time Fourier Transform (STFT) on a signal.

    Parameters:
        signal (np.array): Input signal.
        fs (int): Sampling frequency.
        N (int): Size of the window (in samples).
        hop_size (int): Hop size (in samples).
        window_type (str): Type of window function ('hann', 'hamming', 'gaussian').
        sigma (float): Standard deviation factor, controls the width of the Gaussian.
        padding (bool): If True, add symmetrical padding to the signal before computing the STFT (like in Librosa library).
        verbose (bool): If True, print out the STFT information.

    Returns:
        np.array: STFT matrix (complex values).
        np.array: Frequency bins.
        np.array: Time frames.
    """

    # Create window
    if window_type == 'hann':
        window = np.hanning(N)
    elif window_type == 'hamming':
        window = np.hamming(N)
    elif window_type == 'gaussian':
        window = gaussian_window(N, sigma)
    else:
        raise ValueError(f"Unsupported window type: {window_type}")

    if padding:
        # Symmetric padding
        num_frames = int(np.ceil(len(signal) / hop_size))
        total_padding = (num_frames - 1) * hop_size + N - len(signal)
        left_padding = total_padding // 2
        right_padding = total_padding - left_padding
        signal = np.pad(signal, (left_padding, right_padding), mode='constant')

    num_frames = (len(signal) - N) // hop_size + 1

    if verbose:
        print(f"Number of frames: {num_frames}")
        if padding:
            print(f"left_padding: {left_padding}")
            print(f"right_padding: {right_padding}")
        print(f"signal_shape: {signal.shape}")

    # Preallocate STFT matrix
    stft_matrix = np.zeros((N // 2 + 1, num_frames), dtype=np.complex128)

    for i in range(num_frames):
        start_idx = i * hop_size
        end_idx = start_idx + N
        frame = signal[start_idx:end_idx] * window

        # Compute FFT and take the positive frequencies
        stft_matrix[:, i] = np.fft.rfft(frame)

    # Frequency and time vectors
    freq_bins = np.fft.rfftfreq(N, d=1/fs)
    time_frames = np.arange(num_frames) * hop_size / fs

    return stft_matrix, freq_bins, time_frames


def new_stft(signal, fs: int, N: int, hop_size: int, window_type='hann', sigma=0.1, padding=False, verbose=False):
    """
    Perform Short-Time Fourier Transform (STFT) on a signal.

    Parameters:
        signal (np.array): Input signal.
        fs (int): Sampling frequency.
        N (int): Size of the window (in samples).
        hop_size (int): Hop size (in samples).
        window_type (str): Type of window function ('hann', 'hamming', 'gaussian').
        sigma (float): Standard deviation factor, controls the width of the Gaussian.
        padding (bool): If True, add symmetrical padding to the signal before computing the STFT (like in Librosa library).
        verbose (bool): If True, print out the STFT information.

    Returns:
        np.array: STFT matrix (complex values).
        np.array: Frequency bins.
        np.array: Time frames.
    """

    # Create window
    if window_type == 'hann':
        window = np.hanning(N)
    elif window_type == 'hamming':
        window = np.hamming(N)
    elif window_type == 'gaussian':
        window = gaussian_window(N, sigma)
    else:
        raise ValueError(f"Unsupported window type: {window_type}")

    if padding:
        # Symmetric padding
        num_frames = int(np.ceil(len(signal) / hop_size))
        total_padding = (num_frames - 1) * hop_size + N - len(signal)
        left_padding = total_padding // 2
        right_padding = total_padding - left_padding
        signal = np.pad(signal, (left_padding, right_padding), mode='constant')

    num_frames = (len(signal) - N) // hop_size + 1

    if verbose:
        print(f"Number of frames: {num_frames}")
        if padding:
            print(f"left_padding: {left_padding}")
            print(f"right_padding: {right_padding}")
        print(f"signal_shape: {signal.shape}")

    # Preallocate STFT matrix
    stft_matrix = np.zeros((N // 2 + 1, num_frames), dtype=np.float32)

    for i in range(num_frames):
        start_idx = i * hop_size
        end_idx = start_idx + N
        frame = signal[start_idx:end_idx] * window

        # Compute FFT and take the positive frequencies
        stft_matrix[:, i] = np.fft.rfft(frame)

    # Frequency and time vectors
    freq_bins = np.fft.rfftfreq(N, d=1/fs)
    time_frames = np.arange(num_frames) * hop_size / fs

    return stft_matrix, freq_bins, time_frames


def get_highpass_stft_filter_mask(mask_length, fs, f_pass, f_zero, minimum_transmittance = 1e-9, window_type='hann'):
    """
    Calculate a high-pass filter mask using to multiply with an FFT vectors.

    Parameters:
        mask_length (int): Length of the mask.
        fs (int): Sampling frequency in Hz.
        f_pass (float): Frequency where pass band is started.
        f_zero (float): Frequency below which all bins take on a minimum_transmittance value.
        minimum_transmittance (float): Value of filter transmittance for frequencies below f_zero (linear coefficient).
        window_type (str): Type of window ('hann', 'hamming', 'blackman') for transition.

    Returns:
        mask (np.array): High-pass filter mask.
        zero_index (int): The first index where spectrum vector is multiplied by window.
        pass_index (int): The first index where spectrum vector is multiplied by 1.
    """

    # Validate inputs
    if f_zero >= f_pass:
        raise ValueError("f_zero must be less than f_pass.")
    if f_pass > fs / 2:
        raise ValueError("f_pass must be less than or equal to the Nyquist frequency.")

    # Compute frequency vector for the STFT matrix
    freq_bins = np.fft.rfftfreq((mask_length - 1) * 2, d=1 / fs)

    # Create the filter mask
    mask = np.ones(mask_length)
    zero_index = len(np.where(freq_bins < f_zero)[0])
    pass_index = len(np.where(freq_bins < f_pass)[0])

    # Zero out frequencies below f_zero
    mask[:zero_index] = minimum_transmittance

    # Apply the transition window
    spectrum_edge_bins_number = pass_index - zero_index
    if window_type == 'hann':
        window = np.hanning(spectrum_edge_bins_number * 2)[0:spectrum_edge_bins_number]
    elif window_type == 'hamming':
        window = np.hamming(spectrum_edge_bins_number * 2)[0:spectrum_edge_bins_number]
    elif window_type == 'blackman':
        window = np.blackman(spectrum_edge_bins_number * 2)[0:spectrum_edge_bins_number]
    else:
        raise ValueError(f"Unsupported window type: {window_type}")

    window = np.maximum(window, minimum_transmittance)
    mask[zero_index:pass_index] = window
    return mask, zero_index, pass_index


def highpass_stft_filter(stft_matrix, filter_mask):
    """
    Apply a high-pass filter to an STFT matrix.

    Parameters:
        stft_matrix (np.array): STFT matrix.
        filter_mask (np.array): Filter mask.

    Returns:
        filtered_stft_matrix (np.array): Filtered STFT matrix.
    """

    # Apply the mask to the STFT matrix
    filtered_stft_matrix = stft_matrix * filter_mask[:, np.newaxis]

    return filtered_stft_matrix


def noise_reduction_gain_function(noisy_signal, fs, frame_size=1024, overlap=512,
                                  noise_frames=10):
    """
    Perform noise reduction using the gain function method.

    Parameters:
        noisy_signal (numpy array): The noisy input signal.
        fs (int): Sampling frequency of the signal.
        frame_size (int): Size of each STFT frame (default=1024).
        overlap (int): Overlap between frames (default=512).
        noise_frames (int): Number of initial frames to estimate noise power (default=10).

    Returns:
        clean_signal (numpy array): The denoised signal.
    """
    signal_length = len(noisy_signal)
    
    # STFT of the noisy signal
    f, t, Zxx = scipy_stft(noisy_signal, fs, nperseg=frame_size, noverlap=overlap)

    # Magnitude and phase of the noisy signal
    magnitude = np.abs(Zxx)
    phase = np.angle(Zxx)


    # Estimate noise power spectrum from the first few frames
    noise_power = np.mean(magnitude[:, :noise_frames] ** 2, axis=1, keepdims=True)

    # Compute SNR and gain function
    noisy_power = magnitude ** 2
    snr = np.maximum(noisy_power / (noise_power + 1e-6) - 1, 0.1)  # SNR must be non-negative
    gain = snr / (1 + snr)  # Wiener filter-based gain function


    # Apply the gain function to the magnitude spectrum
    cleaned_magnitude = gain * magnitude

    # Reconstruct the cleaned signal
    cleaned_spectrogram = cleaned_magnitude * np.exp(1j * phase)
    _, clean_signal = istft(cleaned_spectrogram, fs, nperseg=frame_size,
                            noverlap=overlap)

    return clean_signal[:signal_length]

def downsample_image_fixed(input_image, input_height, input_width, target_height, target_width):
    """
    Downsamples an image using bilinear interpolation, matching TensorFlow's tf.image.resize.
    """
    # Calculate scaling factors
    scale_height = input_height / target_height
    scale_width = input_width / target_width

    # Prepare the output image
    output_image = np.zeros((target_height, target_width), dtype=np.float32)

    for y in range(target_height):
        for x in range(target_width):
            # Map target pixel to source coordinates
            src_y = (y + 0.5) * scale_height - 0.5
            src_x = (x + 0.5) * scale_width - 0.5

            # Calculate integer bounding box of the source coordinates
            y0 = int(np.floor(src_y))
            x0 = int(np.floor(src_x))
            y1 = min(y0 + 1, input_height - 1)
            x1 = min(x0 + 1, input_width - 1)

            # Handle edge cases for the coordinates
            y0 = max(0, y0)
            x0 = max(0, x0)

            # Compute interpolation weights
            dy = src_y - y0
            dx = src_x - x0

            # Bilinear interpolation
            v00 = input_image[y0 * input_width + x0]
            v01 = input_image[y0 * input_width + x1]
            v10 = input_image[y1 * input_width + x0]
            v11 = input_image[y1 * input_width + x1]

            # Avoid negative weights
            value = (
                v00 * (1 - dx) * (1 - dy) +
                v01 * dx * (1 - dy) +
                v10 * (1 - dx) * dy +
                v11 * dx * dy
            )

            # Ensure no negative values (important if inputs are strictly non-negative)
            value = max(0, value)

            # Assign to the output image
            output_image[y, x] = value

    return output_image
