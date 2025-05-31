#include <Arduino.h>
#include <AISMessages.h>
#include "Sensor/SHT31/SHT31Sensor.h"
#include "Sensor/AnemoMeter/Anemometer.h"
#include "Sensor/Battery/Battery.h"
#include "Sensor/GPS/GPS.h"

// Interval untuk pembacaan sensor (dalam milidetik)
unsigned long intervalSHT31 = 5000;      // Baca SHT31 setiap 5 detik
unsigned long intervalBattery = 10000;   // Baca baterai setiap 10 detik
unsigned long intervalAnemometer = 2000; // Baca anemometer setiap 2 detik
unsigned long intervalSendAIS = 30000;   // Kirim pesan AIS setiap 30 detik

// Variabel untuk menyimpan waktu terakhir pembacaan
unsigned long prevSHT31 = 0;
unsigned long prevBattery = 0;
unsigned long prevAnemometer = 0;
unsigned long prevSendAIS = 0;

// Pin untuk sensor
#define ANEMOMETER_PIN 2  // Pin untuk anemometer
#define BATTERY_PIN A0    // Pin analog untuk sensor baterai

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600); // Serial untuk GPS (sesuaikan pin TX/RX dengan board Anda)
  delay(1000);
  Serial.println("encodeAIS Library Test with Sensors");
  Serial.println("===================================");
  Serial.println();

  // Inisialisasi sensor
  if (!initSHT31()) {
    Serial.println("Gagal menginisialisasi sensor SHT31!");
  } else {
    Serial.println("Sensor SHT31 berhasil diinisialisasi.");
  }

  initAnemometer(ANEMOMETER_PIN);
  Serial.println("Sensor Anemometer berhasil diinisialisasi.");

  initBattery(BATTERY_PIN, 30000.0, 10000.0, 5.0, 1023); // R1=30k, R2=10k, Vref=5V, Res=1023
  Serial.println("Sensor Baterai berhasil diinisialisasi.");

  initGPS(&Serial2);
  Serial.println("Sensor GPS berhasil diinisialisasi.");
}

void loop() {
  unsigned long now = millis();

  // Baca sensor SHT31 (suhu dan kelembapan)
  if (now - prevSHT31 >= intervalSHT31) {
    float temperature, humidity;
    if (!readSHT31(&temperature, &humidity)) {
      humidity += 0.0; // Offset jika diperlukan
      lastTemperature = temperature + random(400) / 1000.0;
      lastHumidity = humidity + random(400) / 1000.0;
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C\tHumidity: ");
      Serial.print(humidity);
      Serial.println(" %");
    } else {
      Serial.println("Gagal membaca sensor SHT31!");
    }
    prevSHT31 = now;
  }

  // Baca sensor baterai (tegangan)
  if (now - prevBattery >= intervalBattery) {
    float voltage = readBatteryVoltage();
    float percent = getBatteryPercent(voltage);
    Serial.print("Tegangan Baterai: ");
    Serial.print(voltage, 2);
    Serial.print(" V | Persentase: ");
    Serial.print(percent, 0);
    Serial.println(" %");
    prevBattery = now;
  }

  // Baca sensor anemometer (kecepatan angin)
  if (now - prevAnemometer >= intervalAnemometer) {
    float windSpeed = readWindSpeed();
    Serial.print("Kecepatan Angin: ");
    Serial.print(windSpeed);
    Serial.println(" m/s");
    prevAnemometer = now;
  }

  // Baca sensor GPS (koordinat)
  readGPS();

  // Encode dan kirim pesan AIS tipe 8 dan 21
  if (now > 0) {
    // Test Message 8 (Binary Broadcast Message dengan data sensor)
    Serial.println("Testing AIS Message 8 (Binary Broadcast Message):");
    AISMessage8 msg8;
    msg8.repeat_indicator = 0;
    msg8.mmsi = 123456789;
    msg8.spare = 0;
    msg8.dac = 1; // International application
    msg8.fi = 30; // Example functional ID (meteorological data)
    msg8.temperature = lastTemperature; // Dari sensor SHT31
    msg8.humidity = (uint8_t)lastHumidity; // Dari sensor SHT31
    msg8.wind_speed = lastWindSpeed; // Dari anemometer
    msg8.voltage = lastVoltage; // Dari sensor baterai
    msg8.latitude = lastLatitude; // Dari GPS
    msg8.longitude = lastLongitude; // Dari GPS
    msg8.minutes = 900; // 15:00 UTC (contoh, bisa disesuaikan)
    String nmea8 = encodeMessage8(msg8);
    Serial.println("Message 8 NMEA: " + nmea8);
    Serial.println();

    // Test Message 21 (Aids-to-Navigation Report dengan data GPS)
    Serial.println("Testing AIS Message 21 (Aids-to-Navigation Report):");
    AISMessage21 msg21;
    msg21.repeat_indicator = 0;
    msg21.mmsi = 123456789;
    msg21.aid_type = 1; // Reference point
    msg21.name = "LIGHTHOUSE ALPHA";
    msg21.position_accuracy = true;
    msg21.longitude = lastLongitude; // Dari GPS
    msg21.latitude = lastLatitude; // Dari GPS
    msg21.dimension_to_bow = 10;
    msg21.dimension_to_stern = 5;
    msg21.dimension_to_port = 3;
    msg21.dimension_to_starboard = 3;
    msg21.epfd_type = 1; // GPS
    msg21.utc_second = 30;
    msg21.off_position = false;
    msg21.regional = 0;
    msg21.raim_flag = false;
    msg21.virtual_aton_flag = false;
    msg21.assigned_mode_flag = false;
    msg21.spare = 0;
    String nmea21 = encodeMessage21(msg21);
    Serial.println("Message 21 NMEA: " + nmea21);
    Serial.println();

    prevSendAIS = now;
  }
}
