#include "Arduino.h"
#include "ControllerFC.h"

NintendoFC::NintendoFC(){
  pin_mode[0] = INPUT;        // FC_PIN_DATA
  pin_mode[1] = INPUT;        // FC_PIN_SENSOR
  pin_mode[2] = OUTPUT;       // FC_PIN_LATCH
  pin_mode[3] = INPUT_PULLUP; // FC_PIN_MIC
  pin_mode[4] = INPUT_PULLUP;
  pin_mode[5] = OUTPUT;       // FC_PIN_CLOCK
  pin_mode[6] = INPUT;        // FC_PIN_TRIGGER
  return;
}

void NintendoFC::begin(void){
  Controller::begin();
  interface.init(pin_number[FC_PIN_DATA   ],
                 pin_number[FC_PIN_SENSOR ],
                 pin_number[FC_PIN_LATCH  ],
                 pin_number[FC_PIN_CLOCK  ],
                 pin_number[FC_PIN_TRIGGER],
                 pin_number[FC_PIN_MIC    ]);
}

void NintendoFC::get(void){
  interface.get();
  keys->buttons_value[FC_A     ] = interface.raw_dump[0];
  keys->buttons_value[FC_B     ] = interface.raw_dump[1];
  keys->buttons_value[FC_SELECT] = interface.raw_dump[2];
  keys->buttons_value[FC_START ] = interface.raw_dump[3];
  keys->buttons_value[FC_UP    ] = interface.raw_dump[4];
  keys->buttons_value[FC_DOWN  ] = interface.raw_dump[5];
  keys->buttons_value[FC_LEFT  ] = interface.raw_dump[6];
  keys->buttons_value[FC_RIGHT ] = interface.raw_dump[7];
}
