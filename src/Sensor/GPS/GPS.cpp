#include "GPS.h"

float lastLatitude = 0.0;
float lastLongitude = 0.0;
HardwareSerial* gpsSerial = nullptr;
String nmeaLine = "";

void initGPS(HardwareSerial* serial) {
  gpsSerial = serial;
  gpsSerial->begin(9600); // Baud rate default untuk banyak modul GPS
}

void readGPS() {
  while (gpsSerial->available()) {
    char c = gpsSerial->read();
    if (c == '\n') {
      processNMEA(nmeaLine);
      nmeaLine = "";
    } else if (c != '\r') {
      nmeaLine += c;
    }
  }
}

void processNMEA(String nmea) {
  if (nmea.startsWith("$GPGGA") || nmea.startsWith("$GPRMC")) {
    // Parsing sederhana untuk latitude dan longitude
    // Format GGA: $GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
    // Format RMC: $GPRMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a*hh
    int commaCount = 0;
    String latStr = "";
    String lonStr = "";
    char latDir = ' ';
    char lonDir = ' ';
    for (int i = 0; i < nmea.length(); i++) {
      if (nmea[i] == ',') {
        commaCount++;
      } else if (commaCount == 2 && latStr.length() < 10) {
        latStr += nmea[i]; // Latitude
      } else if (commaCount == 3) {
        latDir = nmea[i]; // N/S
      } else if (commaCount == 4 && lonStr.length() < 11) {
        lonStr += nmea[i]; // Longitude
      } else if (commaCount == 5) {
        lonDir = nmea[i]; // E/W
      }
    }
    if (latStr.length() > 0 && lonStr.length() > 0) {
      float lat = latStr.toFloat() / 100.0; // Format ddmm.mmmm to degrees
      int deg = (int)lat;
      lat = deg + (lat - deg) * 100.0 / 60.0;
      if (latDir == 'S') lat = -lat;

      float lon = lonStr.toFloat() / 100.0; // Format dddmm.mmmm to degrees
      deg = (int)lon;
      lon = deg + (lon - deg) * 100.0 / 60.0;
      if (lonDir == 'W') lon = -lon;

      lastLatitude = lat;
      lastLongitude = lon;
    }
  }
}
