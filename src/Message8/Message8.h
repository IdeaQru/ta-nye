#ifndef MESSAGE_8_H
#define MESSAGE_8_H

#include <Arduino.h>
#include "utils/AISUtils.h"

struct AISMessage8 {
  uint8_t message_id = 8;     // 6 bits
  uint8_t repeat_indicator;   // 2 bits
  uint32_t mmsi;              // 30 bits
  uint8_t spare = 0;              // 2 bits
  uint16_t dac = 30;               // 10 bits (Designated Area Code)
  uint8_t fi =31;                 // 6 bits (Functional ID)
  // Contoh data untuk aplikasi khusus (misalnya meteorologi)
  float temperature;          // 10 bits (temperature * 10, signed)
  uint8_t humidity;           // 7 bits (percentage)
  float wind_speed;           // 8 bits (wind speed * 10)
  float voltage;              // 10 bits (voltage * 10)
  float latitude;             // 27 bits (signed)
  float longitude;            // 28 bits (signed)
  uint16_t minutes;           // 11 bits (minutes since midnight UTC)
};

String encodeMessage8(const AISMessage8& msg);

#endif
