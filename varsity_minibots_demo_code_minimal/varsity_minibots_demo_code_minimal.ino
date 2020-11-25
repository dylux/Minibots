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
}

void loop() {
    pollIMU();
    Serial.println(getDistance());
    // moveForwardFor(1.0);
    // delay(2000);
    /*while (!((readLeftUS() > GRID_DIST)||(readRightUS() > GRID_DIST))) {
        followLine();
    }
    stopMotors();
    delay(1000);
    handleIntersection();
    stopMotors();
    delay(1000);*/
}
