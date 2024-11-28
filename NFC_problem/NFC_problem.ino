// -------------------- DESKACITO ARDUINO CODE (so far...) --------------------


// -------------------- NFC INFORMATION BELOW --------------------
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
// -------------------- NFC INFORMATION ABOVE --------------------

// -------------------- LIBRARIES --------------------
#include <SPI.h>
#include <RFID.h>
#include <Servo.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include <NewPing.h>
#include <NewToneLib.h>

// -------------------- LCD --------------------
rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

// -------------------- NFC-RFID and Servos --------------------
#define SS_PIN 10
#define RST_PIN 9
//#define SS_PIN 30
//#define RST_PIN 29
Servo s1;
Servo s2;

RFID rfid(SS_PIN,RST_PIN);

// -------------------- BUZZER --------------------
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
#define MAX_DISTANCE 20
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
bool sensor = false;


/*
* This integer should be the code of Your Mifare card / tag 
*/
int cards[][5] = {{136,4,10,141,11}, {136,4,139,162,165}, {136,4,153,119,98}, {136,4,124,61,205}, {136,4,136,75,79}};

bool access = false;
bool access1 = false;

// <<<<<<<<<<<<<<<<<<<< SETUP >>>>>>>>>>>>>>>>>>>>
void setup(){
    
    // Same Serial rate as the Raspberry Pi 
    Serial.begin(9600);
    //RFID Startup
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
   
}
// <<<<<<<<<<<<<<<<<<<< SETUP >>>>>>>>>>>>>>>>>>>>

