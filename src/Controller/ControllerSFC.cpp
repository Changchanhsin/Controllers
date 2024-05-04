#include "Arduino.h"
#include "ControllerSFC.h"

NintendoSFC::NintendoSFC(){
  pin_mode[0] = INPUT_PULLUP; // SFC_PIN_DATA
  pin_mode[1] = INPUT_PULLUP; // SFC_PIN_SENSOR
  pin_mode[2] = OUTPUT;       // SFC_PIN_LATCH
  pin_mode[3] = INPUT_PULLUP;
  pin_mode[4] = INPUT_PULLUP;
  pin_mode[5] = OUTPUT;       // SFC_PIN_CLOCK
  pin_mode[6] = INPUT_PULLUP; // SFC_PIN_TRIGGER
  return;
}

void NintendoSFC::begin(void){
  Controller::begin();
  interface.init(pin_number[SFC_PIN_DATA],
                 pin_number[SFC_PIN_SENSOR],
                 pin_number[SFC_PIN_LATCH],
                 pin_number[SFC_PIN_CLOCK],
                 pin_number[SFC_PIN_TRIGGER],
                 pin_number[SFC_PIN_D2]);
}

void NintendoSFC::get(void){
  interface.get();
  keys->buttons_value[SFC_B     ] = interface.raw_dump[0 ];
  keys->buttons_value[SFC_Y     ] = interface.raw_dump[1 ];
  keys->buttons_value[SFC_SELECT] = interface.raw_dump[2 ];
  keys->buttons_value[SFC_START ] = interface.raw_dump[3 ];
  keys->buttons_value[SFC_UP    ] = interface.raw_dump[4 ];
  keys->buttons_value[SFC_DOWN  ] = interface.raw_dump[5 ];
  keys->buttons_value[SFC_LEFT  ] = interface.raw_dump[6 ];
  keys->buttons_value[SFC_RIGHT ] = interface.raw_dump[7 ];
  keys->buttons_value[SFC_A     ] = interface.raw_dump[8 ];
  keys->buttons_value[SFC_X     ] = interface.raw_dump[9 ];
  keys->buttons_value[SFC_L     ] = interface.raw_dump[10];
  keys->buttons_value[SFC_R     ] = interface.raw_dump[11];
}
