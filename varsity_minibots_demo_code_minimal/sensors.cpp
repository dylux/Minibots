#include <Arduino.h>

#include "sensors.h"

UltraSonicDistanceSensor US_left(US_LEFT_TRIGGER, US_LEFT_ECHO);
UltraSonicDistanceSensor US_right(US_RIGHT_TRIGGER, US_RIGHT_ECHO);
UltraSonicDistanceSensor US_front(US_FRONT_TRIGGER, US_FRONT_ECHO);

Adafruit_MPU6050 mpu;
