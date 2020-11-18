#include "config.h"
#include <Servo.h>

extern Servo wrist;
extern Servo gripper;

// Move the right motor (values depend on the specific wiring configuration)
void moveR(float power, bool dir);
// Move the left motor (values depend on the specific wiring configuration)
void moveL(float power, bool dir);

// Stop the motors all together
void stopMotors();

void driveForDuration(int motorSpeed, int duration);

// Base position of the gripper (close gripper and move it upwards)
void gripperSetPosition();
// Move the gripper down and open it to grasp the ping pong ball
void gripperOpenPosition();
// Close the gripper and move it up
void gripperGrasp();

// Turn left(counterclockwise when viewed from above) using a proportional control on the gyro sensor readings
void turnDegreesLeft(float targetAngle);
void turnDegreesRight(float targetAngle);
// Follow the right hand side of the wall using a proportional/derivative controller. 
void followRightWall(float frontDistanceCutoff);

// Smash into the wall and then move back a little bit for the 90 degree turn
void adjustWithWall();
