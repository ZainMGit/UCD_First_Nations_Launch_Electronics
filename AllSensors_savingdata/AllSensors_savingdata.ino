#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <SD.h>
#include <ISM330DHCXSensor.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h>

#define BMP_CS   (10)
#define SD_CS    (4)

Adafruit_BMP280 bmp;
#define dev_interface Wire
ISM330DHCXSensor AccGyr(&dev_interface);
SFE_MMC5983MA mag;

File dataFile, dataFile2, dataFile3;
float manualTime = 0.00;
int magZeroCount = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(100);

  Serial.println(F("Initializing sensors and SD card..."));

  if (!bmp.begin()) {
    Serial.println(F("BMP280 not found!"));
    while (1) delay(10);
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);

  dev_interface.begin();
  AccGyr.begin();
  AccGyr.ACC_Enable();
  AccGyr.GYRO_Enable();

  if (!mag.begin()) {
    Serial.println(F("MMC5983MA not found!"));
    while (1) delay(10);
  }

  if (!SD.begin(SD_CS)) {
    Serial.println(F("SD card init failed!"));
    while (1) delay(10);
  }
  Serial.println(F("Initialization done."));
}

void loop() {
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude(1013);

  if (pressure < 30000 || pressure > 110000 || temperature < -40 || temperature > 85) {
    Serial.println("Invalid BMP280 data, skipping loop.");
    return;
  }

  dataFile3 = SD.open("graphBMP.txt", FILE_WRITE);
  if (dataFile3) {
    dataFile3.print(manualTime);
    dataFile3.print(" ");
    dataFile3.print(pressure);
    dataFile3.println();
    dataFile3.flush();
    dataFile3.close();
  }

  dataFile = SD.open("BMP280.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("Temperature = "); dataFile.print(temperature); dataFile.println(" *C");
    dataFile.print("Pressure = "); dataFile.print(pressure); dataFile.println(" Pa");
    dataFile.print("Approx altitude = "); dataFile.print(altitude); dataFile.println(" m");
    dataFile.println();
    dataFile.close();
  }

  int32_t accelerometer[3];
  int32_t gyroscope[3];
  uint8_t accStatus = AccGyr.ACC_GetAxes(accelerometer);
  uint8_t gyrStatus = AccGyr.GYRO_GetAxes(gyroscope);

  if (accStatus != 0 || gyrStatus != 0) {
    Serial.println("Warning: IMU read failed. Reinitializing...");
    AccGyr.end();
    AccGyr.begin();
    AccGyr.ACC_Enable();
    AccGyr.GYRO_Enable();
    return;
  }

  float magX = mag.getMeasurementX();
  float magY = mag.getMeasurementY();
  float magZ = mag.getMeasurementZ();

  if (magX == 0.0 && magY == 0.0 && magZ == 0.0) {
    magZeroCount++;
    Serial.println("Warning: Magnetometer returned zeros.");
    if (magZeroCount > 3) {
      Serial.println("Magnetometer seems stuck. Reinitializing...");
      mag.begin();
      magZeroCount = 0;
    }
  } else {
    magZeroCount = 0;
  }

  Serial.print("Acc[mg]: "); Serial.print(accelerometer[0]); Serial.print(", ");
  Serial.print(accelerometer[1]); Serial.print(", "); Serial.print(accelerometer[2]);
  Serial.print(" | Gyro[mdps]: "); Serial.print(gyroscope[0]); Serial.print(", ");
  Serial.print(gyroscope[1]); Serial.print(", "); Serial.print(gyroscope[2]);
  Serial.print(" | Mag[uT]: "); Serial.print(magX, 3); Serial.print(", ");
  Serial.print(magY, 3); Serial.print(", "); Serial.println(magZ, 3);

  dataFile2 = SD.open("position.txt", FILE_WRITE);
  if (dataFile2) {
    dataFile2.print("Acc[mg]: "); dataFile2.print(accelerometer[0]); dataFile2.print(", ");
    dataFile2.print(accelerometer[1]); dataFile2.print(", "); dataFile2.print(accelerometer[2]);
    dataFile2.print(" | Gyro[mdps]: "); dataFile2.print(gyroscope[0]); dataFile2.print(", ");
    dataFile2.print(gyroscope[1]); dataFile2.print(", "); dataFile2.print(gyroscope[2]);
    dataFile2.print(" | Mag[uT]: "); dataFile2.print(magX, 3); dataFile2.print(", ");
    dataFile2.print(magY, 3); dataFile2.print(", "); dataFile2.println(magZ, 3);
    dataFile2.println();
    dataFile2.close();
  }

  manualTime += 2;
  delay(1800);
}
