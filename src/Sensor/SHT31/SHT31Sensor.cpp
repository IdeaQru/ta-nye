#include "SHT31Sensor.h"

float lastTemperature = 0.0;
float lastHumidity = 0.0;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

bool initSHT31() {
  if (!sht31.begin(0x44)) { // Alamat I2C default untuk SHT31
    return false;
  }
  return true;
}

bool readSHT31(float* temperature, float* humidity) {
  *temperature = sht31.readTemperature();
  *humidity = sht31.readHumidity();
  if (isnan(*temperature) || isnan(*humidity)) {
    return false;
  }
  return true;
}
