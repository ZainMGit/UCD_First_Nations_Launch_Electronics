Components needed for payload system
- Arduino Uno R4
- Micro SD card reader & microSD card
- 9V battery
- BMP 280 (pressure sensor)
- SparkFun 9DoF IMU Breakout (ISM 330 DHCX & MMC5983MA)	(position sensor)

The I2C addresses of the sensors do not conflict so both sensors can be connected to the same I2C bus without issues.


If using the Qwiic connectors the blue wire is for SDA and yellow for SCL
On an Arduino Uno the I2C pins are: A4(SDA) and A5(SCL)
Reccomended to use screw terminal blocks to secure wires during launch.


The BMP 280 sensor is recording temperature (°C) and pressure (Pa). Based on the pressure values from the sensor altitude (m) is being calculated. The ISM330DHCX sensor is recording acceleration (mg) and gyroscope angular velocity (mdps). Data from both sensors are saved to the SD card in separate .txt files every two seconds.







