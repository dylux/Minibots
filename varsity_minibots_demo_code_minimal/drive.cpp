#include "drive.h"

#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>

#include "imu.h"
#include "sensors.h"
#include "control.h"

Servo wrist;
Servo gripper;

// Move the right motor (values depend on the specific wiring configuration)
void moveR(float power, bool dir) {
    power *= RIGHT_RATIO;
    if (power > 100) {
        power = 100;
    } else if (power < 0) {
        power = 0;
    }
    int pwm = int(255 * power / 100);

    if (pwm == 0) {
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, LOW);
    } else {
        if (dir == 0) {
            digitalWrite(AIN1, LOW);
            digitalWrite(AIN2, HIGH);
        } else {
            digitalWrite(AIN1, HIGH);
            digitalWrite(AIN2, LOW);
        }
    }
    analogWrite(APWM, pwm);
}

// Move the left motor (values depend on the specific wiring configuration)
void moveL(float power, bool dir) {
    power *= LEFT_RATIO;
    if (power > 100) {
        power = 100;
    } else if (power < 0) {
        power = 0;
    }
    int pwm = int(255 * power / 100);

    if (pwm == 0) {
        digitalWrite(BIN1, LOW);
        digitalWrite(BIN2, LOW);
    } else {
        if (dir == 1) {
            digitalWrite(BIN1, LOW);
            digitalWrite(BIN2, HIGH);
        } else {
            digitalWrite(BIN1, HIGH);
            digitalWrite(BIN2, LOW);
        }
    }
    analogWrite(BPWM, pwm);
}

// Stop the motors all together
void stopMotors() {
    moveL(0, FWD);
    moveR(0, FWD);
}

void driveForDuration(int motorSpeed, int duration) {
    moveL(motorSpeed, FWD);
    moveR(motorSpeed, FWD);
    delay(duration);

    stopMotors();
}

// Base position of the gripper (close gripper and move it upwards)
void gripperSetPosition() {
    wrist.attach(WRIST_PIN);
    gripper.attach(GRIPPER_PIN);
    wrist.write(100);
    gripper.write(90);
    delay(1000);

    wrist.detach();
    gripper.detach();
}

// Close the gripper and move it up
void gripperGrasp() {
    wrist.attach(WRIST_PIN);
    gripper.attach(GRIPPER_PIN);

    gripper.write(190);
    delay(500);
    wrist.write(70);
    delay(500);

    wrist.detach();
    gripper.detach();
}

// Turn left(counterclockwise when viewed from above) using a proportional
// control on the gyro sensor readings
void turnDegreesLeft(float targetAngle) {
    resetIMU();
    float basePWM = 50;
    float kp = 0.35;
    unsigned long prevtime = micros();
    while (true) {
        if(fixGettingStuck()){
          return;  
        }
        pollIMU();
        float angle = getAngle();
        float error = targetAngle - angle;
        float motorPWM = basePWM + error * kp;
        moveR(motorPWM, FWD);
        moveL(motorPWM, REV);
        if (abs(error) < 3) {
            break;
        }
    }
    stopMotors();
}

void turnDegreesRight(float targetAngle) {
    resetIMU();
    float basePWM = 40;
    float kp = 0.4;
    unsigned long prevtime = micros();
    while (true) {
        fixGettingStuck();
        pollIMU();
        float angle = -getAngle();
        float error = targetAngle - angle;
        float motorPWM = basePWM + error * kp;
        moveR(motorPWM, REV);
        moveL(motorPWM, FWD);
        if (abs(error) < 3) {
            break;
        }
    }
    stopMotors();
}

void moveForwardFor(float distance) {
    resetIMU();
    float basePWM = 50;
    float angleFactor = 5;
    while (getDistance() < distance) {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        pollIMU();
        float motorPWM = getAngle();
        moveR(abs(basePWM - motorPWM), FWD);
        moveL(abs(basePWM + motorPWM), FWD);
    }
    stopMotors();
}

void moveUntilWall(float distance) {
    resetIMU();
    float basePWM = 50;
    float angleFactor = 5;
    int closeReadings = 0;
    while (closeReadings<5) {
        fixGettingStuck();
        float d = readFrontUS();
        Serial.println(d);
        if(d<distance){
          closeReadings++;
        } else{
          closeReadings = 0;
        }
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        pollIMU();
        float motorPWM = getAngle();
        moveR(abs(basePWM - motorPWM), FWD);
        moveL(abs(basePWM + motorPWM), FWD);
    }
    stopMotors();
}
