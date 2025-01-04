clear all;
run("filterImpulseResponse.m");

f = 1; % Frequency in Hz
Fs = 48e3; % Sampling frequency in Hz (must be at least 2*f for Nyquist)
T = 1; % Duration of the signal in seconds
A = 1; % Amplitude of the sinusoid
fft_N = 256;

% Time vector
t = 0:1/Fs:T-1/Fs; % Time from 0 to T with step size 1/Fs
f0 = 1;
f1 = 30e3;

% Generate chirp signal
signal = chirp(t, f0, T, f1);

% Plot original signal
figure;
plot(t, signal);
title("Original Signal");
xlabel("Time [s]");
ylabel("Amplitude");

% Compute and plot spectrum of original signal
signal_fft = fft(signal, fft_N);
frequencies = linspace(0, Fs/2, fft_N/2 + 1);

figure;
plot(frequencies, abs(signal_fft(1:fft_N/2 + 1)));
title("Spectrum of Original Signal");
xlabel("Frequency [Hz]");
ylabel("Amplitude");

% Load FIR filter coefficients
data = load("FIR_LP_fcutoff=8kHz_real.mat");
Num = data.Num;

% Apply filter
filtered_signal = filter(Num, 1, signal);

% Plot filtered signal
figure;
plot(t, filtered_signal);
title("Filtered Signal");
xlabel("Time [s]");
ylabel("Amplitude");

% Compute and plot spectrum of filtered signal
filtered_signal_fft = fft(filtered_signal, fft_N);

figure;
plot(frequencies, abs(filtered_signal_fft(1:fft_N/2 + 1)));
title("Spectrum of Filtered Signal");
xlabel("Frequency [Hz]");
ylabel("Amplitude");

% Manual filtering implementation
y_manual_filtered = zeros(size(signal));
impulse_response_length = length(impulse_response_coefficients);

for k = 1:length(signal)
    if k < impulse_response_length
        q = [signal(1:k)'; zeros(impulse_response_length-k, 1)];
    else
        q = signal(k-impulse_response_length+1:k)';
    end
    y_manual_filtered(k) = impulse_response_coefficients' * q;
end

% Plot manually filtered signal
figure;
plot(t, y_manual_filtered);
title("Manually Filtered Signal");
xlabel("Time [s]");
ylabel("Amplitude");

% Compute and plot spectrum of manually filtered signal
y_manual_filtered_fft = fft(y_manual_filtered, fft_N);

figure;
plot(frequencies, abs(y_manual_filtered_fft(1:fft_N/2 + 1)));
title("Spectrum of Manually Filtered Signal");
xlabel("Frequency [Hz]");
ylabel("Amplitude");