void loop(){
    // Initial TURN ON State
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    s1.write(0);
    s2.write(0);

    int distance = sonar.ping_cm();

    if (distance < 15) {
      sensor = true;
      while (sensor) {
        delay(100);

        distance = sonar.ping_cm();
        if (distance >= 15) {
          sensor = false;
          break;
        }

        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("---LOGIN---");
        lcd.setCursor(1, 1);
        lcd.print("Scan your NFC");

    
        // Judges the NFC being Scanned for Login
        if(rfid.isCard()){
            
            // Prints on Serial the NFC Card Number Sequence
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
                
                //for(int x = 0; x < sizeof(cards); x++){
                for(int x = 0; x < sizeof(cards) / sizeof(cards[0]); x++){
                  for(int i = 0; i < sizeof(rfid.serNum) / sizeof(rfid.serNum[0]); i++ ){
                      // IF NOT in Registered NFC's
                      if(rfid.serNum[i] != cards[x][i]) {
                          access = false;
                          access1 = false;
                          break;
                      // ELSE it is ACCEPTED
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
                          //t.tone(buzzPin, 1000, 150);
                          //digitalWrite(greenLED, LOW);
                          //delay(150);
                      }
                  }
                  if(access) break;
                }
              
            }

          // Path taken when NFC 
          while(access == true){
              distance = sonar.ping_cm();
              if (distance >= 15) {
                sensor = false;
                access = false;
                access1 = true;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Please be Seated!");
                lcd.setCursor(0, 1);
                lcd.print("Appropriately!");
                break;
              }

              // NFC Tags
              int cards1[][5] = {{119, 26, 232, 57, 188}};
              int cards2[][5] = {{234, 108, 21, 11, 152}};
              //int cards1[][5] = {{136,4,10,141,11},{136,4,139,162,165},{136,4,153,119,98},{136,4,136,75,79}};


              //Serial.println(count);
              //  Serial.println("---------- Welcome to Deskacito! ----------");
              //  Serial.println("<------ LEFT  OR  RIGHT ------>");
              //Serial.println("(Not Luis Fonsi Intended)");
              digitalWrite(greenLED, HIGH);
              digitalWrite(redLED, LOW);
              s1.write(0);
              s2.write(0);
              lcd.clear();
              lcd.setCursor(3, 0);
              lcd.print("Welcome to");
              lcd.setCursor(3, 1);
              lcd.print("Deskacito!");
              delay(1000);
              lcd.clear();
              lcd.print("<----- Left");
              lcd.setCursor(4, 1);
              lcd.print("Right ----->");
              delay(1000);

              for(int x = 0; x < sizeof(cards1) / sizeof(cards1[0]); x++){
                  for(int i = 0; i < sizeof(rfid.serNum) / sizeof(rfid.serNum[0]); i++ ){
                    if(rfid.serNum[i] != cards1[x][i]){
                      s1.write(180);
                      delay(1000);
                      s1.write(0);
                      //break;
                    }
                  }
                  //if (rfid.serNum[i] == cards1[x][i]) break;
                }

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

                  //for(int x = 0; x < sizeof(cards1) / sizeof(cards1[0]) / x < sizeof(cards2) / sizeof(cards2[0]); x++){
                    //for(int i = 0; i < sizeof(rfid.serNum) / sizeof(rfid.serNum[0]); i++ ){
                      //if(rfid.serNum[i] != cards1[x][i] && rfid.serNum != cards2[x][i]) {

                      //} else {}
                    //}
                  //}
                
                  for(int x = 0; x < sizeof(cards1) / sizeof(cards1[0]) && x < sizeof(cards2) / sizeof(cards2[0]); x++){
                    for(int i = 0; i < sizeof(rfid.serNum) / sizeof(rfid.serNum[0]); i++ ){
                      if(rfid.serNum[i] != cards1[x][i] && rfid.serNum != cards2[x][i]) {
                        access = false;
                        access1 = true;
                        s1.write(0);
                        s2.write(0);
                        //Serial.println("Logout!");
                        lcd.clear();
                        lcd.setCursor(1, 0);
                        lcd.print("Logging out...");
                        lcd.setCursor(1, 1);
                        lcd.print("See You Later!");
                        t.tone(buzzPin, 600, 100);
                        digitalWrite(redLED, HIGH);
                        delay(100);
                        t.tone(buzzPin, 1200, 100);
                        digitalWrite(redLED, LOW); 
                        delay(100);
                        //access = false;
                        break;
                      // ELSE Remain LOGGED IN
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
                if (signal.equals("bottle") && last_object != "bottle") {
                  s1.write(180);
                  s2.write(0);
                  lcd.clear();
                  lcd.print("Object:");
                  lcd.setCursor(0, 1);
                  lcd.print("Bottle");
                  //Serial.println("Bottle");
                  digitalWrite(greenLED, HIGH);
                  delay(500);
                  digitalWrite(greenLED, LOW);
                  delay(500);
                  digitalWrite(greenLED, HIGH);
                  delay(5000);
                  s1.write(0);
                  last_object = "bottle";

                  while (last_object == "bottle") {
                    if (count == 5) {
                      count = 0;
                      Serial.println("Count Reset");
                      last_object = " ";
                    } else {
                      count += 1;
                      Serial.print(count);
                    }
                  }
                }

                // Open RIGHT Compartment 
                else if (signal.equals("scissors") && last_object != "scissors") {
                  s1.write(0);
                  s2.write(180);
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
                  s2.write(0);
                  last_object = "scissors";

                  while (last_object == "scissors") {
                    if (count == 5) {
                      count = 0;
                      Serial.println("Count Reset");
                      last_object = " ";
                    } else {
                      count += 1;
                      Serial.print(count);
                    }
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
                  delay(500);
                  last_object = "none";
                  
                  while (last_object == "none") {
                    if (count == 5) {
                      count = 0;
                      Serial.println("Count Reset");
                      last_object = " ";
                    } else {
                      count += 1;
                      Serial.print(count);
                    }
                  }
                }


                //if (count == 5) {
                  //count = 0;
                  //Serial.println("Count Reset");
                  //last_object = " ";
                //} else {
                  //count += 1;
                  //Serial.print(count);
                //}
                //delay(1000);
              }


    /*
    * Valid card : Switch ON the LED for 1000 ms (1 second)
    */
              //digitalWrite(greenLED, HIGH); 
              //delay(500);
    /*
    * Valid card : Switch ON the POWER PIN for 2000 ms (2 seconds)), the POWER PIN can activate for example a relais, controlling a doorlock)
    */           
              //digitalWrite(power, HIGH);
              //delay(2000);
              //digitalWrite(power, LOW);
              //digitalWrite(greenLED, LOW);
              //delay(500);
              //digitalWrite(greenLED, HIGH);
              //delay(500);
              //digitalWrite(greenLED, LOW);
              //access = false;
              //delay(4000);
          
          // IF NFC Tag is NOT Registered
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
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please be Seated!");
      lcd.setCursor(0, 1);
      lcd.print("Appropriately!");
      delay(500);
    }
}
