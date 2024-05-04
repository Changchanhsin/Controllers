#include "Arduino.h"
#include "ControllerSS.h"

SegaSaturn::SegaSaturn(){
  pin_mode[0] = INPUT;   // SS_PIN_D0
  pin_mode[1] = INPUT;   // SS_PIN_D1
  pin_mode[2] = INPUT;   // SS_PIN_D2
  pin_mode[3] = INPUT;   // SS_PIN_D3
  pin_mode[4] = INPUT_PULLUP;
  pin_mode[5] = OUTPUT;  // SS_PIN_S0
  pin_mode[6] = OUTPUT;  // SS_PIN_S1
  return;
}

void SegaSaturn::get(void){
  digitalWrite(pin_number[SS_PIN_S0], LOW );
  digitalWrite(pin_number[SS_PIN_S1], HIGH);
  delayMicroseconds(SS_DELAY);
  keys->buttons_value[SS_UP   ] = digitalRead(pin_number[SS_PIN_D0]);
  keys->buttons_value[SS_DOWN ] = digitalRead(pin_number[SS_PIN_D1]);

  if(keys->buttons_value[SS_UP   ]==LOW && keys->buttons_value[SS_DOWN ]==LOW){
    subtype = SS_SUBTYPE_LIGHTGUN;
    keys->buttons_value[SS_UP   ] = RELEASE;
    keys->buttons_value[SS_DOWN ] = RELEASE;
    keys->buttons_value[SS_A    ] = digitalRead(pin_number[SS_PIN_D2]);
    keys->buttons_value[SS_B    ] = digitalRead(pin_number[SS_PIN_D3]);
    digitalWrite(pin_number[SS_PIN_S0], HIGH);
    digitalWrite(pin_number[SS_PIN_S1], HIGH);
    delayMicroseconds(SS_DELAY);
    keys->buttons_value[SS_X    ] = digitalRead(pin_number[SS_PIN_D2]);
    keys->buttons_value[SS_Y    ] = digitalRead(pin_number[SS_PIN_D3]);
    digitalWrite(pin_number[SS_PIN_S0], LOW);
    digitalWrite(pin_number[SS_PIN_S1], LOW);
    delayMicroseconds(SS_DELAY);
    keys->buttons_value[SS_C    ] = digitalRead(pin_number[SS_PIN_D2]);
    keys->buttons_value[SS_Z    ] = digitalRead(pin_number[SS_PIN_D3]);
    digitalWrite(pin_number[SS_PIN_S0], HIGH);
    digitalWrite(pin_number[SS_PIN_S1], LOW);
    delayMicroseconds(SS_DELAY);
    keys->buttons_value[SS_L    ] = digitalRead(pin_number[SS_PIN_D2]);
    keys->buttons_value[SS_R    ] = digitalRead(pin_number[SS_PIN_D3]);
  }else{
    subtype = SS_SUBTYPE_PAD;
    keys->buttons_value[SS_LEFT ] = digitalRead(pin_number[SS_PIN_D2]);
    keys->buttons_value[SS_RIGHT] = digitalRead(pin_number[SS_PIN_D3]);

    digitalWrite(pin_number[SS_PIN_S0], LOW);
    digitalWrite(pin_number[SS_PIN_S1], LOW);
    delayMicroseconds(SS_DELAY);
    keys->buttons_value[SS_Z] = digitalRead(pin_number[SS_PIN_D0]);
    keys->buttons_value[SS_Y] = digitalRead(pin_number[SS_PIN_D1]);
    keys->buttons_value[SS_X] = digitalRead(pin_number[SS_PIN_D2]);
    keys->buttons_value[SS_R] = digitalRead(pin_number[SS_PIN_D3]);

    digitalWrite(pin_number[SS_PIN_S0], HIGH);
    digitalWrite(pin_number[SS_PIN_S1], LOW );
    delayMicroseconds(SS_DELAY);
    keys->buttons_value[SS_B    ] = digitalRead(pin_number[SS_PIN_D0]);
    keys->buttons_value[SS_C    ] = digitalRead(pin_number[SS_PIN_D1]);
    keys->buttons_value[SS_A    ] = digitalRead(pin_number[SS_PIN_D2]);
    keys->buttons_value[SS_START] = digitalRead(pin_number[SS_PIN_D3]);

    digitalWrite(pin_number[SS_PIN_S0], HIGH);
    digitalWrite(pin_number[SS_PIN_S1], HIGH);
    delayMicroseconds(SS_DELAY);
  //  keys->buttons_value[SS_1    ] = digitalRead(pin_number[SS_PIN_D0]);
  //  keys->buttons_value[SS_2    ] = digitalRead(pin_number[SS_PIN_D1]);
  //  keys->buttons_value[SS_3    ] = digitalRead(pin_number[SS_PIN_D2]);
    keys->buttons_value[SS_L    ] = digitalRead(pin_number[SS_PIN_D3]);
  }
}
