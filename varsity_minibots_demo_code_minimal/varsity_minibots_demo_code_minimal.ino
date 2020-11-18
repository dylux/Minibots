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
  gripperSetPosition();
  unsigned long startTime = millis();
  while (millis() - startTime < 3*1000){
    driveInCorridor();

    if(detectIntersection()){
      handleIntersection();
    }
  }
  
  stopMotors();
  delay(1000);
  gripperGrasp();
  delay(10000);
}

void driveInCorridor(){
  double distToLeft = readLeftUS();
  double distToRight = readRightUS();
  
  float baseSpeed = 0;
  float turnAmount = 70;
  float leftMotorSpeed = (distToRight / (distToRight+distToLeft))*turnAmount + baseSpeed;
  float rightMotorSpeed = (distToLeft / (distToRight+distToLeft))*turnAmount + baseSpeed;

  moveL(leftMotorSpeed,FWD);
  moveR(rightMotorSpeed,FWD);
}

bool detectIntersection(){
  int gridLength = 20;
  if (readLeftUS() > gridLength || readRightUS() > gridLength) {
    return true;
  }
  else{
    return false;
  }
}

void handleIntersection(){
  bool canGoLeft = (readLeftUS() > gridLength);
  bool canGoFront = (readFrontUS() > gridLength);
  bool canGoRight = (readRightUS() > gridLength);
  driveForDuration(70,1000);
  if(canGoRight){
    turnDegreesRight(90);
  }
  else if(canGoForward){
    
  } else if(canGoLeft) {
    turnDegreesLeft(90);
  } else {
    turnDegreesLeft(180);
  }
  driveForDuration(70,1000);
}

void driveForDuration(motorSpeed,duration){
  moveL(motorSpeed,FWD);
  moveR(motorSpeed,FWD);
  delay(duration);

  stopMotors();
}
