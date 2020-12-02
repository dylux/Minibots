#include "control.h"

#include "config.h"
#include "drive.h"
#include "sensors.h"

bool followLine() {
    float baseSpeed = 45;
    float leftMotorSpeed = baseSpeed;
    float rightMotorSpeed = baseSpeed;
    float turnAmount = 20;

    bool leftLight = readLeftLight();
    bool rightLight = readRightLight();

    if (leftLight == 1 && rightLight == 0) {
        leftMotorSpeed += turnAmount;
        rightMotorSpeed -= turnAmount;
    }
    if (leftLight == 0 && rightLight == 1) {
        rightMotorSpeed += turnAmount;
        leftMotorSpeed -= turnAmount;
    }
    moveL(leftMotorSpeed, FWD);
    moveR(rightMotorSpeed, FWD);

    return leftLight==0 && rightLight==0;
}

// INTERSECTIONS
// =============

void followLineCarefullyUntilIntersection() {
    // Forward (depending on US) until intersection touched
    for (;;) {
        while (!readLeftLight() && !readRightLight()) {
            moveL(30, FWD);
            moveR(30, FWD);
            delay(10);
        }
        stopMotors();
        // Try correction
        const int MAX_TURN_TRIES = 5;
        int turnTries = 0;
        if (readLeftLight()) {
            for (; turnTries < MAX_TURN_TRIES; turnTries++) {
                turnDegreesLeft(1);
                if (!readLeftLight()) break;
            }
        } else {
            for (; turnTries < MAX_TURN_TRIES; turnTries++) {
                turnDegreesRight(1);
                if (!readRightLight()) break;
            }
        }

        // If correction fails, we arrived at interesction
        if (turnTries == MAX_TURN_TRIES) {
            break;
        }
    }
}

bool detectIntersection(){
  return (readLeftUS() > GRID_DIST) || (readRightUS() > GRID_DIST);
  //Serial.println(readFrontUS());
  //delay(100);
  //return(readFrontUS()<18);
}

void handleIntersection() {
    Serial.println("step 2");
    
    while (readFrontUS()>17) {
        Serial.println(readFrontUS());
        followLine();
    }
    Serial.println("In intersection");
    stopMotors();
    delay(500);

    bool openLeft = readLeftUS()>10;
    

    Serial.println("step 4");
    moveUntilWall(8);
    stopMotors();
    delay(500);

    if(openLeft)
      turnLeftUntilLine();
    else
      turnRightUntilLine();

    delay(500);

    for(int i = 0; i<500; i++){
      followLine();
    }
    stopMotors();
    delay(500);
}

void grabBall(){
    stopMotors();
    Serial.println("Ball 1");
    gripperGrasp();
    Serial.println("Ball 2");
    turnDegreesLeft(120);
    Serial.println("Ball 3");
    delay(500);
    turnLeftUntilLine();
    Serial.println("Ball 4");
    delay(500);

    
    wrist.attach(WRIST_PIN);
    wrist.write(30);
    delay(1000);

    wrist.detach();
    Serial.println("Ball 5followLine");
}

void turnLeftUntilLine(){
    while(readLeftLight() == 1){
      moveL(60,REV);
      moveR(60,FWD);
    }
    while(readLeftLight() == 0){
      moveL(50,REV);
      moveR(50,FWD);
    }
    stopMotors();
}



void turnRightUntilLine(){
    while(readRightLight() == 1){
      moveL(60,FWD);
      moveR(60,REV);
    }
    while(readRightLight() == 0){
      moveL(50,FWD);
      moveR(50,REV);
    }
    stopMotors();
}

// DEPRECATED
// ==========

void driveInCorridorUntilIntersection() {
    double distToLeft = readLeftUS();
    double distToRight = readRightUS();

    while (max(distToLeft, distToRight) < WALL_DIST) {
        float baseSpeed = 40;
        float turnAmount = 30;
        float leftMotorSpeed =
            (distToRight / (distToRight + distToLeft)) * turnAmount + baseSpeed;
        float rightMotorSpeed =
            (distToLeft / (distToRight + distToLeft)) * turnAmount + baseSpeed;

        moveL(leftMotorSpeed, FWD);
        moveR(rightMotorSpeed, FWD);

        distToLeft = readLeftUS();
        distToRight = readRightUS();
    }
}
