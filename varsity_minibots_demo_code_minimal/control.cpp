#include "control.h"

#include "config.h"
#include "drive.h"
#include "sensors.h"

void followLine() {
    float baseSpeed = 45;
    float leftMotorSpeed = baseSpeed;
    float rightMotorSpeed = baseSpeed;
    float turnAmount = 20;

    if (readLeftLight() == 1 && readRightLight() == 0) {
        leftMotorSpeed += turnAmount;
        rightMotorSpeed -= turnAmount;
    }
    if (readLeftLight() == 0 && readRightLight() == 1) {
        rightMotorSpeed += turnAmount;
        leftMotorSpeed -= turnAmount;
    }
    moveL(leftMotorSpeed, FWD);
    moveR(rightMotorSpeed, FWD);
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

    //followLineCarefullyUntilIntersection();
    
    while(readFrontUS()>18){
      followLine();
    }
    
    // Const forward movement
    float dist;
    do {
      moveL(40,FWD);
      moveR(40,FWD);
      dist = readFrontUS();
      Serial.println(dist);
    } while(dist>4 && dist<100);
    stopMotors();
    //driveForDuration(40, 1400);
    
    bool canGoLeft = (readLeftUS() > GRID_DIST);
    bool canGoForward = (readFrontUS() > GRID_DIST);
    bool canGoRight = (readRightUS() > GRID_DIST);

    if (canGoRight) {
        turnDegreesRight(90);
    } else if (canGoForward) {
    } else if (canGoLeft) {
        turnDegreesLeft(90);
    } else {
        turnDegreesLeft(180);
    }
    //driveForDuration(40, 1000);
}

void grabBall(){
    stopMotors();
    gripperGrasp();
    turnDegreesLeft(120);
    delay(1000);
    turnLeftUntilLine();
    delay(1000);

    
    wrist.attach(WRIST_PIN);
    wrist.write(30);
    delay(1000);

    wrist.detach();
}

void turnLeftUntilLine(){
    while(readLeftLight() == 1){
      moveL(60,REV);
      moveR(60,FWD);
    }
    while(readLeftLight() == 0){
      moveL(60,REV);
      moveR(60,FWD);
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
