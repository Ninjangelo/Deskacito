// -------------------- DESKACITO ARDUINO CODE (so far...) --------------------


//  -------------------- NFC INFORMATION BELOW  --------------------
/*
* edited by Velleman / Patrick De Coninck
* Read a card using a mfrc522 reader on your SPI interface
* Pin layout should be as follows (on Arduino Uno - Velleman VMA100):
* MOSI: Pin 11 / ICSP-4
* MISO: Pin 12 / ICSP-1
* SCK: Pin 13 / ISCP-3
* SS/SDA (MSS on Velleman VMA405) : Pin 10
* RST: Pin 9
* VCC: 3,3V (DO NOT USE 5V, VMA405 WILL BE DAMAGED IF YOU DO SO)
* GND: GND on Arduino UNO / Velleman VMA100
* IRQ: not used
*/
//  -------------------- NFC INFORMATION ABOVE  --------------------

//  -------------------- LIBRARIES  --------------------
#include <SPI.h>
#include <RFID.h>
#include <Servo.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include <NewPing.h>
#include "NewToneLib.h"

//  -------------------- LCD  --------------------
rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

//  -------------------- NFC-RFID and Servos  --------------------
#define SS_PIN 10
#define RST_PIN 9
//#define SS_PIN 30
//#define RST_PIN 29
Servo s1;
Servo s2;

RFID rfid(SS_PIN,RST_PIN);

//  -------------------- Buzzer  --------------------
#define buzzPin 7
MyTone t(false);

// -------------------- LED --------------------
int redLED = 8;
int greenLED = 5;
int serNum[5];

// -------------------- OBJECT IDENTIFICATION Communication --------------------
String signal;
String last_object;
int count = 0;

// -------------------- ULTRASONIC --------------------
#define TRIGGER_PIN 2
#define ECHO_PIN 3
#define MAX_DISTANCE 30
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
bool sensor = false;

/*
* This integer should be the code of Your Mifare card / tag 
*/
int cards[][5] = {{136,4,10,141,11}, {136,4,139,162,165}, {136,4,153,119,98}, {136,4,124,61,205}, {136,4,136,75,79}};

bool access = false;
bool access1 = false;
//bool identify = false;

// <<<<<<<<<<<<<<<<<<<< SETUP >>>>>>>>>>>>>>>>>>>>
void setup(){

    Serial.begin(9600);
    SPI.begin();
    rfid.init();
/*
* define VMA100 (UNO) pins 7 & 8 as outputs and put them LOW
*/
    // LED Setup
    pinMode(redLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    digitalWrite(redLED, LOW);
    //digitalWrite(power,LOW);

    // BUZZER Setup
    pinMode (buzzPin,OUTPUT);

    // SERVO Setup
    s1.attach(6);
    s2.attach(4);
    s1.write(0);
    s2.write(0);

    // LCD Setup
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);

    // SAME SERIAL RATE as the Rasberry Pi
    Serial.begin(9600);
   
}
// <<<<<<<<<<<<<<<<<<<< SETUP >>>>>>>>>>>>>>>>>>>>

