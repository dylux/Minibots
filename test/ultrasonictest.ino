

const int us_left_trigger = 14;
const int us_left_echo = 15;
const int us_centre_trigger = 12;
const int us_centre_echo = 13;
const int us_right_trigger = 2;
const int us_right_echo = 3;

void setup() {
   Serial.begin(9600); // Starting Serial Terminal
   pinMode(us_left_trigger, OUTPUT);
   pinMode(us_left_echo, INPUT);
   pinMode(us_centre_trigger, OUTPUT);
   pinMode(us_centre_echo, INPUT);
   pinMode(us_right_trigger, OUTPUT);
   pinMode(us_right_echo, INPUT);
}

void loop() {
   Serial.print(ultrasonic_read(us_left_trigger, us_left_echo));
   Serial.print(" ");
   Serial.print(ultrasonic_read(us_centre_trigger, us_centre_echo));
   Serial.print(" ");
   Serial.print(ultrasonic_read(us_right_trigger, us_right_echo));
   Serial.println();
   delay(100);
}

double ultrasonic_read(int ping, int echo){
   digitalWrite(ping, LOW);
   delayMicroseconds(2);
   digitalWrite(ping, HIGH);
   delayMicroseconds(10);
   digitalWrite(ping, LOW);
   pinMode(echo, INPUT);
   return pulseIn(echo, HIGH) / 29.0 / 2.0; // in centimetres
}
