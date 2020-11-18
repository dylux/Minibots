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
  //Serial.println(readFrontUS());

  driveInCorridorUntilIntersection();
  stopMotors();
  delay(1000);
  handleIntersection();
  stopMotors();
  delay(1000);
  
  // gripperGrasp();
  // gripperSetPosition();

  /*
  while (millis() - startTime < 3*1000){
    driveInCorridor();
  }*/
}

void driveInCorridorUntilIntersection(){
  double distToLeft = readLeftUS();
  double distToRight = readRightUS();

  while (max(distToLeft, distToRight) < WALL_DIST) {
    float baseSpeed = 20;
    float turnAmount = 50;
    float leftMotorSpeed = (distToRight / (distToRight+distToLeft))*turnAmount + baseSpeed;
    float rightMotorSpeed = (distToLeft / (distToRight+distToLeft))*turnAmount + baseSpeed;
  
    moveL(leftMotorSpeed,FWD);
    moveR(rightMotorSpeed,FWD); 
    
    distToLeft = readLeftUS();
    distToRight = readRightUS();
  }
}

void handleIntersection(){
  bool canGoLeft = (readLeftUS() > GRID_DIST);
  bool canGoForward = (readFrontUS() > GRID_DIST);
  bool canGoRight = (readRightUS() > GRID_DIST);
  driveForDuration(40,500);
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
