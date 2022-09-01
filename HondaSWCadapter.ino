// Steering wheel control v1.1

// float Vin = 5.1;       //value of voltage from arduino - not used

float R_REF_HFT = 9970;     //value of known resistance for A0 - HFT SWC
float R_REF_Tip = 9970;     //value of known resistance for A1 - Tip Adapter
float R_REF_Ring = 9980;     //value of known resistance for A2 - Ring Adapter

float R_TIP = 0;          //value of unknown resistance for TIP
float R_RING = 0;         //value of unknown resistance for RING
float R_HFT = 0;          //value of unknown resistance for HFT

float input_TIP = 0;
float input_RING = 0;
float input_HFT = 0;

// Values for Pioneer Adapter
//float R_MODE_min = 1200; //1821.11
//float R_MODE_max = 2200;
//float R_VolPlus_min = 17700; //18204.89
//float R_VolPlus_max = 18700;
//float R_VolMinus_min = 23500; //24027.70
//float R_VolMinus_max = 24500;
//float R_ChPlus_min = 8200; //8778.73
//float R_ChPlus_max = 9200;
//float R_ChMinus_min = 12200; //12796.32
//float R_ChMinus_max = 13200;

// Values for Honda SWC
float R_MODE_min = 3500; //3702.00; 
float R_MODE_max = 4000; //3683.69;
float R_VolPlus_min = 300; //342.75; 
float R_VolPlus_max = 400; //332.33;
float R_VolMinus_min = 50; //78.58; 
float R_VolMinus_max = 140; //98.42;
float R_ChPlus_min = 1500; //1673.05; 
float R_ChPlus_max = 1800; //1686.35;
float R_ChMinus_min = 700; //754.83; 
float R_ChMinus_max = 800; //766.12;

float R_Talk_min = 100; //158
float R_Talk_max = 250; //168
float R_Back_min = 5; //20
float R_Back_max = 80; //40
float R_HFT_notPressed_min = 900;
float R_HFT_notPressed_max = 1100;

// to avoid measurment mistakes
float TIP_threshold = 3;

int answer_pushed = 0;
int answer_long_pushed = 0;
float time_answer = 0;
float answer_long_press_duration = 2000; //2s.

int debug = 0;

int delayTime = 50;

void setup() {
  // only for debugging
  if(debug == 1){
    Serial.begin(9600);
  }

  // prepare analog inputs for SWC
  pinMode(A0, INPUT); // HFT SWC
  pinMode(A1, INPUT); // Tip Adapter
  pinMode(A2, INPUT); // Ring Adapter

  // prepare digital outputs
  pinMode(2, OUTPUT); // Voice Control - 68k + Ring
  digitalWrite(2, LOW);
  pinMode(3, OUTPUT); // Decline Call - 5.6k + Ring
  digitalWrite(3, LOW);
  pinMode(4, OUTPUT); // Answer Call - 3k + Ring
  digitalWrite(4, LOW);
  pinMode(5, OUTPUT); // CH - - 12k
  digitalWrite(5, LOW);
  pinMode(6, OUTPUT); // CH + - 8.2k
  digitalWrite(6, LOW);
  pinMode(7, OUTPUT); // Vol - - 22k
  digitalWrite(7, LOW);
  pinMode(8, OUTPUT); // Vol + - 15k
  digitalWrite(8, LOW);
  pinMode(9, OUTPUT); // Mode - 1.2k
  digitalWrite(9, LOW);
}

