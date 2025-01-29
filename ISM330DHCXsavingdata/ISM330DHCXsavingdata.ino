#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <ISM330DHCXSensor.h>

#define SD_CS  (4) // Chip select pin for the SD card

// ISM330DHCX Sensor
#define dev_interface Wire
ISM330DHCXSensor AccGyr(&dev_interface);

// SD Card File Object
File dataFile;

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(100); // Wait for Serial Monitor to connect (for native USB)

  Serial.println(F("Initializing ISM330DHCX sensor and SD card..."));

  // Initialize I2C interface
  dev_interface.begin();

  // Initialize ISM330DHCX Sensor
  AccGyr.begin();
  AccGyr.ACC_Enable();  
  AccGyr.GYRO_Enable();

  // Initialize SD card
  if (!SD.begin(SD_CS)) {
    Serial.println(F("SD card initialization failed!"));
    while (1) delay(10);
  }
  Serial.println(F("SD card initialized successfully."));
}

void loop() {
  // Read ISM330DHCX sensor data
  int32_t accelerometer[3];
  int32_t gyroscope[3];
  AccGyr.ACC_GetAxes(accelerometer);  
  AccGyr.GYRO_GetAxes(gyroscope);

  // Print ISM330DHCX data to Serial Monitor
  Serial.print("Acc[mg]: ");
  Serial.print(accelerometer[0]);
  Serial.print(", ");
  Serial.print(accelerometer[1]);
  Serial.print(", ");
  Serial.print(accelerometer[2]);
  Serial.print(", Gyro[mdps]: ");
  Serial.print(gyroscope[0]);
  Serial.print(", ");
  Serial.print(gyroscope[1]);
  Serial.print(", ");
  Serial.println(gyroscope[2]);

  // Open SD card file and write data
  dataFile = SD.open("data2.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("Acc[mg]: ");
    dataFile.print(accelerometer[0]);
    dataFile.print(", ");
    dataFile.print(accelerometer[1]);
    dataFile.print(", ");
    dataFile.print(accelerometer[2]);
    dataFile.print(", Gyro[mdps]: ");
    dataFile.print(gyroscope[0]);
    dataFile.print(", ");
    dataFile.print(gyroscope[1]);
    dataFile.print(", ");
    dataFile.println(gyroscope[2]);

    dataFile.println();
    dataFile.close();
    Serial.println(F("ISM330DHCX data written to SD card."));
  } else {
    Serial.println(F("Error opening data2.txt for writing."));
  }

  delay(2000); // Delay before the next reading
}