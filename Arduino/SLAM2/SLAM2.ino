#include <SoftwareSerial.h>
#include "Adafruit_VL53L0X.h"
#include <math.h>

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

int ang = 0;
int x = 2;

int count = 0;

int setpoint_ang = 180;
int erro_ang = 0;
float Kp_ang = 3; //4.5;
float Ki_ang = 0.03;//0.0075;//;

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

float calc_w(int wheel, int ang0) {
  float wr = 0;
  float wl = 0;
  float l = 0.071;
  float r = 0.061;
  float w = 0;

  Serial.println("W0 = " + (String)ang0);

  if (wheel == 0) {
    int t0 = millis();
    while ((millis() - t0) < 1000) {
      analogWrite(MDF, 255);
    }
    analogWrite(MDF, 0);

    for (int k = 0; k < 20; k++) {
      if (mySerial.available()) {
        w = mySerial.parseInt();

        Serial.println("W = " + (String)w);
      }
    }

    Serial.println("W = " + (String)w);


    w = (w- ang0) * (PI/180);

    wr = (2 * l * w) / r;

    return wr;

  } else

    if (wheel == 1) {
      int t0 = millis();

      while ((millis() - t0) < 1000) {
        analogWrite(MEF, 255);
      }
      analogWrite(MEF, 0);

      for (int k = 0; k < 20; k++) {
        if (mySerial.available()) {
          w = mySerial.parseInt();
        }
      }

      Serial.println("W = " + (String)w);

      w = (ang0 - w) * (PI/180);

      wl = (2 * l * w) / r;

      return wl;

    }




}


void setup() {

  pinMode(MDF, OUTPUT);
  pinMode(MDT, OUTPUT);
  pinMode(MEF, OUTPUT);
  pinMode(MET, OUTPUT);
  pinMode(13, OUTPUT);

  Serial.begin(57600);
  while (!Serial);
  mySerial.begin(9600);

  if (!lox.begin()) {
    digitalWrite(13, 1);
    while (1);
  }

  digitalWrite(13, 0);


  delay(1000);

  while (ang == 0) {
    if (mySerial.available()) {
      ang = mySerial.parseInt();
    }
  }

  setpoint_ang = ang;

  float wr = calc_w(0, ang);

  ang = 0;
  while (ang == 0) {
    if (mySerial.available()) {
      ang = mySerial.parseInt();
    }
  }

  float wl = calc_w(1, ang);

  Serial.println("wr: " + (String)wr + " wl: " + (String)wl);

}

void loop() { // run over and over



}
