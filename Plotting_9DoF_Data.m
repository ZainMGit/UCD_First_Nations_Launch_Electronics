% Prompt user to select the sensor log file
[filename, pathname] = uigetfile('*.TXT', 'Select the Sensor Log File');
if isequal(filename,0)
    disp('User canceled file selection.');
    return;
end

% Read file line-by-line
lines = readlines(fullfile(pathname, filename));

% Initialize data arrays
Acc_X = []; Acc_Y = []; Acc_Z = [];
Gyro_X = []; Gyro_Y = []; Gyro_Z = [];
Mag_X = []; Mag_Y = []; Mag_Z = [];

% Regular expression pattern to extract all values
pattern = 'Acc\[mg\]: (-?\d+), (-?\d+), (-?\d+) \| Gyro\[mdps\]: (-?\d+), (-?\d+), (-?\d+) \| Mag\[uT\]: ([\d.]+), ([\d.]+), ([\d.]+)';

for i = 1:length(lines)
    tokens = regexp(lines(i), pattern, 'tokens');
    if ~isempty(tokens)
        nums = str2double(tokens{1});
        Acc_X(end+1) = nums(1);
        Acc_Y(end+1) = nums(2);
        Acc_Z(end+1) = nums(3);
        Gyro_X(end+1) = nums(4);
        Gyro_Y(end+1) = nums(5);
        Gyro_Z(end+1) = nums(6);
        Mag_X(end+1) = nums(7);
        Mag_Y(end+1) = nums(8);
        Mag_Z(end+1) = nums(9);
    end
end

samples = 1:length(Acc_X);

% --- Accelerometer Tab ---
figure('Name', 'Accelerometer Data');
plot(samples, Acc_X, 'r', samples, Acc_Y, 'g', samples, Acc_Z, 'b');
title('Accelerometer Data (mg)');
xlabel('Sample Index'); ylabel('Acceleration (mg)');
legend({'X', 'Y', 'Z'});
grid on;

% --- Gyroscope Tab ---
figure('Name', 'Gyroscope Data');
plot(samples, Gyro_X, 'r', samples, Gyro_Y, 'g', samples, Gyro_Z, 'b');
title('Gyroscope Data (mdps)');
xlabel('Sample Index'); ylabel('Angular Velocity (mdps)');
legend({'X', 'Y', 'Z'});
grid on;

% --- Magnetometer Tab ---
figure('Name', 'Magnetometer Data');
plot(samples, Mag_X, 'r', samples, Mag_Y, 'g', samples, Mag_Z, 'b');
title('Magnetometer Data (\muT)');
xlabel('Sample Index'); ylabel('Magnetic Field (\muT)');
legend({'X', 'Y', 'Z'});
grid on;
