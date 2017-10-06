//#include <AS5147P.h>

#include <SPI.h>
#include "Encoder.h"
#include "TimerOne.h"
#include "controller.h"
#include "setPWMFrequency.h"
#include <Arduino.h>

#define DEG2ENC 16384.0/360.0
#define ENC2DEG 360.0/16384.0
#define P_MOTOR1 5
#define P_MOTOR2 6
#define KP -0.05
#define KD 0
#define KI 0

float enc1 = 0;
float enc2 = 0;
float cmd1 = 0.0;
float cmd2 = 0.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(500000);
  initEncoders();
  clearEncoderCount();
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  setPwmFrequency(P_MOTOR1, 1024);
  setPwmFrequency(P_MOTOR2, 1024);
}

void loop() {
  enc1 = readAS5147P(1)*ENC2DEG;
  enc2 = readAS5147P(2)*ENC2DEG;
  cmd1 = KP*(180-enc1);//centiamps
  cmd2 = KP*(180-enc2);//centiamps

  //0 to 255 scales from 0 to 100% duty cycle
  //50% duty cycle+- 50% duty cycle corresponds to Commands of -1000 to 1000
  //Commands of -1000 to 1000 correspond to +- 75 amps
//  analogWrite(P_MOTOR1, map(cmd1, -750, 750, 2,253));
//  analogWrite(P_MOTOR2, map(cmd2, -750, 750, 2,253));
  analogWrite(P_MOTOR1, map(cmd1, -180, 180, 2,253));
  analogWrite(P_MOTOR2, map(cmd2, -180, 180, 2,253));
  
  delay(1);
  Serial.print(enc1);
  Serial.print(" ");
  Serial.print(enc2);
  Serial.print(" ");
  Serial.print(cmd1);
  Serial.print(" ");
  Serial.println(cmd2);
}

