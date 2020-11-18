/*
 *  PINS
 */

//Motors

// Right motor
#define AIN1 4
#define AIN2 9
#define APWM 5

// Left motor
#define BIN1 8
#define BIN2 7
#define BPWM 6

// Line / table sensors

#define LIGHT_LEFT A2
#define LIGHT_RIGHT A3

// Servos

#define WRIST_PIN 11
#define GRIPPER_PIN 10

// Ultrasonic sensors

#define US_LEFT_TRIGGER A0
#define US_LEFT_ECHO A1

#define US_RIGHT_TRIGGER 2
#define US_RIGHT_ECHO 3

#define US_FRONT_TRIGGER 12
#define US_FRONT_ECHO 13

/*
 *  config parameters
 */

// drive settings

#define FWD 1
#define REV 0

// ultrasonic settings

#define US_NUM_READINGS 2
#define INF 1e9
#define WALL_DIST 15
#define GRID_DIST 30
