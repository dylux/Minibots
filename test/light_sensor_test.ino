const int lightPin1 = 16;
const int lightPin2 = 17;

void setup() {
   Serial.begin(9600); // Starting Serial Terminal
   pinMode(lightPin1, INPUT);
   pinMode(lightPin2, INPUT);
}

void loop() {
   long duration, inches, cm;
   double val = analogRead(lightPin);
   Serial.print(val);
   Serial.println();
   delay(300);
}
