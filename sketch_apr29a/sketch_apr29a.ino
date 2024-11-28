#include<Servo.h>
Servo s1;
Servo s2;

String signal;

void setup() {
  Serial.begin(9600);
  
  s1.attach(39);
  s2.attach(36);

  delay(2000);

}

void loop() {
  if (Serial.available()) {
    signal = Serial.readStringUntil('\n');
    signal.trim();

    if (signal.equals("on")) {
      s1.write(180);
      s2.write(0);
    }
    else if (signal.equals("off")) {
      s1.write(0);
      s2.write(180);
    }
    Serial.print("Signal:");
    Serial.println(signal);
  }
}
