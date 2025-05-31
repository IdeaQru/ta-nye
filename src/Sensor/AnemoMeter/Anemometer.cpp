#include "Anemometer.h"

float lastWindSpeed = 0.0;
volatile int rpmcount = 0;
int GPIO_pulse = -1;

void initAnemometer(int pin) {
  GPIO_pulse = pin;
  pinMode(GPIO_pulse, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(GPIO_pulse), rpm_anemometer, RISING);
}

float readWindSpeed() {
  detachInterrupt(digitalPinToInterrupt(GPIO_pulse));
  float rotasi_per_detik = float(rpmcount) / 2.0; // Asumsi interval 2 detik
  float kecepatan_meter_per_detik = (-0.0181 * rotasi_per_detik * rotasi_per_detik) +
                                    (1.3859 * rotasi_per_detik) + 1.4055;
  if (kecepatan_meter_per_detik <= 1.5) kecepatan_meter_per_detik = 0.0;
  lastWindSpeed = kecepatan_meter_per_detik;
  rpmcount = 0;
  attachInterrupt(digitalPinToInterrupt(GPIO_pulse), rpm_anemometer, RISING);
  return kecepatan_meter_per_detik;
}

void rpm_anemometer() {
  rpmcount++;
}
