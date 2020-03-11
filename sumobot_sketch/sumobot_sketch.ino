#include <Servo.h>

Servo servoLeft;
Servo servoRight;
//#define calibration

int motorStopL = 1490; // Motor stop uS
int motorStopR = 1500; // Motor stop uS
int motorSpeedL = 25; // Constant fwd speed
int motorSpeedR = 25; // Constant fwd speed
int errorMulti = 100; // Error multiplier
int speedLeft = motorStopL + motorSpeedL; // Left constant speed
int speedRight = motorStopR - motorSpeedR; // Right constant speed
int prevError = 0;

const int numSensors = 8;
const char sensors[numSensors] = {A0,A1,A2,A3,A4,A5,A6,A7};
int sensorValues[numSensors] = {0};
uint8_t sensorBuffer[numSensors+1] = {0};

int myTimeout = 300;

void setup() {
  Serial.begin(115200); // opens serial port, sets data rate to 115200 bps
  Serial.setTimeout(myTimeout);
  //Serial.print("Arduino is ready \n");
  servoLeft.attach(9);
  servoRight.attach(6);
  servoLeft.writeMicroseconds(speedLeft);
  servoRight.writeMicroseconds(speedRight);
  }

int startIndex = 10;
const int numSpdCmnd = 4;

void loop() {
  while(Serial.available() > 0){
    int16_t speedCommand[numSpdCmnd] = {0,0,0,0};
    Serial.readBytes((uint8_t *)&speedCommand, sizeof(speedCommand));
    for (int i =0; i < numSpdCmnd; i++){
      if(speedCommand[i] == 32767){startIndex = i+1;}
    }
    servoLeft.writeMicroseconds(speedLeft + (int16_t)speedCommand[(startIndex + 0) % numSpdCmnd]); // + (int16_t)speedCommand[(startIndex + 1) % numSpdCmnd]);
    servoRight.writeMicroseconds(speedRight + (int16_t)speedCommand[(startIndex + 0) % numSpdCmnd]); // + (int16_t)speedCommand[(startIndex + 2) % numSpdCmnd]);
    sensorBuffer[8]= 0;
    for (int i=0; i < numSensors; i++){
      sensorValues[i] = analogRead(sensors[i]);
      sensorBuffer[i] = map(sensorValues[i], 0, 1023, 0, 255);
      //Serial.println(sensorBuffer[i]);
    }
    Serial.write((uint8_t *)&sensorBuffer, sizeof(sensorBuffer));
    //Serial.write((uint8_t *)&speedCommand, sizeof(speedCommand));
    //Serial.println(" ");
  }
}
