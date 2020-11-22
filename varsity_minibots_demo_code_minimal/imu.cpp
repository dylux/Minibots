#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include "sensors.h"

unsigned long prevtime;
float angle = 0;
float distance = 0;
sensors_event_t a, g, temp;
float tuningFactor = 0.000605;

void resetIMU() {
  prevtime = micros();
  angle = 0;
  distance = 0;
}

void pollIMU() {
  mpu.getEvent(&a, &g, &temp);
  unsigned long curtime = micros();
  float delta = curtime - prevtime;
  prevtime = curtime;
  
  float angleAdjustment = delta / 1000000.0 / PI * 180.0;
  angle += (g.gyro.z + tuningFactor) * angleAdjustment;
  distance += delta * delta * (-g.acceleration.x) / 2;
}

float getAngle() {
  return angle;
}

float getDistance() {
  return distance;
}
