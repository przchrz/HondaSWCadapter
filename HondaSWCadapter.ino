// Steering wheel controll

#include "SPI.h" // necessary library
int ss=10; // using digital pin 10 for SPI slave select
int del=200; // used for various delays

// float Vin = 5.1;       //value of voltage from arduino - not used

float R_REF_1 = 9960;     //value of known resistance for TIP
float R_REF_2 = 9970;     //value of known resistance for RING
float R_REF_3 = 9990;     //value of known resistance for HFT

float R_TIP = 0;          //value of unknown resistance for TIP
float R_RING = 0;         //value of unknown resistance for RING
float R_HFT = 0;          //value of unknown resistance for HFT

float input_TIP = 0;
float input_RING = 0;
float input_HFT = 0;



void setup() {

  pinMode(2, OUTPUT); // ring
  digitalWrite(2, LOW); //Not connected
  //digitalWrite(2, HIGH); //Connected

  pinMode(3, OUTPUT); // tip
  digitalWrite(3, LOW); //Not connected
  //digitalWrite(3, HIGH); //Connected
  
  pinMode(ss, OUTPUT); // we use this for SS pin
  SPI.begin(); // wake up the SPI bus.
  SPI.setBitOrder(MSBFIRST);
  // our MCP4162 requires data to be sent MSB (most significant byte) first

  //setValue(4); //Mode 1.816k
  //setValue(22); //CH+ 8.71k
  //setValue(33); //CH- 12.89k
  //setValue(47); //Vol+ 18.30k
  //setValue(63); //Vol- 24.44k

  //setValue(8); //Answer 3.337k
  setValue(14); //Hang-up 5.67k

  // only for debugging
  Serial.begin(9600);

  // prepare analog inputs for SWC
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

}

void loop() {
  
  input_TIP = analogRead(A0);
  input_RING = analogRead(A1);
  input_HFT = analogRead(A2);

  R_TIP = (R_REF_1 * input_TIP) / (1023 - input_TIP);
  R_RING = (R_REF_2 * input_RING) / (1023 - input_RING);
  R_HFT = (R_REF_3 * input_HFT) / (1023 - input_HFT);

  Serial.print("R Tip: ");
  Serial.println(R_TIP);
  Serial.print("R Ring: ");
  Serial.println(R_RING);
  Serial.print("R HFT: ");
  Serial.println(R_HFT);
  Serial.println("------------");

  // Ground - 0
  // Open circuit - 1023


  delay(500);
  
}

void setValue(int value)
{
  digitalWrite(ss, LOW);
  SPI.transfer(0); // send command byte
  SPI.transfer(value); // send value (0~255)
  digitalWrite(ss, HIGH);
}
