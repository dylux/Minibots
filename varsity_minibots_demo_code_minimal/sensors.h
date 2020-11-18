#include <HCSR04.h>
#include <Adafruit_MPU6050.h>
#include "config.h"

extern UltraSonicDistanceSensor US_left;
extern UltraSonicDistanceSensor US_right;
extern UltraSonicDistanceSensor US_front;

extern Adafruit_MPU6050 mpu;

// Function to read values of the LEFT ultrasonic sensor
double readLeftUS();
// Function to read values of the RIGHT ultrasonic sensor
double readRightUS();
// Function to read values of the FRONT ultrasonic sensor
double readFrontUS();

// Function to read values of the LEFT light sensor
// Output true/false if it finds the line or not (can tuen on the sensor it self)
float readLeftLight();
// Function to read values of the RIGHT light sensor
// Output true/false if it finds the line or not (can tuen on the sensor it self)
float readRightLight();

// Quick print function to see if all the ultrasonic sensors are funcitonal 
void displayUS();
// Quick print function to see if all the light sensors are funcitonal 
void displayLight();
