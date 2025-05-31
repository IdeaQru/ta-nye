#include "Message8.h"

String encodeMessage8(const AISMessage8& msg) {
  uint8_t bits[21] = {0}; // 168 bits = 21 bytes (untuk pesan 8 standar)
  writeBits(bits, msg.message_id, 0, 6);
  writeBits(bits, msg.repeat_indicator, 6, 2);
  writeBits(bits, msg.mmsi, 8, 30);
  writeBits(bits, msg.spare, 38, 2);
  writeBits(bits, msg.dac, 40, 10);
  writeBits(bits, msg.fi, 50, 6);
  
  // Encode data khusus (contoh: meteorologi)
  int16_t temp_scaled = (int16_t)(msg.temperature * 10);
  writeBits(bits, temp_scaled, 56, 10, true);
  writeBits(bits, msg.humidity, 66, 7);
  uint8_t wind_scaled = (uint8_t)(msg.wind_speed * 10);
  writeBits(bits, wind_scaled, 73, 8);
  uint16_t volt_scaled = (uint16_t)(msg.voltage * 10);
  writeBits(bits, volt_scaled, 81, 10);
  int32_t lat_enc = (int32_t)round(msg.latitude * 600000.0);
  writeBits(bits, lat_enc, 91, 27, true);
  int32_t lng_enc = (int32_t)round(msg.longitude * 600000.0);
  writeBits(bits, lng_enc, 118, 28, true);
  writeBits(bits, msg.minutes, 146, 11);
  
  char payload[29];
  bitsToAISString(bits, 168, payload);
  return createNMEAMessage(payload);
}
