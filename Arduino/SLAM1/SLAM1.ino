#include <SoftwareSerial.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

#define MDF 10
#define MDT 11
#define MEF 9
#define MET 6

SoftwareSerial mySerial(2, 3); // RX, TX

int pMDF, pMDT, pMEF, pMET;

int flag_ang = 0;
int fv_ang = 0;
int i_ang = 0;
int v_ant_ang = 0;

int ang;
int x = 2;

int setpoint_ang = 180;
int erro_ang = 0;
float Kp_ang = 2.5; //4.5;
float Ki_ang = 0.05;//0.0075;//;



int setpoint_dist = 200;
int erro_dist;
float Kp_dist = 0;
float Ki_dist = 0;

int f = 0.75;

int controle_ang() {

  /////////////////////////////////////// calculo da integral /////////////////////////////////////////////

  if (flag_ang == 1000) {
    i_ang = i_ang - v_ant_ang;
    v_ant_ang = erro_ang;
    flag_ang--;
  } else {
    flag_ang++;
  }

  i_ang = i_ang + erro_ang;




  ////////////////////////////////////////////////////////////////////////////////////////////////////////


  float Ukp = Kp_ang * erro_ang;
  float Uki = Ki_ang * i_ang;

  //Serial.print("Uki: " + (String)Uki + " ");

  if (Uki > 255) {
    Uki = 255;
  }

  if (Uki < -255) {
    Uki = -255;
  }

  int Uang = Ukp + Uki;

  if (Uang > 255) {
    Uang = 255;
  }

  if (Uang < -255) {
    Uang = -255;
  }

  return Uang;
}

int Atuadores(int pot, int uang) {

  uang = uang * f;

  if (pot > 255) {
    pot = 255;
  }

  if (pot < -255) {
    pot = -255;
  }


  if ((pot >= 0) && (uang >= 0)) {
    analogWrite(MDF, pot);
    analogWrite(MDT, uang);
    analogWrite(MEF, pot + uang);
    analogWrite(MET, 0);
  }

  if ((pot >= 0) && (uang < 0)) {
    analogWrite(MDF, pot + (uang * -1));
    analogWrite(MDT, 0);
    analogWrite(MEF, pot);
    analogWrite(MET, uang * -1);
  }

  if ((pot < 0) && (uang < 0)) {
    analogWrite(MDF, uang * -1);
    analogWrite(MDT, pot * -1);
    analogWrite(MEF, 0);
    analogWrite(MET, (pot * -1) + uang);
  }

  if ((pot < 0) && (uang >= 0)) {
    analogWrite(MDF, 0);
    analogWrite(MDT, (pot * -1) + uang);
    analogWrite(MEF, uang);
    analogWrite(MET, pot * -1);
  }

}

int count = 0;

void setup() {



  pinMode(MDF, OUTPUT);
  pinMode(MDT, OUTPUT);
  pinMode(MEF, OUTPUT);
  pinMode(MET, OUTPUT);
  pinMode(13, OUTPUT);

  Serial.begin(57600);
  while (!Serial) {
    ;
  }
  mySerial.begin(9600);

  if (!lox.begin()) {
    digitalWrite(13, 1);
    while (1);
  }
  digitalWrite(13, 0);

  //ang = setpoint_ang;


}

void loop() {

  count++;
  if (mySerial.available()) {
    ang = mySerial.parseInt();
  }

  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    int distancia = measure.RangeMilliMeter;
    Serial.println("{\"d\":" + (String)distancia + ",\"a\":" + (String)ang + "}");
  }



  analogWrite(MDF, 180);
  analogWrite(MDT, 0);
  analogWrite(MEF, 0);
  analogWrite(MET, 180);

  delay(50);

  analogWrite(MDF, 0);
  analogWrite(MDT, 0);
  analogWrite(MEF, 0);
  analogWrite(MET, 0);

  delay(950);


}
