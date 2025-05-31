#ifndef AIS_UTILS_H
#define AIS_UTILS_H

#include <Arduino.h>

// Fungsi untuk menulis nilai ke buffer bit array mulai dari startBit
void writeBits(uint8_t* buffer, uint32_t value, uint16_t startBit, uint8_t numBits, bool isSigned = false);

// Fungsi konversi dari bit array ke string ASCII 6-bit sesuai standar AIS
void bitsToAISString(const uint8_t* bits, uint16_t bitCount, char* output);

// Fungsi menghitung checksum NMEA
uint8_t calcNMEAChecksum(const char* str);

// Fungsi untuk mengencode string ke format 6-bit AIS
void encodeStringToBits(const String& str, uint8_t* buffer, uint16_t startBit, uint8_t maxChars);

// Fungsi untuk membuat pesan NMEA
String createNMEAMessage(const char* payload);

#endif
