#include "Arduino.h"
#include "ControllerMD.h"

SegaMegadrive::SegaMegadrive(){
  pin_mode[0] = INPUT;   // MD_PIN_D0
  pin_mode[1] = INPUT;   // MD_PIN_D1
  pin_mode[2] = INPUT;   // MD_PIN_D2
  pin_mode[3] = INPUT;   // MD_PIN_D3
  pin_mode[4] = INPUT;   // MD_PIN_D4
  pin_mode[5] = INPUT;   // MD_PIN_D5
  pin_mode[6] = OUTPUT;  // MD_PIN_SEL
  return;
}

void SegaMegadrive::get(void){
  uint8_t i;
  boolean k[20];
  uint8_t pinout[]={MD_PIN_D0, MD_PIN_D1, MD_PIN_D2, MD_PIN_D3, MD_PIN_D4, MD_PIN_D5};

  // Try HIGH/LOW cycles
  // HIGH first
  digitalWrite(pin_number[MD_PIN_SEL], HIGH);
  delayMicroseconds(MD_DELAY);
  // k0=up, k1=down, k2=left, k3=right, k4=B, k5=C(A)
  for(i=0; i<6; i++){
    k[i] = digitalRead(pin_number[pinout[i]]);
  }
  digitalWrite(pin_number[MD_PIN_SEL], LOW);
  delayMicroseconds(MD_DELAY);
  // k6=up, k7=down, k8=0, k9=0, k10=A(C), k11=start
  for(i=0; i<6; i++){
    k[i+6] = digitalRead(pin_number[pinout[i]]);
  }
  if(k[6]==PRESSED && k[7]==PRESSED && k[8]==PRESSED && k[9]==PRESSED) // is a 6 tirggers controller? need a next circle?
  {
    // Try HIGH/LOW again for external tirggers, this is the forth H/L circle.
    digitalWrite(pin_number[MD_PIN_SEL], HIGH);
    delayMicroseconds(MD_DELAY);
    // k12=Z, k13=Y, k14=X, k15=MODE(SELECT), k16=B, k17=C(A)
    for(i=0; i<6; i++){
      k[i+12] = digitalRead(pin_number[pinout[i]]);
    }
    digitalWrite(pin_number[MD_PIN_SEL], LOW);
    delayMicroseconds(MD_DELAY);
    // for k18=A(C), k19=START, other 4 are ignore.
    k[18] = digitalRead(pin_number[MD_PIN_D4]);
    k[19] = digitalRead(pin_number[MD_PIN_D5]);
    if(k[11]==k[19] && k[10]==k[18] && k[4]==k[16] && k[5]==k[17]){// && (k[0]+k[1])<2 && (k[2]+k[3])<2){
      // if Start=Start, A=A, B=B,C=C then these two circle signals is OK.
      // k0=UP,    k1=DOWN,  k2=LEFT, k3=RIGHT,         k4=B,     k5=C(A)
      // k6=UP,    k7=DOWN,  k8=GND,  k9=GND,           k10=A(C), k11=START
      // k12=Z,    k13=Y,    k14=X,   k15=MODE(SELECT), k16=B,    k17=C(A)
      // k18=A(C), k19=START
      subtype=MD_SUBTYPE_6BUTTONS;
      keys->buttons_value[MD_UP]    = k[0];
      keys->buttons_value[MD_DOWN]  = k[1];
      keys->buttons_value[MD_LEFT]  = k[2];
      keys->buttons_value[MD_RIGHT] = k[3];
      keys->buttons_value[MD_MODE]  = k[15];
      keys->buttons_value[MD_START] = k[11];
      keys->buttons_value[MD_A] = k[10];
      keys->buttons_value[MD_B] = k[4];
      keys->buttons_value[MD_C] = k[5];
      keys->buttons_value[MD_X] = k[14];
      keys->buttons_value[MD_Y] = k[13];
      keys->buttons_value[MD_Z] = k[12];      
    }
  }
  else if(k[0]==k[6] && k[1]==k[7] && k[8]==LOW && k[9]==LOW){ // && (k[0]+k[1])<2 && (k[2]+k[3])<2){
    // is a 3 tirggers controller? up=up, down=down, and TOW GND,
    // k0=UP, k1=DOWN, k2=LEFT, k3=RIGHT, k4=B,     k5=C(A)
    // k6=UP, k7=DOWN, k8=GND,  k9=GND,   k10=A(C), k11=START
    subtype=MD_SUBTYPE_3BUTTONS;
    keys->buttons_value[MD_UP]    = k[0];
    keys->buttons_value[MD_DOWN]  = k[1];
    keys->buttons_value[MD_LEFT]  = k[2];
    keys->buttons_value[MD_RIGHT] = k[3];
    keys->buttons_value[MD_START] = k[11];
    keys->buttons_value[MD_A] = k[10];
    keys->buttons_value[MD_B] = k[4];
    keys->buttons_value[MD_C] = k[5];
  }
}
