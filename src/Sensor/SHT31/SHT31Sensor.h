#ifndef SHT31_SENSOR_H
#define SHT31_SENSOR_H

#include <Arduino.h>
#include <Wire.h>

extern float lastTemperature; // Variabel global untuk menyimpan suhu terakhir
extern float lastHumidity;    // Variabel global untuk menyimpan kelembapan terakhir

// Fungsi untuk inisialisasi sensor SHT31
bool initSHT31();

// Fungsi untuk membaca data dari sensor SHT31
bool readSHT31(float* temperature, float* humidity);

#endif
