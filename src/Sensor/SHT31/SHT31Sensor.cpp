#include "SHT31Sensor.h"

// Deklarasi variabel global untuk menyimpan suhu dan kelembapan terakhir
float lastTemperature = 0.0;
float lastHumidity = 0.0;

// Alamat I2C default untuk SHT31 (0x44)
#define SHT31_ADDRESS 0x44

// Perintah untuk SHT31
#define SHT31_MEAS_HIGHREP 0x2400 // Perintah untuk pengukuran dengan repetisi tinggi

// Fungsi untuk inisialisasi sensor SHT31
bool initSHT31() {
  Wire.begin(); // Inisialisasi I2C
  Wire.beginTransmission(SHT31_ADDRESS);
  if (Wire.endTransmission() != 0) {
    Serial.println("Could not find a valid SHT31 sensor, check wiring!");
    return false;
  }
  return true;
}

// Fungsi untuk membaca data dari sensor SHT31
bool readSHT31(float* temperature, float* humidity) {
  // Kirim perintah pengukuran ke sensor
  Wire.beginTransmission(SHT31_ADDRESS);
  Wire.write(SHT31_MEAS_HIGHREP >> 8); // Byte tinggi dari perintah
  Wire.write(SHT31_MEAS_HIGHREP & 0xFF); // Byte rendah dari perintah
  if (Wire.endTransmission() != 0) {
    Serial.println("Failed to send measurement command to SHT31!");
    return false;
  }

  // Tunggu hingga pengukuran selesai (sekitar 15ms untuk repetisi tinggi)
  delay(15);

  // Baca data dari sensor (6 byte: 2 untuk suhu, 1 CRC suhu, 2 untuk kelembapan, 1 CRC kelembapan)
  Wire.requestFrom(SHT31_ADDRESS, 6);
  if (Wire.available() != 6) {
    Serial.println("Failed to read data from SHT31 sensor!");
    return false;
  }

  // Baca data suhu
  uint16_t rawTemp = (Wire.read() << 8) | Wire.read();
  Wire.read(); // Abaikan byte CRC untuk suhu (tidak diperiksa di sini untuk kesederhanaan)

  // Baca data kelembapan
  uint16_t rawHum = (Wire.read() << 8) | Wire.read();
  Wire.read(); // Abaikan byte CRC untuk kelembapan

  // Konversi data mentah ke nilai suhu dan kelembapan
  *temperature = -45.0 + (175.0 * rawTemp / 65535.0); // Rumus dari datasheet SHT31
  *humidity = 100.0 * rawHum / 65535.0; // Rumus dari datasheet SHT31

  // Simpan nilai terakhir ke variabel global
  lastTemperature = *temperature;
  lastHumidity = *humidity;

  return true;
}
