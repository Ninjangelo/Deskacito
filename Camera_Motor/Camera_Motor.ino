// SERVO Library & Servo
#include<Servo.h>
#include <Wire.h>
#include "rgb_lcd.h"

// LCD
rgb_lcd lcd;

int count = 0;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;
Servo s1;
Servo s2;

int redLED = 8;
int greenLED = 5;

String signal;
String last_object;

void setup() {
  // Needs to be the same as the Serial Rate on the Raspberry Pi
  Serial.begin(9600);
  
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  pinMode(A0, OUTPUT);

  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);


  // DIGITAL PINOUTS
  s1.attach(6);
  s2.attach(4);
  s1.write(0);
  s2.write(0);
  Serial.println("working");

  // Delay for 0.1 Seconds
  delay(100);

}

void loop() {
  lcd.clear();
  digitalWrite(A0, LOW);
  //lcd.clear();
  //lcd.setCursor(5, 0);
  //lcd.print("CAMERA");
  //lcd.setCursor(3, 1);
  //lcd.print("Show Object");
  //delay(100);
  // Checks if Serial is running
  if (Serial.available()) {
    // Reads and Stores what is being Printed in the Serial Monitor
    signal = Serial.readStringUntil('\n');
    signal.trim();
    //Serial.print("Signal:");
    //Serial.println(signal);
    // SERVO 1 ACTIVATION CONDITION
    if (signal.equals("bottle") && last_object != "bottle") {
      s1.write(180);
      s2.write(0);
      Serial.println("Bottle");
      lcd.clear();
      lcd.print("Object:");
      lcd.setCursor(0, 1);
      lcd.print("Bottle");
      digitalWrite(greenLED, HIGH);
      digitalWrite(A0, HIGH); 
      delay(500);
      digitalWrite(A0, LOW);
      delay(1000);
      s1.write(0);
      last_object = "bottle";
    }

    // SERVO 2 ACTIVATION CONDITION
    else if (signal.equals("scissors") && last_object != "scissors") {
      s1.write(0);
      s2.write(180);
      Serial.println("Scissors");
      lcd.clear();
      lcd.print("Object:");
      lcd.setCursor(0, 1);
      lcd.print("Scissors");
      digitalWrite(greenLED, HIGH); 
      digitalWrite(A0, HIGH);
      delay(500);
      digitalWrite(A0, LOW);
      delay(1000);
      s2.write(0);
      last_object = "scissors";
    }

    // NO ITEM DETECTED
    //else if (signal.equals("none")) {
      //s1.write(0);
      //s2.write(0);
      //lcd.clear();
      //lcd.print("NOT");
      //lcd.setCursor(0, 1);
      //lcd.print("RECOGNISED");
      //digitalWrite(redLED, HIGH);
      //delay(500);
      //digitalWrite(redLED, LOW); 
      //delay(500);
      //digitalWrite(redLED, HIGH);
      //delay(500);
      //digitalWrite(redLED, LOW);

    //}
    if (count == 30){
      count = 0;
      last_object = "";
      }
      else{
        count+=1;
      }
      delay(100);
  }

}
