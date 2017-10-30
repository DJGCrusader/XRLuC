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
#define KP .30
#define KD 0.01
#define KI 0
#define FREQ 100.0
#define PERIOD 1.0/FREQ

#define H1A A0
#define H1B A1
#define H1C A2
#define H2A A3
#define H2B A4
#define H2C A5

#define OFF1 0
#define OFF2 0
#define ENC2TICK (16384.0)/90.0//((8192)/45)

float enc1 = 0;
float enc1Prev = 0;
float enc2 = 0;
float enc2Prev = 0;
float dEnc1dt = 0;
float dEnc2dt = 0;

float cmd1 = 0.0;
float cmd2 = 0.0;
double t = 0; 
double dt = 0;
double tPrev = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(500000);
  initEncoders();
  clearEncoderCount();
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(H1A, OUTPUT);
  pinMode(H1B, OUTPUT);
  pinMode(H1C, OUTPUT);
  pinMode(H2A, OUTPUT);
  pinMode(H2B, OUTPUT);
  pinMode(H2C, OUTPUT);
  
  setPwmFrequency(P_MOTOR1, 1024);
  setPwmFrequency(P_MOTOR2, 1024);
  
  tPrev = micros()/100000.0;
  
  enc1 = readAS5147P(1)*ENC2DEG;
  enc2 = readAS5147P(2)*ENC2DEG;
  enc1Prev = enc1;
  enc2Prev = enc2;

  //setHalls(enc1, OFF1, ENC2TICK, H1A. H1B, H1C);
  setHalls(enc2, OFF2, ENC2TICK, H2A, H2B, H2C);
}

void loop() {
  t = micros();
  t = t/100000.0;
  dt = t - tPrev;
  
  if(dt>=PERIOD){
    tPrev = t;
    if(dt>1.5*PERIOD){
      Serial.println("SLOW");
    }
    enc1 = readAS5147P(1);
    enc2 = readAS5147P(2);
    
    setHalls(enc1, OFF1, ENC2TICK,H1A, H1B, H1C);
    //setHalls(enc2, OFF2, ENC2TICK,H2A, H2B, H2C);
    enc1 = enc1*ENC2DEG;
    enc2 = enc2*ENC2DEG;
  
    dEnc1dt = (enc1-enc1Prev)/dt;
    dEnc2dt = (enc2-enc2Prev)/dt;
    cmd1 = -KP*(180-45-45*sin(t*1*(2*PI))-enc1)+KD*dEnc1dt;//centiamps
    cmd2 = -KP*(180+45+45*sin(t*1*(2*PI))-enc2)+KD*dEnc2dt;//centiamps
  
    //0 to 255 scales from 0 to 100% duty cycle
    //50% duty cycle+- 50% duty cycle corresponds to Commands of -1000 to 1000
    //Commands of -1000 to 1000 correspond to +- 75 amps
    //  analogWrite(P_MOTOR1, map(cmd1, -750, 750, 2,253));
    //  analogWrite(P_MOTOR2, map(cmd2, -750, 750, 2,253));
    if(t < 2){      
      analogWrite(P_MOTOR1, map(0, -180, 180, 2,253));
      analogWrite(P_MOTOR2, map(0, -180, 180, 2,253));
    }else{      
      analogWrite(P_MOTOR1, map(cmd1, -180, 180, 2,253));
      analogWrite(P_MOTOR2, map(cmd2, -180, 180, 2,253));
    }
    
//    Serial.print(enc1);
//    Serial.print(" ");
//    Serial.print(enc2);
//    Serial.print(" ");
    //Serial.print(dEnc1dt);
    //Serial.print(" ");
    //Serial.print(dEnc2dt);
    //Serial.print(" ");
//    Serial.print(cmd1);
//    Serial.print(" ");
//    Serial.print(cmd2);
    Serial.println("");
    enc1Prev = enc1;
    enc2Prev = enc2;
  }
}

