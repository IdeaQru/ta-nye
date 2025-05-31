#include "Battery.h"

float lastVoltage = 0.0;
int analogPin = -1;
float R1 = 0.0;
float R2 = 0.0;
float vRef = 5.0;
int resolution = 1023;

void initBattery(int pin, float r1, float r2, float vref, int res) {
  analogPin = pin;
  R1 = r1;
  R2 = r2;
  vRef = vref;
  resolution = res;
  pinMode(analogPin, INPUT);
}

float readBatteryVoltage() {
  int adcValue = analogRead(analogPin);
  float vOut = (adcValue / float(resolution)) * vRef;
  float voltage = vOut * ((R1 + R2) / R2);
  voltage = constrain(voltage, 0, 20);
  lastVoltage = voltage;
  return voltage;
}

float getBatteryPercent(float voltage) {
  // Logika sederhana untuk menghitung persentase baterai
  // Asumsi baterai 12V, 100% di 12.6V, 0% di 10.5V
  float maxVoltage = 12.6;
  float minVoltage = 10.5;
  float percent = ((voltage - minVoltage) / (maxVoltage - minVoltage)) * 100.0;
  return constrain(percent, 0, 100);
}
