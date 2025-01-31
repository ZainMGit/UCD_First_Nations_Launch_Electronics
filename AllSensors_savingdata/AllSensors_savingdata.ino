#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <SD.h>
#include <ISM330DHCXSensor.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)
#define SD_CS    (4) // Chip select pin for the SD card

// BMP280 Sensor
Adafruit_BMP280 bmp; // I2C

// ISM330DHCX Sensor
#define dev_interface Wire
ISM330DHCXSensor AccGyr(&dev_interface);

// SD Card File Objects
File dataFile;
File dataFile2;
File dataFile3;
unsigned long startTime;
void setup() {
  Serial.begin(9600);
  while (!Serial) delay(100); // Wait for Serial Monitor to connect (for native USB)

  startTime = millis();

  Serial.println(F("Initializing BMP280, ISM330DHCX, and SD card..."));

  // Initialize BMP280 sensor
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1) delay(10);
  }

  // Set BMP280 sampling
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     
                  Adafruit_BMP280::SAMPLING_X2,     
                  Adafruit_BMP280::SAMPLING_X16,    
                  Adafruit_BMP280::FILTER_X16,      
                  Adafruit_BMP280::STANDBY_MS_500);

  // Initialize ISM330DHCX Sensor
  dev_interface.begin();
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
  // Read BMP280 sensor data
  unsigned long currentTime = millis() - startTime;
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude(1029); // Adjusted for Davis

  // Print BMP280 data to Serial Monitor
  Serial.print(F("Temperature = "));
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(pressure);
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(altitude);
  Serial.println(" m");

  //Serial.println();
  dataFile3 = SD.open("graphBMP.txt", FILE_WRITE);
  if (dataFile3) {
    dataFile3.print(currentTime / 1000.0); // Convert milliseconds to seconds
    dataFile3.print(" ");
    dataFile3.print(pressure);
    dataFile3.println();
    dataFile3.flush();
    dataFile3.close();
  } else {
    Serial.println(F("Error opening AltitudeValue.txt for writing."));
  }

  // Open BMP280 data file
  dataFile = SD.open("BMP280.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(F("Temperature = "));
    dataFile.print(temperature);
    dataFile.println(" *C");

    dataFile.print(F("Pressure = "));
    dataFile.print(pressure);
    dataFile.println(" Pa");

    dataFile.print(F("Approx altitude = "));
    dataFile.print(altitude);
    dataFile.println(" m");

    dataFile.println();
    dataFile.close();
    Serial.println(F("BMP280 data written to SD card."));
    Serial.println();
  } else {
    Serial.println(F("Error opening BMP280.txt for writing."));
    Serial.println();
  }

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

  // Open ISM330DHCX data file
  dataFile2 = SD.open("position.txt", FILE_WRITE);
  if (dataFile2) {
    dataFile2.print("Acc[mg]: ");
    dataFile2.print(accelerometer[0]);
    dataFile2.print(", ");
    dataFile2.print(accelerometer[1]);
    dataFile2.print(", ");
    dataFile2.print(accelerometer[2]);
    dataFile2.print(", Gyro[mdps]: ");
    dataFile2.print(gyroscope[0]);
    dataFile2.print(", ");
    dataFile2.print(gyroscope[1]);
    dataFile2.print(", ");
    dataFile2.println(gyroscope[2]);

    dataFile2.println();
    dataFile2.close();
    Serial.println(F("ISM330DHCX data written to SD card."));
    Serial.println();
  } else {
    Serial.println(F("Error opening position.txt for writing."));
    Serial.println();
  }

  delay(1800); // Delay before the next reading
}