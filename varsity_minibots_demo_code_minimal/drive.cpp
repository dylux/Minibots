#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include "drive.h"
#include "sensors.h"

Servo wrist;
Servo gripper;

// Move the right motor (values depend on the specific wiring configuration)
void moveR(float power, bool dir) {

  if(power > 100) {
    power = 100;
  }
  else if(power < 0) {
    power = 0;
  }
  int pwm = int(255*power/100);


  if(pwm == 0) {
    digitalWrite(AIN1, LOW); digitalWrite(AIN2, LOW);
  }
  else {
    if(dir == 0) {
      digitalWrite(AIN1, LOW); digitalWrite(AIN2, HIGH);
    }
    else {
      digitalWrite(AIN1, HIGH); digitalWrite(AIN2, LOW);
    }
  }
  analogWrite(APWM, pwm);
}

// Move the left motor (values depend on the specific wiring configuration)
void moveL(float power, bool dir) {

  if(power > 100) {
    power = 100;
  }
  else if(power < 0) {
    power = 0;
  }
  int pwm = int(255*power/100);


  if(pwm == 0) {
    digitalWrite(BIN1, LOW); digitalWrite(BIN2, LOW);
  }
  else {
    if(dir == 1) {
      digitalWrite(BIN1, LOW); digitalWrite(BIN2, HIGH);
    }
    else {
      digitalWrite(BIN1, HIGH); digitalWrite(BIN2, LOW);
    }
  }
  analogWrite(BPWM, pwm);
}


// Stop the motors all together
void stopMotors() {
  moveL(0, FWD); moveR(0, FWD);
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
  
  gripper.write(175);
  delay(1000);
  wrist.write(60);
  delay(300);

  
  wrist.detach();
  gripper.detach();
}


// Turn left(counterclockwise when viewed from above) using a proportional control on the gyro sensor readings
void turnDegreesLeft(float targetAngle) {
  float integral = 0;
  // float angleAdjustment = 0.45;
  float basePWM = 40;
  float kp = 0.2;
  unsigned long prevtime = micros();
  while(true) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    unsigned long curtime = micros();
    float delta = curtime - prevtime;

    prevtime = curtime;
    Serial.println(delta);
    
    float angleAdjustment = delta / 1000000.0 / PI * 180.0;
    
    integral += g.gyro.z * angleAdjustment;

    float error = targetAngle - integral;
    float motorPWM = basePWM + error*kp;

    moveR(motorPWM, FWD);
    moveL(motorPWM, REV);
    Serial.println(integral);

    if(abs(error) < 3) {
      break;
    }
  }

  stopMotors();
}
