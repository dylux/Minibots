#include "config.h"
#include "control.h"
#include "drive.h"
#include "imu.h"
#include "sensors.h"

void setup() {
    Serial.begin(
        115200);  // Open serial monitor at 115200 baud to see ping results.

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
    resetIMU();

    gripperSetPosition();
}

void loop() {
    while (readRightUS()<20 && readLeftUS()<20) {
        Serial.println("Following line ");
        bool detectedBall = followLine();
        fixGettingStuck();
        if(detectedBall){
          Serial.println("Ball!");
          moveL(40,FWD);
          moveR(40,FWD);
          delay(300);//TUNE AT COMPETITION
          stopMotors();
          delay(500);

          
          grabBall();
          delay(500);
        }
    }
    Serial.println("step 1");
    stopMotors();
    delay(500);

    handleIntersection();

    
}
