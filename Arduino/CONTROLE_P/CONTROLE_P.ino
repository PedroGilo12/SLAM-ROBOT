#include "Adafruit_VL53L0X.h"

#define MDF 10
#define MDT 11

#define MEF 9
#define MET 6

int setpoint = 200;

int Kp = 2;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int Atuadores(int pot) {

  if (pot > 255) {
    pot = 255;
  }

  if (pot < -255) {
    pot = -255;
  }

  if (pot > 0) {
    analogWrite(MDF, pot);
    analogWrite(MDT, 0);
    analogWrite(MEF, pot);
    analogWrite(MET, 0);
  }

  if (pot < 0) {
    analogWrite(MDF, 0);
    analogWrite(MDT, pot * -1);
    analogWrite(MEF, 0);
    analogWrite(MET, pot * -1);
  }



}

void setup() {
  Serial.begin(115200);

  pinMode(MDF, OUTPUT);
  pinMode(MDT, OUTPUT);
  pinMode(MEF, OUTPUT);
  pinMode(MET, OUTPUT);


  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  // power
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
}


void loop() {
  VL53L0X_RangingMeasurementData_t measure;

  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    int distancia = measure.RangeMilliMeter;

    int erro = distancia - setpoint;

    int pot = erro*Kp;

    Atuadores(pot);

    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
    analogWrite(MDF, 0);
    analogWrite(MDT, 0);
    analogWrite(MEF, 0);
    analogWrite(MET, 0);
  }

  delay(100);
}
