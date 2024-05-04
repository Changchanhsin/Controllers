#include "ControllerAtari.h"

Atari2600::Atari2600(){
  pin_mode[0] = INPUT_PULLUP;
  pin_mode[1] = INPUT_PULLUP;
  pin_mode[2] = INPUT_PULLUP;
  pin_mode[3] = OUTPUT;
  pin_mode[4] = INPUT_PULLUP;
  pin_mode[5] = INPUT_PULLUP;
  pin_mode[6] = INPUT_PULLUP;
  return;
}

void Atari2600::get(void){
  uint8_t u,d,l,r;

  digitalWrite(pin_number[A2600_PIN_COM], LOW);
  u = digitalRead(pin_number[A2600_PIN_UP]   );
  d = digitalRead(pin_number[A2600_PIN_DOWN] );
  l = digitalRead(pin_number[A2600_PIN_LEFT] );
  r = digitalRead(pin_number[A2600_PIN_RIGHT]);
  if( u == PRESSED && d == PRESSED && l == PRESSED && r == RELEASE){
    keys->buttons_value[A2600_GAMESELECT] = PRESSED;
    keys->buttons_value[A2600_PLAYERSELECT] = RELEASE;
    keys->buttons_value[A2600_RESET] = RELEASE;
    keys->buttons_value[A2600_UP   ] = RELEASE;
    keys->buttons_value[A2600_DOWN ] = RELEASE;
    keys->buttons_value[A2600_LEFT ] = RELEASE;
    keys->buttons_value[A2600_RIGHT] = RELEASE;
  }else{
    keys->buttons_value[A2600_GAMESELECT] = RELEASE;
    if( u == PRESSED && d == PRESSED && l == RELEASE && r == RELEASE){
      keys->buttons_value[A2600_PLAYERSELECT] = PRESSED;
      keys->buttons_value[A2600_RESET] = RELEASE;
      keys->buttons_value[A2600_UP   ] = RELEASE;
      keys->buttons_value[A2600_DOWN ] = RELEASE;
      keys->buttons_value[A2600_LEFT ] = RELEASE;
      keys->buttons_value[A2600_RIGHT] = RELEASE;
    }else if( u == RELEASE && d == RELEASE && l == PRESSED && r == PRESSED){
      keys->buttons_value[A2600_PLAYERSELECT] = RELEASE;
      keys->buttons_value[A2600_RESET] = PRESSED;
      keys->buttons_value[A2600_UP   ] = RELEASE;
      keys->buttons_value[A2600_DOWN ] = RELEASE;
      keys->buttons_value[A2600_LEFT ] = RELEASE;
      keys->buttons_value[A2600_RIGHT] = RELEASE;
    }else{
      keys->buttons_value[A2600_PLAYERSELECT] = RELEASE;
      keys->buttons_value[A2600_RESET] = RELEASE;
      keys->buttons_value[A2600_UP   ] = u;
      keys->buttons_value[A2600_DOWN ] = d;
      keys->buttons_value[A2600_LEFT ] = l;
      keys->buttons_value[A2600_RIGHT] = r;
    }
  }
  keys->buttons_value[A2600_TRIGGER] = digitalRead(pin_number[A2600_PIN_TRIGGER]);
}