void loop() {

  input_TIP = analogRead(A1);
  input_RING = analogRead(A2);
  input_HFT = analogRead(A0);
  // Ground - 0
  // Open circuit - 1023

  R_TIP = (R_REF_Tip * input_TIP) / (1023 - input_TIP);
  R_RING = (R_REF_Ring * input_RING) / (1023 - input_RING);
  R_HFT = (R_REF_HFT * input_HFT) / (1023 - input_HFT);

  if(debug == 1){
    Serial.print("R_TIP: ");
    Serial.print(R_TIP);
    Serial.print("; R_HFT: ");
    Serial.print(R_HFT);
    Serial.print("; input Tip: ");
    Serial.print(input_TIP);
    Serial.print("; input HFT: ");
    Serial.println(input_HFT);
  }

  if (input_TIP < 508 - TIP_threshold) {
    if (R_TIP > R_MODE_min && R_TIP < R_MODE_max) {
      answer_pushed = 0;
      answer_long_pushed = 0;
      if(debug == 1){
        Serial.println("Mode");
      }
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, HIGH);
    }
    else {
      if (R_TIP > R_VolPlus_min && R_TIP < R_VolPlus_max) {
        answer_pushed = 0;
        answer_long_pushed = 0;
        if(debug == 1){
          Serial.println("Vol Plus");
        }
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, LOW);
        digitalWrite(8, HIGH);
        digitalWrite(9, LOW);
      }
      else {
        if (R_TIP > R_VolMinus_min && R_TIP < R_VolMinus_max) {
          answer_pushed = 0;
          answer_long_pushed = 0;
          if(debug == 1){
            Serial.println("Vol Minus");
          }
          digitalWrite(2, LOW);
          digitalWrite(3, LOW);
          digitalWrite(4, LOW);
          digitalWrite(5, LOW);
          digitalWrite(6, LOW);
          digitalWrite(7, HIGH);
          digitalWrite(8, LOW);
          digitalWrite(9, LOW);
        }
        else {
          if (R_TIP > R_ChPlus_min && R_TIP < R_ChPlus_max) {
            answer_pushed = 0;
            answer_long_pushed = 0;
            if(debug == 1){
              Serial.println("CH Plus");
            }
            digitalWrite(2, LOW);
            digitalWrite(3, LOW);
            digitalWrite(4, LOW);
            digitalWrite(5, LOW);
            digitalWrite(6, HIGH);
            digitalWrite(7, LOW);
            digitalWrite(8, LOW);
            digitalWrite(9, LOW);
          }
          else {
            if (R_TIP > R_ChMinus_min && R_TIP < R_ChMinus_max) {
              answer_pushed = 0;
              answer_long_pushed = 0;
              if(debug == 1){
                Serial.println("CH Minus");
              }
              digitalWrite(2, LOW);
              digitalWrite(3, LOW);
              digitalWrite(4, LOW);
              digitalWrite(5, HIGH);
              digitalWrite(6, LOW);
              digitalWrite(7, LOW);
              digitalWrite(8, LOW);
              digitalWrite(9, LOW);
            }
          }
        }
      }
    }
  }
  else {
    if (R_HFT < R_HFT_notPressed_min && input_HFT < 91 - TIP_threshold) {
      if (R_HFT > R_Talk_min && R_HFT < R_Talk_max) {
        if(answer_pushed == 0){
          time_answer = millis();
          answer_pushed = 1;
        }

        if(millis()-time_answer < answer_long_press_duration){
          digitalWrite(2, LOW);
          digitalWrite(3, LOW);
          digitalWrite(4, HIGH);
          digitalWrite(5, LOW);
          digitalWrite(6, LOW);
          digitalWrite(7, LOW);
          digitalWrite(8, LOW);
          digitalWrite(9, LOW);
          if(debug == 1){
            Serial.println("Answer");
          }
        }
        else{
          if(answer_long_pushed == 0){
            digitalWrite(2, LOW);
            digitalWrite(3, LOW);
            digitalWrite(4, LOW);
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
            digitalWrite(7, LOW);
            digitalWrite(8, LOW);
            digitalWrite(9, LOW);
            delay(50);
            digitalWrite(2, HIGH);
            delay(300);
            digitalWrite(2, LOW);
            answer_long_pushed = 1;
            if(debug == 1){
              Serial.println("Voice Control");
            }
          }
          else{
            digitalWrite(2, LOW);
            digitalWrite(3, LOW);
            digitalWrite(4, LOW);
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
            digitalWrite(7, LOW);
            digitalWrite(8, LOW);
            digitalWrite(9, LOW);
            if(debug == 1){
              Serial.println("Answer - too long, voice control ended");
            }
          } 
        } 
      }
      else {
        if (R_HFT > R_Back_min && R_HFT < R_Back_max) {
          answer_pushed = 0;
          answer_long_pushed = 0;
          if(debug == 1){
            Serial.println("Hang-up");
          }
          digitalWrite(2, LOW);
          digitalWrite(3, HIGH);
          digitalWrite(4, LOW);
          digitalWrite(5, LOW);
          digitalWrite(6, LOW);
          digitalWrite(7, LOW);
          digitalWrite(8, LOW);
          digitalWrite(9, LOW);
        }
      }
    }
    else {
      answer_pushed = 0;
      answer_long_pushed = 0;
      if(debug == 1){
        Serial.println("----------");
      }
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
    }
  }

  delay(delayTime);

}
