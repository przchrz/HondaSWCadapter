// Steering wheel control

#include "SPI.h" // necessary library
int ss = 10; // using digital pin 10 for SPI slave select
int del = 200; // used for various delays

// float Vin = 5.1;       //value of voltage from arduino - not used

float R_REF_1 = 9960;     //value of known resistance for TIP
float R_REF_2 = 9970;     //value of known resistance for RING
float R_REF_3 = 9990;     //value of known resistance for HFT

float R_TIP = -1;          //value of unknown resistance for TIP
float R_RING = -1;         //value of unknown resistance for RING
float R_HFT = -1;          //value of unknown resistance for HFT

float R_TIP_prev = -1;
float R_RING_prev = -1;
float R_HFT_prev = -1;

float input_TIP = 0;
float input_RING = 0;
float input_HFT = 0;

float R_MODE_min = 1800;
float R_MODE_max = 1850;
float R_VolPlus_min = 18200;
float R_VolPlus_max = 18300;
float R_VolMinus_min = 24060;
float R_VolMinus_max = 24180;
float R_ChPlus_min = 8800;
float R_ChPlus_max = 8880;
float R_ChMinus_min = 12800;
float R_ChMinus_max = 12870;

float R_Talk_min = 170;
float R_Talk_max = 190;
float R_Back_min = 30;
float R_Back_max = 50;
float R_HFT_notPressed_min = 900;
float R_HFT_notPressed_max = 1100;

int delayTime = 10;

void setup() {

  pinMode(2, OUTPUT); // ring, yellow
  digitalWrite(2, LOW); //ring not connected

  pinMode(3, OUTPUT); // tip, red
  digitalWrite(3, LOW); //tip not connected

  pinMode(ss, OUTPUT); // we use this for SS pin
  SPI.begin(); // wake up the SPI bus.
  SPI.setBitOrder(MSBFIRST);
  // our MCP4162 requires data to be sent MSB (most significant byte) first

  // only for debugging
  Serial.begin(9600);

  // prepare analog inputs for SWC
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

}

void loop() {

  R_TIP_prev = R_TIP;
  R_RING_prev = R_RING;
  R_HFT_prev = R_HFT;

  input_TIP = analogRead(A0);
  input_RING = analogRead(A1);
  input_HFT = analogRead(A2);
  // Ground - 0
  // Open circuit - 1023

  R_TIP = (R_REF_1 * input_TIP) / (1023 - input_TIP);
  R_RING = (R_REF_2 * input_RING) / (1023 - input_RING);
  R_HFT = (R_REF_3 * input_HFT) / (1023 - input_HFT);

  if (input_TIP < 1023 && R_TIP != R_TIP_prev && R_TIP_prev != -1) {
    if (R_TIP > R_MODE_min && R_TIP < R_MODE_max) {
      setValue(4); //Mode 1.816k
      digitalWrite(3, HIGH); //tip connected
      digitalWrite(2, LOW); //ring not connected
    }
    else {
      if (R_TIP > R_VolPlus_min && R_TIP < R_VolPlus_max) {
        setValue(47); //Vol+ 18.30k
        digitalWrite(3, HIGH); //tip connected
        digitalWrite(2, LOW); //ring not connected
      }
      else {
        if (R_TIP > R_VolMinus_min && R_TIP < R_VolMinus_max) {
          setValue(63); //Vol- 24.44k
          digitalWrite(3, HIGH); //tip connected
          digitalWrite(2, LOW); //ring not connected
        }
        else {
          if (R_TIP > R_ChPlus_min && R_TIP < R_ChPlus_max) {
            setValue(22); //CH+ 8.71k
            digitalWrite(3, HIGH); //tip connected
            digitalWrite(2, LOW); //ring not connected
          }
          else {
            if (R_TIP > R_ChMinus_min && R_TIP < R_ChMinus_max) {
              setValue(33); //CH- 12.89k
              digitalWrite(3, HIGH); //tip connected
              digitalWrite(2, LOW); //ring not connected
            }
          }
        }
      }
    }
  }
  else {
    if (R_HFT < R_HFT_notPressed_min && R_HFT != R_HFT_prev && R_HFT_prev != -1) {
      if (R_HFT > R_Talk_min && R_HFT < R_Talk_max) {
        setValue(8); //Answer 3.337k
        digitalWrite(3, HIGH); //tip connected
        digitalWrite(2, HIGH); //ring connected
      }
      else {
        if (R_HFT > R_Back_min && R_HFT < R_Back_max) {
          setValue(14); //Hang-up 5.67k
          digitalWrite(3, HIGH); //tip connected
          digitalWrite(2, HIGH); //ring connected
        }
      }
    }
    else {
      digitalWrite(3, LOW); //tip not connected
      digitalWrite(2, LOW); //ring not connected
    }
  }

  // debug

  Serial.print("R Tip: ");
  Serial.println(R_TIP);
  Serial.print("R Ring: ");
  Serial.println(R_RING);
  Serial.print("R HFT: ");
  Serial.println(R_HFT);
  Serial.println("------------");

  // ------

  delay(delayTime);

}

void setValue(int value)
{
  digitalWrite(ss, LOW);
  SPI.transfer(0); // send command byte
  SPI.transfer(value); // send value (0~255)
  digitalWrite(ss, HIGH);
}
