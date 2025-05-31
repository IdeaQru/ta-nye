#ifndef BATTERY_SENSOR_H
#define BATTERY_SENSOR_H

#include <Arduino.h>

extern float lastVoltage; // Variabel global untuk menyimpan tegangan terakhir

// Fungsi untuk inisialisasi sensor baterai
void initBattery(int pin, float r1, float r2, float vref, int res);

// Fungsi untuk membaca tegangan baterai
float readBatteryVoltage();

// Fungsi untuk menghitung persentase baterai
float getBatteryPercent(float voltage);

#endif
