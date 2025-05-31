#ifndef GPS_SENSOR_H
#define GPS_SENSOR_H

#include <Arduino.h>

extern float lastLatitude;  // Variabel global untuk menyimpan latitude terakhir
extern float lastLongitude; // Variabel global untuk menyimpan longitude terakhir

// Fungsi untuk inisialisasi sensor GPS
void initGPS(HardwareSerial* serial);

// Fungsi untuk membaca data GPS
void readGPS();

// Fungsi untuk memproses data NMEA
void processNMEA(String nmea);

#endif
