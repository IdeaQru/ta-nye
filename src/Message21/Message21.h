
#ifndef MESSAGE_21_H
#define MESSAGE_21_H

#include <Arduino.h>
#include "utils/AISUtils.h"

struct AISMessage21 {
  uint8_t message_id = 21;          // 6 bits
  uint8_t repeat_indicator;         // 2 bits
  uint32_t mmsi;                    // 30 bits
  uint8_t aid_type = 0;                 // 5 bits
  String name;                      // 120 bits (20 chars)
  bool position_accuracy = true;           // 1 bit
  float longitude;                  // 28 bits
  float latitude;                   // 27 bits
  uint16_t dimension_to_bow=0;        // 9 bits
  uint16_t dimension_to_stern=0;      // 9 bits
  uint8_t dimension_to_port=0;        // 6 bits
  uint8_t dimension_to_starboard=0;   // 6 bits
  uint8_t epfd_type=0;                // 4 bits
  uint8_t utc_second=0;               // 6 bits
  bool off_position=0;                // 1 bit
  uint8_t regional=0;                 // 8 bits
  bool raim_flag=0;                   // 1 bit
  bool virtual_aton_flag=0;           // 1 bit
  bool assigned_mode_flag=0;          // 1 bit
  uint8_t spare = 0;                    // 1 bit
};

String encodeMessage21(const AISMessage21& msg);

#endif
