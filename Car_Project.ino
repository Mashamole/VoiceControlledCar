
#include <SoftwareSerial.h>
#include <Servo.h>
#include "VoiceRecognitionV3.h" //Downloaded library from elechouse



/**
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
**/

VR myVR(2, 3);   // 2:RX 3:TX, chosen pins on arduino

uint8_t records[7]; // save record
uint8_t buf[64];


int ret;
int trigPin = 12;
int echoPin = 13;
int led = 4;

Servo LeftMotor;
Servo RightMotor;


#define Back  (1)
#define Right   (3)
#define Left (2)
#define Forward (0)
#define Stop (4)
#define Thirty (5)



/**
  @brief   Print signature, if the character is invisible,
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  int i;
  for (i = 0; i < len; i++) {
    if (buf[i] > 0x19 && buf[i] < 0x7F) {
      Serial.write(buf[i]);
    }
    else {
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

/**
  @brief   Print signature, if the character is invisible,
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized.
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if (buf[0] == 0xFF) {
    Serial.print("NONE");
  }
  else if (buf[0] & 0x80) {
    Serial.print("UG ");
    Serial.print(buf[0] & (~0x80), DEC);
  }
  else {
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if (buf[3] > 0) {
    printSignature(buf + 4, buf[3]);
  }
  else {
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}


/*
  Function For detecting object by
  UltraSonic sensor
*/
int ObjectSensor() {

  long duration, distance;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  delay(10);

  if ((distance <= 10))
  {
    digitalWrite(led, HIGH);
    return 1;
  }
  else if (distance > 10)
  {
    digitalWrite(led, LOW);
    return 0;
  }
  return 0;
}






void setup()
{
  /** initialize */
  myVR.begin(9600);

  Serial.begin(115200);


  LeftMotor.attach(8);
  RightMotor.attach(9);

  pinMode(led, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  /*
     Detects if voice module is functioning and connected
  */
  if (myVR.clear() == 0) {
    Serial.println("Recognizer cleared.");
  } else {
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while (1);
  }

  if (myVR.load((uint8_t)Forward) >= 0) {
    Serial.println("Forward");

  }

  if (myVR.load((uint8_t)Back) >= 0) {
    Serial.println("Back");

  }

  if (myVR.load((uint8_t)Right) >= 0) {
    Serial.println("Right");

  }

  if (myVR.load((uint8_t)Left) >= 0) {
    Serial.println("Left");

  }

  if (myVR.load((uint8_t)Stop) >= 0) {
    Serial.println("Stop");

  }
  if (myVR.load((uint8_t)Thirty) >= 0) {
    Serial.println("Thirty");

  }
}

/*
  [Note]
  
  For moving sirvo motors:
  (eg)--> RightMotor.write(90); //sirvo do not move
  Numbers greater than 90 moves sirvo forward
  Numbers less than 90 moves sirvo backwards

*/

void loop()
{

  ret = myVR.recognize(buf, 50); //records response from voice module

  if (ObjectSensor() > 0 ) { //For detects object 10cm in direction

    LeftMotor.write(60);
    RightMotor.write(60);
    delay(550);
    RightMotor.write(60);
    LeftMotor.write(120);
    delay(500);
    RightMotor.write(90);
    LeftMotor.write(90);

  }

  if (ret > 0) { //checks if response is valid which needs to be greater than 0


    if (buf[1] == Forward) { //Performs forward movement
      /* if (ObjectSensor() > 0) {
         LeftMotor.write(90);
         RightMotor.write(90);
        }
      */
      RightMotor.write(120);
      LeftMotor.write(120);
      if (ObjectSensor() > 0 ) {

        LeftMotor.write(60);
        RightMotor.write(60);
        delay(550);
        RightMotor.write(60);
        LeftMotor.write(120);
        delay(500);
        RightMotor.write(90);
        LeftMotor.write(90);

      }
      delay(1500);
      LeftMotor.write(90);
      RightMotor.write(90);
      Serial.println("Forward");

    }
    if (buf[1] == Back) { //Performs Backwards movement

      RightMotor.write(60);
      LeftMotor.write(60);
      if (ObjectSensor() > 0 ) {

        LeftMotor.write(60);
        RightMotor.write(60);
        delay(550);
        RightMotor.write(60);
        LeftMotor.write(120);
        delay(500);
        RightMotor.write(90);
        LeftMotor.write(90);

      }
      delay(1000);
      LeftMotor.write(90);
      RightMotor.write(90);
      Serial.println("Back");

    }
    if (buf[1] == Left) { //Performs Leftward movement


      RightMotor.write(120);
      LeftMotor.write(60);
      if (ObjectSensor() > 0 ) {

        LeftMotor.write(60);
        RightMotor.write(60);
        delay(550);
        RightMotor.write(60);
        LeftMotor.write(120);
        delay(500);
        RightMotor.write(90);
        LeftMotor.write(90);

      }
      delay(615);
      RightMotor.write(90);
      LeftMotor.write(90);
      Serial.println("Left");

    }

    if (buf[1] == Right) { //Performs Rightward movement

      RightMotor.write(60);
      LeftMotor.write(120);
      if (ObjectSensor() > 0 ) {

        LeftMotor.write(60);
        RightMotor.write(60);
        delay(550);
        RightMotor.write(60);
        LeftMotor.write(120);
        delay(500);
        RightMotor.write(90);
        LeftMotor.write(90);
      }
      delay(615);
      RightMotor.write(90);
      LeftMotor.write(90);
      Serial.println("Right;)");


    }
    if (buf[1] == Thirty) { //Performs movement 30 degrees right

      RightMotor.write(60);
      LeftMotor.write(120);
      if (ObjectSensor() > 0 ) {

        LeftMotor.write(60);
        RightMotor.write(60);
        delay(550);
        RightMotor.write(60);
        LeftMotor.write(120);
        delay(500);
        RightMotor.write(90);
        LeftMotor.write(90);

      }
      delay(350);
      RightMotor.write(90);
      LeftMotor.write(90);
      Serial.println("Thirty;)");


    }
    /** voice recognized **/
    printVR(buf);
  }


}