// <<<<<<<<<<<<<<<<<<<< LOOP >>>>>>>>>>>>>>>>>>>>
void loop(){
    // Green LED to show it's Turned ON
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    // Close Doors Initially
    s1.write(0);
    s2.write(0);

    //initialise the ultrasonic sensor
    int distance = sonar.ping_cm();
    //Serial.print("Distance:");
    //Serial.println(distance);

    
    if (distance < 15){
      sensor = true;
      while(sensor) {
        delay(100);

        distance = sonar.ping_cm();
        if (distance >= 15){
          sensor = false;
          break;
        }

      //Serial.println(identify);
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("---LOGIN---");
      lcd.setCursor(1, 1);
      lcd.print("Scan your NFC");

      
      if(rfid.isCard()){

          if(rfid.readCardSerial()){
              Serial.print(rfid.serNum[0]);
              Serial.print(" ");
              Serial.print(rfid.serNum[1]);
              Serial.print(" ");
              Serial.print(rfid.serNum[2]);
              Serial.print(" ");
              Serial.print(rfid.serNum[3]);
              Serial.print(" ");
              Serial.print(rfid.serNum[4]);
              Serial.println("");
              
              for(int x = 0; x < sizeof(cards) / sizeof(cards[0]); x++){
                for(int i = 0; i < sizeof(rfid.serNum) / sizeof(rfid.serNum[0]); i++ ){
                    if(rfid.serNum[i] != cards[x][i]) {
                        access = false;
                        access1 = false;
                        break;
                    } else {
                        access = true;
                        lcd.clear();
                        lcd.setCursor(3,0);
                        lcd.print("Loading...");
                        t.tone(buzzPin, 2000, 90);
                        digitalWrite(greenLED, LOW);
                        delay(90);
                        t.tone(buzzPin, 0, 90);
                        digitalWrite(greenLED, HIGH);
                        delay(90);
                        t.tone(buzzPin, 20000, 500);
                        digitalWrite(greenLED, LOW);
                        delay(30);
                    }
                }
                if(access) break;
              }
            
          }
          
        while(access == true){
            distance = sonar.ping_cm();
            if (distance >= 15){
              sensor = false;
              access = false;
              access1 = true;
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Please Be Seated");
              lcd.setCursor(0,1);
              lcd.print("Appropriately!");
              break;
            }

            // NFC Tags
            int cards3[][5] = {{137,241,240,147,27}};
            int cards2[][5] = {{170,41,217,37,127}}; //Left NFC Tag
            int cards1[][5] = {{234,159,215,37,138}}; //Right NFC Tag

            //int cards1[][5] = {{119, 26, 232, 57, 188}};
            //int cards2[][5] = {{234, 108, 21, 11, 152}};
            //int cards3[][5] = {{234,159,215,37,138}};
            //int cards1[][5] = {{136,4,10,141,11},{136,4,139,162,165},{136,4,153,119,98},{136,4,136,75,79}};

            //Serial.println(identify);
            digitalWrite(greenLED, HIGH);
            digitalWrite(redLED, LOW);
            //Serial.println("---------- Welcome to Deskacito! ----------");
            lcd.clear();
            lcd.setCursor(3, 0);
            lcd.print("Welcome to");
            lcd.setCursor(3, 1);
            lcd.print("Deskacito!");
            s1.write(0);
            s2.write(0);
            delay(1000);
            lcd.clear();
            lcd.print("<----- Left");
            lcd.setCursor(4, 1);
            lcd.print("Right ----->");
            delay(1000);
            //delay(100);

            if (rfid.isCard()) {

              if(rfid.readCardSerial()){
                Serial.print(rfid.serNum[0]);
                Serial.print(" ");
                Serial.print(rfid.serNum[1]);
                Serial.print(" ");
                Serial.print(rfid.serNum[2]);
                Serial.print(" ");
                Serial.print(rfid.serNum[3]);
                Serial.print(" ");
                Serial.print(rfid.serNum[4]);
                Serial.println("");
              }
                
                for(int x = 0; x < sizeof(cards1) / sizeof(cards1[0]) && x < sizeof(cards2) / sizeof(cards2[0]); x++){
                  for(int i = 0; i < sizeof(rfid.serNum) / sizeof(rfid.serNum[0]); i++ ){
                    if(rfid.serNum[i] == cards1[x][i]){
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("<----Left------>");
                      lcd.setCursor(0, 1);
                      lcd.print("<---Drawer----->");
                      s1.write(180);
                      delay(5000);
                      s1.write(0);
                      break;

                    }else if(rfid.serNum[i] == cards2[x][i]){
                      lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print("<----Right----->");
                        lcd.setCursor(0, 1);
                        lcd.print("<---Drawer----->");
                        s2.write(180);
                        delay(5000);
                        s2.write(0);
                        break;
                    }

                    else if(rfid.serNum[i] != cards1[x][i] && rfid.serNum[i] != cards2[x][i] && rfid.serNum[i] != cards3[x][i]) {
                      access = false;
                      access1 = true;
                      s1.write(0);
                      s2.write(0);
                      delay(1000);
                      //Serial.println("Logout!");
                      lcd.clear();
                      lcd.setCursor(1, 0);
                      lcd.print("Logging out...");
                      digitalWrite(redLED, HIGH);
                      t.tone(buzzPin, 600, 100);
                      delay(100);
                      digitalWrite(redLED, LOW);
                      t.tone(buzzPin, 1200, 100); 
                      delay(100);
                      digitalWrite(redLED, HIGH);
                      t.tone(buzzPin, 600, 100);
                      delay(100);
                      digitalWrite(redLED, LOW);
                      t.tone(buzzPin, 1200, 100);
                      delay(100);
                      //access = false;
                      break;

                    } else {
                      access = true;
                    
                  }
                }
                if(access) break; 
              }
            }

            // Reading from Serial Port
            if (Serial.available()) {
              signal = Serial.readStringUntil('\n');
              signal.trim();

              // Open LEFT Compartment
              if (signal.equals("clock") && last_object != "clock") {
                s1.write(0);
                s2.write(180);
                lcd.clear();
                lcd.print("Object:");
                lcd.setCursor(0, 1);
                lcd.print("Watch");
                //Serial.println("Bottle");
                digitalWrite(greenLED, HIGH);
                delay(500);
                digitalWrite(greenLED, LOW);
                delay(500);
                digitalWrite(greenLED, HIGH);
                delay(5000);
                s2.write(0);
                last_object = "clock";

                while (last_object == "clock") {
                  if (count == 3) {
                    count = 0;
                    Serial.println("Count Reset");
                    last_object = "";
                  } else {
                    count += 1;
                    Serial.println(count);
                  }
                  delay(1000);
                  //Serial.println(count);
                }
              }

              // Open RIGHT Compartment 
              else if (signal.equals("scissors") && last_object != ("scissors")) {
                s1.write(180);
                s2.write(0);
                lcd.clear();
                lcd.print("Object:");
                lcd.setCursor(0, 1);
                lcd.print("Scissors");
                //Serial.println("Scissors");
                digitalWrite(greenLED, HIGH); 
                delay(500);
                digitalWrite(greenLED, LOW);
                delay(500);
                digitalWrite(greenLED, HIGH);
                delay(5000);
                s1.write(0);
                last_object = "scissors";

                while (last_object == "scissors") {
                  if (count == 3) {
                    count = 0;
                    Serial.println("Count Reset");
                    last_object = "";
                  } else {
                    count += 1;
                    Serial.println(count);
                  }
                  delay(1000);
                  //Serial.println(count);
                }
              }

              // TESTING Path if Unknown Object Category is Detected
              else if (signal.equals("none") && last_object != "none"){
                s1.write(0);
                s2.write(0);
                lcd.clear();
                lcd.print("NOT");
                lcd.setCursor(0, 1);
                lcd.print("RECOGNISED");
                //Serial.println("NOT RECOGNISED");
                t.tone(buzzPin, 1000, 500);
                digitalWrite(redLED, HIGH);
                delay(500);
                t.tone(buzzPin, 600, 500);
                digitalWrite(redLED, LOW); 
                delay(500);
                t.tone(buzzPin, 1000, 500);
                digitalWrite(redLED, HIGH);
                delay(5000);
                last_object = "none";
                
                while (last_object == "none") {
                  if (count == 3) {
                    count = 0;
                    Serial.println("Count Reset");
                    last_object = "";
                  } else {
                    count += 1;
                    Serial.println(count);
                  }
                  delay(1000);
                  //Serial.println(count);
                }
              }
            }

            
        } if (access == false && access1 == false) {
  /*
  * NON-Valid card : switch ON and OFF the LED twice for 0.5 seconds
  */
            //Serial.println("Access Denied!");
            lcd.clear();
            lcd.setCursor(5, 0);
            lcd.print("Access");
            lcd.setCursor(5, 1);
            lcd.print("Denied!");
            t.tone(buzzPin, 500, 500);
            digitalWrite(redLED, HIGH);
            delay(500);
            t.tone(buzzPin, 250, 500);
            digitalWrite(redLED, LOW); 
            delay(500);
            digitalWrite(redLED, HIGH);
            delay(500);
            digitalWrite(redLED, LOW);
            delay(500);
        }        
      }
        rfid.halt();
    }

    } else {
      sensor = false;
      s1.write(0);
      s2.write(0);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please be Seated");
      t.tone(buzzPin, 600, 500);
      lcd.setCursor(0,1);
      lcd.print("Appropriately!");
      delay(500);
    }

  }