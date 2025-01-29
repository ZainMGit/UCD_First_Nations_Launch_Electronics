#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <SD.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)
#define SD_CS    (4) // Chip select pin for the SD card

Adafruit_BMP280 bmp; // I2C
File dataFile;        // File object for writing to the SD card

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(100); // Wait for Serial Monitor to connect (for native USB)

  Serial.println(F("Initializing BMP280 and SD card..."));

  // Initialize BMP280 sensor
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1) delay(10);
  }

  // Set BMP280 sampling
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  // Initialize SD card
  if (!SD.begin(SD_CS)) {
    Serial.println(F("SD card initialization failed!"));
    while (1) delay(10);
  }
  Serial.println(F("SD card initialized successfully."));
}

void loop() {
  // Read BMP280 sensor data
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude(1021); // 1013.25 is defaul value, 
//currently using pressure reading from Davis

  // Print data to Serial Monitor
  Serial.print(F("Temperature = "));
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(pressure);
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(altitude);
  Serial.println(" m");

  Serial.println();

  // Open the file for appending data
  dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile) {
    // Write sensor data to the file
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
    dataFile.close(); // Close the file
    Serial.println(F("Data written to SD card."));
  } else {
    Serial.println(F("Error opening data.txt for writing."));
  }

  delay(2000); // Delay before the next reading
}
