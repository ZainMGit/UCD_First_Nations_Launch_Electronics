% Use this script to plot pressure and altitude (in feet) after running
% "AllSensors_savingdata.ino"

% Select the file graphBMP.txt when prompted
[file, path] = uigetfile('*.txt', 'Select a .txt file');
if isequal(file, 0)
    disp('User canceled file selection.');
    return;
end

% Read the file
filename = fullfile(path, file);
data = readmatrix(filename);

% Check if the file has two columns
if size(data, 2) ~= 2
    error('Selected file must contain exactly two columns: time and pressure.');
end

% Extract time and pressure
time = data(:, 1);
pressure = data(:, 2);  % Pressure in Pascals

% Convert pressure to altitude in meters using barometric formula
P0 = 101325;  % Sea level standard atmospheric pressure (Pa)
altitude_m = 44330 * (1 - (pressure / P0).^(1/5.255));

% Convert altitude to feet
altitude_ft = altitude_m * 3.28084;

% Plot Pressure vs Time
figure;
plot(time, pressure, 'b-', 'LineWidth', 1.5);
grid on;
xlabel('Time (seconds)');
ylabel('Pressure (Pa)');
title('Time vs Pressure');

% Plot Altitude vs Time (in feet)
figure;
plot(time, altitude_ft, 'r-', 'LineWidth', 1.5);
grid on;
xlabel('Time (seconds)');
ylabel('Altitude (feet)');
title('Time vs Altitude (ft)');
