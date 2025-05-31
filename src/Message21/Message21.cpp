#include "Message21.h"

String encodeMessage21(const AISMessage21& msg) {
  uint8_t bits[34] = {0}; // 272 bits = 34 bytes
  writeBits(bits, msg.message_id, 0, 6);
  writeBits(bits, msg.repeat_indicator, 6, 2);
  writeBits(bits, msg.mmsi, 8, 30);
  writeBits(bits, msg.aid_type, 38, 5);
  encodeStringToBits(msg.name, bits, 43, 20);
  writeBits(bits, msg.position_accuracy ? 1 : 0, 163, 1);
  int32_t lng_enc = (int32_t)round(msg.longitude * 600000.0);
  writeBits(bits, lng_enc, 164, 28, true);
  int32_t lat_enc = (int32_t)round(msg.latitude * 600000.0);
  writeBits(bits, lat_enc, 192, 27, true);
  writeBits(bits, msg.dimension_to_bow, 219, 9);
  writeBits(bits, msg.dimension_to_stern, 228, 9);
  writeBits(bits, msg.dimension_to_port, 237, 6);
  writeBits(bits, msg.dimension_to_starboard, 243, 6);
  writeBits(bits, msg.epfd_type, 249, 4);
  writeBits(bits, msg.utc_second, 253, 6);
  writeBits(bits, msg.off_position ? 1 : 0, 259, 1);
  writeBits(bits, msg.regional, 260, 8);
  writeBits(bits, msg.raim_flag ? 1 : 0, 268, 1);
  writeBits(bits, msg.virtual_aton_flag ? 1 : 0, 269, 1);
  writeBits(bits, msg.assigned_mode_flag ? 1 : 0, 270, 1);
  writeBits(bits, msg.spare, 271, 1);
  
  char payload[47];
  bitsToAISString(bits, 272, payload);
  return createNMEAMessage(payload);
}