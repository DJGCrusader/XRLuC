signed long enc = 0;
signed long error  = 0;
float eSum = 0;
float Kp = 5;
float Ki = 0.01;

float sp = 1000;
float umax = 1000;

float u = 0;
float controlEffort = 0;

float proportional = 0;
float integral = 0;


void pController(){
  
   enc = readEncoder(1)*(-1)*360/64;
   error = sp - enc;
   eSum = error + eSum;

  proportional = Kp*error;
  integral = Ki*(eSum + error);

  u = proportional + integral;
  
  controlEffort = round( min( abs(u),umax ) /umax*255);

  if (u<-0.001) {
    digitalWrite(4,HIGH);
  }
  else if (u>0.001){
    digitalWrite(4,LOW);
  }
  else {
    analogWrite(5,0);
    }

  analogWrite(5, controlEffort);  
}


float prevEnc = 0;
float spv = 0;
float Kpv = 5;
float Kiv = 0.01;
float vel = 0 ;


void vController(){
  enc = readEncoder(1);
  vel = enc-prevEnc;

  error = spv - vel;
  eSum = error + eSum;


  proportional = Kpv*error;
  integral = Kiv*(eSum + error);

  u = proportional + integral;
  
  controlEffort = round( min( abs(u),umax ) /umax*255);

  if (u<-0.001) {
    digitalWrite(4,HIGH);
  }
  else if (u>0.001){
    digitalWrite(4,LOW);
  }
  else {
    analogWrite(5,0);
    }

  analogWrite(5, controlEffort);


  
  
  
  
  prevEnc = enc;
  }
