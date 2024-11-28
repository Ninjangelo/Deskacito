


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

// LIBRARIES
#include <SPI.h>
#include <RFID.h>
#include <Servo.h>
#include <Wire.h>
#include "rgb_lcd.h"

// LCD
rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

// NFC-RFID and Servos
#define SS_PIN 10
#define RST_PIN 9
//#define SS_PIN 30
//#define RST_PIN 29

RFID rfid(SS_PIN,RST_PIN);


int power = 7;
int redLED = 8;
int greenLED = 5;
int serNum[5];
Servo s1;
Servo s2;


/*
* This integer should be the code of Your Mifare card / tag 
*/
int cards[][5] = {{170,41,217,37,127}};

bool access = false;

void setup(){

    Serial.begin(9600);
    SPI.begin();
    rfid.init();
/*
* define VMA100 (UNO) pins 7 & 8 as outputs and put them LOW
*/
    pinMode(redLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    pinMode (power,OUTPUT);
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, LOW);
    digitalWrite (power,LOW);

    s1.attach(6);
    s2.attach(4);
    s1.write(0);
    s2.write(0);

    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);

    //lcd.print("Please Scan");
    //lcd.setCursor(0, 1);
    //lcd.print("NFC");
    //delay(1000);
   
}

void loop(){
    lcd.clear();
    lcd.print("Please Scan");
    lcd.setCursor(0, 1);
    lcd.print("NFC");
    
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
            
            for(int x = 0; x < sizeof(cards); x++){
              for(int i = 0; i < sizeof(rfid.serNum); i++ ){
                  if(rfid.serNum[i] != cards[x][i]) {
                      access = false;
                      break;
                  } else {
                      access = true;
                  }
              }
              if(access) break;
            }
           
        }
        
       if(access){
           Serial.println("<<<<<<<<<< Welcome to Deskacito! >>>>>>>>>>");
           Serial.println("(Not Luis Fonsi Intended)");
           lcd.clear();
           lcd.print("Welcome to");
           lcd.setCursor(0, 1);
           lcd.print("Deskacito!");
           s1.write(360);
           s2.write(360);

/*
* Valid card : Switch ON the LED for 1000 ms (1 second)
*/
           digitalWrite(greenLED, HIGH); 
           delay(1000);
/*
* Valid card : Switch ON the POWER PIN for 2000 ms (2 seconds)), the POWER PIN can activate for example a relais, controlling a doorlock)
*/           
           digitalWrite(power, HIGH);
           delay(2000);
           digitalWrite(power, LOW);
           digitalWrite(greenLED, LOW);
           access = false;
           
      } else {
/*
* NON-Valid card : switch ON and OFF the LED twice for 0,5 seconds
*/
           Serial.println("Access Denied!");
           lcd.clear();
           lcd.print("Access");
           lcd.setCursor(0, 1);
           lcd.print("Denied!");
           s1.write(0);
           s2.write(0);
           digitalWrite(redLED, HIGH);
           delay(500);
           digitalWrite(redLED, LOW); 
           delay(500);
           digitalWrite(redLED, HIGH);
           delay(500);
           digitalWrite(redLED, LOW);    
       }        
    }
    
    
    
    rfid.halt();

}