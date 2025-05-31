#include "AISUtils.h"

void writeBits(uint8_t* buffer, uint32_t value, uint16_t startBit, uint8_t numBits, bool isSigned) {
  if (isSigned && (value & (1UL << (numBits - 1)))) {
    value |= 0xFFFFFFFFUL << numBits;
  }
  for (uint8_t i = 0; i < numBits; i++) {
    uint16_t pos = startBit + i;
    uint8_t byteIdx = pos / 8;
    uint8_t bitIdx = 7 - (pos % 8);
    if (value & (1UL << (numBits - 1 - i))) {
      buffer[byteIdx] |= (1 << bitIdx);
    } else {
      buffer[byteIdx] &= ~(1 << bitIdx);
    }
  }
}

void bitsToAISString(const uint8_t* bits, uint16_t bitCount, char* output) {
  uint16_t charCount = (bitCount + 5) / 6;
  for (uint16_t i = 0; i < charCount; i++) {
    uint8_t sixBits = 0;
    for (uint8_t j = 0; j < 6; j++) {
      uint16_t bitIdx = i * 6 + j;
      if (bitIdx < bitCount) {
        sixBits = (sixBits << 1) | ((bits[bitIdx / 8] >> (7 - (bitIdx % 8))) & 1);
      } else {
        sixBits <<= 1;
      }
    }
    output[i] = (sixBits < 40) ? (sixBits + 48) : (sixBits + 56);
  }
  output[charCount] = '\0';
}

uint8_t calcNMEAChecksum(const char* str) {
  uint8_t c = 0;
  for (uint16_t i = 1; str[i] != '\0'; i++) c ^= str[i];
  return c;
}

void encodeStringToBits(const String& str, uint8_t* buffer, uint16_t startBit, uint8_t maxChars) {
  String paddedStr = str;
  paddedStr.toUpperCase();
  if (paddedStr.length() > maxChars) {
    paddedStr = paddedStr.substring(0, maxChars);
  }
  while (paddedStr.length() < maxChars) {
    paddedStr += "@";
  }
  for (uint8_t i = 0; i < maxChars; i++) {
    char c = paddedStr.charAt(i);
    uint8_t val = 0;
    if (c >= 'A' && c <= 'Z') {
      val = c - 'A' + 1;
    } else if (c >= '0' && c <= '9') {
      val = c - '0';
    } else if (c == ' ') {
      val = 32;
    } else if (c == '@') {
      val = 0;
    }
    writeBits(buffer, val, startBit + (i * 6), 6);
  }
}

String createNMEAMessage(const char* payload) {
  char nmea[150];
  sprintf(nmea, "!AIVDM,1,1,,A,%s,0", payload);
  uint8_t checksum = calcNMEAChecksum(nmea);
  char checksumStr[4];
  sprintf(checksumStr, "*%02X", checksum);
  strcat(nmea, checksumStr);
  return String(nmea);
}
