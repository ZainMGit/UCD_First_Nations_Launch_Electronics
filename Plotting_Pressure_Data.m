% Use this script  to plot pressure data after running
% "AllSensors_savingdata.ino"

%Select the file graphBMP.txt when prompted
[file, path] = uigetfile('*.txt', 'Select a .txt file');
if isequal(file, 0)
    disp('User canceled file selection.');
    return;
end

% Reads the file
filename = fullfile(path, file);
data = readmatrix(filename);

% Check if the file has two columns
if size(data, 2) ~= 2
    error('Selected file must contain exactly two columns: time and pressure.');
end

% Extracts time and pressure columns from the text file
time = data(:, 1);
pressure = data(:, 2);

% Plots the data
figure;
plot(time, pressure, 'b-', 'LineWidth', 1.5);
grid on;
xlabel('Time (seconds)');
ylabel('Pressure (Pa)');
title('Time vs Pressure');
