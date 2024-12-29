clear all;

data = load("FIR_LP_fcutoff=4kHz.mat");
Num = data.Num;

N = 100;
kstart = 10;
order = length(Num);
impulse = zeros(N, 1);
impulse(kstart) = 1;

impulse_response = filter(Num, 1, impulse);

figure;
stairs(impulse_response);

impulse_response_coefficients = impulse_response(kstart:kstart+order-1);

%% Export C array

% Export as a C array
fileID = fopen('fir_impulse_response.txt', 'w');
fprintf(fileID, 'float fir_impulse_response[] = {');

for i = 1:length(impulse_response_coefficients)
    fprintf(fileID, '%g', impulse_response_coefficients(i)); % Print each value
    if i < length(impulse_response_coefficients)
        fprintf(fileID, ', '); % Add a comma between values
    end
end

fprintf(fileID, '};\n');
fclose(fileID);