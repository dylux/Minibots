#include "config.h"
#include "drive.h"
#include "sensors.h"

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(APWM, OUTPUT);
  
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(BPWM, OUTPUT);
  
  pinMode(LIGHT_LEFT, INPUT);
  pinMode(LIGHT_RIGHT, INPUT);
  
  pinMode(WRIST_PIN, OUTPUT);
  pinMode(GRIPPER_PIN, OUTPUT);
  
  Serial.println("Finding Sensor");
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("Sensor Found!!");

  delay(1000);
}

void loop() {
  for(int i = 0; i<100; i++){
    driveInCorridor();
  }
  
  stopMotors();
  delay(10000);
}

void driveInCorridor(){
  float distToLeft = readLeftUS();
  float distToright = readRightUS();

  float baseSpeed = 20;
  float turnAmount = 20;
  float leftMotorSpeed = (distToRight / (distToRight+distToLeft))*turnAmount + baseSpeed;
  float rightMotorSpeed = (distToLeft / (distToRight+distToLeft))*turnAmount + baseSpeed;

  moveL(leftMotorSpeed,FWD);
  moveR(rightMotorSpeed,FWD);
}