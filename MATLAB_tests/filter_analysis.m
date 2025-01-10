%% Filter analysis
clear all;
data = load("FIR_LP_fcutoff=8kHz_real.mat");
Num = data.Num;

fft_N = 256;
Fs = 48e3;

% Frequency response
[H, W] = freqz(Num, 1, fft_N, Fs);
figure;
subplot(2, 1, 1);
plot(W, 20*log10(abs(H)));
title("Frequency Response (Magnitude)");
xlabel("Frequency [Hz]");
ylabel("Magnitude [dB]");
grid on;

% Phase response
subplot(2, 1, 2);
plot(W, angle(H));
title("Phase Response");
xlabel("Frequency [Hz]");
ylabel("Phase [radians]");
grid on;

% Group delay
[gd, W] = grpdelay(Num, 1, fft_N, Fs);
figure;
plot(W, gd);
title("Group Delay");
xlabel("Frequency [Hz]");
ylabel("Samples");
grid on;

% Pole-zero plot
figure;
zplane(Num, 1);
title("Pole-Zero Plot");
grid on;
