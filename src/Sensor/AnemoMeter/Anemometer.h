#ifndef ANEMOMETER_SENSOR_H
#define ANEMOMETER_SENSOR_H

#include <Arduino.h>

extern float lastWindSpeed; // Variabel global untuk menyimpan kecepatan angin terakhir

// Fungsi untuk inisialisasi sensor anemometer
void initAnemometer(int pin);

// Fungsi untuk membaca kecepatan angin
float readWindSpeed();

// Fungsi interrupt untuk menghitung pulsa
void rpm_anemometer();

#endif
