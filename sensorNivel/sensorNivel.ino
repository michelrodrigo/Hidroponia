/*
 HC-SR04 Ping distance sensor]
 VCC to arduino 5v GND to arduino GND
 Echo to Arduino pin 13 Trig to Arduino pin 12
 More info at: http://goo.gl/kJ8Gl
 */

#define trigPin PC_4
#define echoPin PB_3

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  int duration;
  // int freq;
  float distance;
  int conversion;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 27.6233;
 
    
    Serial.print(distance);
    Serial.println(" cm");

  delay(10);
}
