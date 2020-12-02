#include "sensors.h"

#include <Arduino.h>

UltraSonicDistanceSensor US_left(US_LEFT_TRIGGER, US_LEFT_ECHO);
UltraSonicDistanceSensor US_right(US_RIGHT_TRIGGER, US_RIGHT_ECHO);
UltraSonicDistanceSensor US_front(US_FRONT_TRIGGER, US_FRONT_ECHO);

Adafruit_MPU6050 mpu;

double readUS(UltraSonicDistanceSensor &US) {
    double total = 0.0;
    int numReadings = 0;
    while (numReadings < US_NUM_READINGS) {
        double val = US.measureDistanceCm();
        if (300 > val && val > 0) {
            numReadings++;
            total += val;
        }
    }
    if (numReadings == 0) {
        return INF;
    } else {
        return total / numReadings;
    }
}

double readLeftUS() { return readUS(US_left); }

double readFrontUS() { return readUS(US_front); }

double readRightUS() { return readUS(US_right); }

bool readLeftLight() { 
  float v = analogRead(LIGHT_LEFT);
  Serial.print("Left: ");
  Serial.println(v);
  return v > 500; 
 }

bool readRightLight() { 
  float v = analogRead(LIGHT_RIGHT);
  Serial.print("Right: ");
  Serial.println(v);
  return v > 500; 
 }
