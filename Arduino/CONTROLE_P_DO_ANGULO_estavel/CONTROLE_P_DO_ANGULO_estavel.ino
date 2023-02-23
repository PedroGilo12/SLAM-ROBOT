#include <SoftwareSerial.h>

#define MDF 10
#define MDT 11
#define MEF 9
#define MET 6

SoftwareSerial mySerial(2, 3); // RX, TX

int ang;
int x = 2;

int setpoint_ang = 70;
int erro_ang;
int Kp_ang = 4;

void setup() {

  pinMode(MDF, OUTPUT);
  pinMode(MDT, OUTPUT);
  pinMode(MEF, OUTPUT);
  pinMode(MET, OUTPUT);

  Serial.begin(57600);
  while (!Serial) {
    ;
  }
  mySerial.begin(9600);
}

void loop() { // run over and over
  if (mySerial.available()) {
    ang = mySerial.parseInt();
  }

  erro_ang = setpoint_ang - ang;
  int Uang = Kp_ang * erro_ang;

  if(Uang > 255){
    Uang = 255;
  }

  if(Uang < -255) {
    Uang = -255;
  }

  if (Uang < 0) {
    analogWrite(MDF, Uang*-1);
    analogWrite(MDT, 0);
    analogWrite(MEF, 0);
    analogWrite(MET, Uang*-1);
  } else {
    analogWrite(MDF, 0);
    analogWrite(MDT, Uang);
    analogWrite(MEF, Uang);
    analogWrite(MET, 0);
  }



  Serial.println("setpoint " + (String)setpoint_ang + "\t" + "angulo " + (String)ang + "\t" + "Uang " + (String)Uang);

}
