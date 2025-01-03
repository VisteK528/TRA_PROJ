clear all;

f = 1; % Frequency in Hz
Fs = 48e3; % Sampling frequency in Hz (must be at least 2*f for Nyquist)
T = 1; % Duration of the signal in seconds
A = 1; % Amplitude of the sinusoid

% Time vector
t = 0:1/Fs:T-1/Fs; % Time from 0 to T with step size 1/Fs
f = 1;
f0 = 1;
f1 = 30e3;

% Sinusoidal signal
signal = A * sin(2 * pi * f * t);

signal = chirp(t, f0, T, f1);

figure;
plot(signal);

data = load("FIR_LP_fcutoff=8kHz.mat");
Num = data.Num;

filtered_signal = filter(Num, 1, signal);

figure;
plot(filtered_signal);


data2 = load("impulse_response_coefficients2.mat");
impulse_response = data2.impulse_response_coefficients;

y_manual_filtered = zeros(size(filtered_signal));
for k=1:length(signal)
    if k < 59
        q = [signal(1:k)'; zeros(58-k, 1)];
    else
        q = signal(k-58+1:k)';
    end

    y_manual_filtered(k) = impulse_response' * q;
end

figure;
plot(y_manual_filtered);

