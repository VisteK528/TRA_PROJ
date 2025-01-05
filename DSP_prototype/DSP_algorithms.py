import numpy as np


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
