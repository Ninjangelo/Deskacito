#include <SPI.h>
#include <RFID.h>
#include <Servo.h>

RFID rfid(10, 9);
unsigned char status;
unsigned char str[MAX_LEN];

String accessGranted [2] = {"310988016", "19612012715"};
int accessGrantedSize = 2;

Servo lockServo;
int lockPos = 15;
int unlockPos = 75;
boolean locked = true;

int redLEDPin = 5;
int greenLEDPin = 6;


void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  digitalWrite(redLEDPin, HIGH);
  delay(200);
  digitalWrite(greenLEDPin, HIGH);
  delay(200);
  digitalWrite(redLEDPin, LOW);
  delay(200);
  digitalWrite(greenLEDPin, LOW);
  lockServo.attach(3);
  lockServo.write(lockPos);
  Serial.println("Place card/tag near reader...");
}


void loop()
{
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)
  {
    Serial.println("Card found");
    String temp = "";
    if (rfid.anticoll(str) == MI_OK)
    {
      Serial.print("The card's ID number is : ");
      for (int i = 0; i < 4; i++)
      {
        temp = temp + (0x0F & (str[i] >> 4));
        temp = temp + (0x0F & (str[i]));
      }
      Serial.println (temp);
      checkAccess (temp);
    }
    rfid.selectTag(str);
  }
  rfid.halt();
}


void checkAccess (String temp)
{
  boolean granted = false;
  for (int i=0; i <= (accessGranted-1); i++)
  {
    if(accessGranted[i] == temp)
    {
      Serial.println ("Access Granted");
      granted = true;
      if (locked == true)
      {
        lockServo.write(unlockPos);
        locked = false;
      }
      else if (locked == false)
      {
        lockServo.write(lockPos);
        locked = true;
      }
      digitalWrite(greenLEDPin, HIGH);
      delay(200);
      digitalWrite(greenLEDPin, LOW);
      delay(200);
      digitalWrite(greenLEDPin, HIGH);
      delay(200);
      digitalWrite(greenLEDPin, LOW);
      delay(200);
    }
  }
  if (granted == false)
  {
    Serial.println ("Access Denied");
    digitalWrite(redLEDPin, HIGH);
    delay(200);
    digitalWrite(redLEDPin, LOW);
    delay(200);
    digitalWrite(redLEDPin, HIGH);
    delay(200);
    digitalWrite(redLEDPin, LOW);
    delay(200);
  }
}