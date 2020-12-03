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
long lastTimeOfMovement = 0;


void resetIMU() {
    prevtime = micros();
    angle = 0;
    distance = 0;
    velocity = 0;
}

long getLastTimeOfMovement(){
  return (long)micros()-lastTimeOfMovement;
}

void pollIMU() {
    if(lastTimeOfMovement==0){
      lastTimeOfMovement = micros();
    }
    mpu.getEvent(&a, &g, &temp);
    unsigned long curtime = micros();
    float delta = curtime - prevtime;
    prevtime = curtime;

    float angleAdjustment = delta / 1000000.0 / PI * 180.0;
    angle += (g.gyro.z + angleTuningFactor) * angleAdjustment;

    float accel = g.acceleration.x + accelTuningFactor;
    velocity += (delta / 1000000.0) * accel;
    distance += (delta / 1000000.0) * velocity;

    Serial.print("accel: ");
    float a = g.acceleration.x*g.acceleration.x+g.acceleration.y*g.acceleration.y+g.acceleration.z*g.acceleration.z;
    Serial.println(a);
    if(a > 0.1){
      Serial.println("reset!");
      lastTimeOfMovement = curtime;
    }
    Serial.println(getLastTimeOfMovement());
}

float getAngle() { return angle; }

float getDistance() { return distance; }
