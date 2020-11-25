#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>

#include "sensors.h"

unsigned long prevtime;
float angle = 0;
float distance = 0;
float velocity = 0;
sensors_event_t a, g, temp;
float angleTuningFactor = 0.000605;
float accelTuningFactor = 0.03;

void resetIMU() {
    prevtime = micros();
    angle = 0;
    distance = 0;
    velocity = 0;
}

void pollIMU() {
    mpu.getEvent(&a, &g, &temp);
    unsigned long curtime = micros();
    float delta = curtime - prevtime;
    prevtime = curtime;

    float angleAdjustment = delta / 1000000.0 / PI * 180.0;
    angle += (g.gyro.z + angleTuningFactor) * angleAdjustment;

    float accel = g.acceleration.x + accelTuningFactor;
    velocity += (delta / 1000000.0) * accel;
    distance += (delta / 1000000.0) * velocity;
}

float getAngle() { return angle; }

float getDistance() { return distance; }
