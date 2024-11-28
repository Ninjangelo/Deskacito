#include<Servo.h>
Servo s1;
//Servo s2;

void setup() {
  // put your setup code here, to run once:

s1.attach(6);
//s2.attach(6);
}

void loop() {
  // put your main code here, to run repeatedly:
  s1.write(0);
  //s2.write(0);
  //delay(1000);
  //s1.write(180);
  //s2.write(180);
  //delay(1000);
  //s1.write(0);
  //s2.write(0);
}
