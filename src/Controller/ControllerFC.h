#ifndef Controller_FC_h
#define Controller_FC_h

#include "Arduino.h"
#include "../Interface/InterfaceFC.h"
#include "Controller.h"

#define FC_UP     GAMEPAD_UP
#define FC_DOWN   GAMEPAD_DOWN
#define FC_LEFT   GAMEPAD_LEFT
#define FC_RIGHT  GAMEPAD_RIGHT
#define FC_A      GAMEPAD_A
#define FC_B      GAMEPAD_B
#define FC_SELECT GAMEPAD_SELECT
#define FC_START  GAMEPAD_START

#define FC_PIN_DATA    0
#define FC_PIN_SENSOR  1
#define FC_PIN_LATCH   2
#define FC_PIN_MIC     3
#define FC_PIN_CLOCK   5
#define FC_PIN_TRIGGER 6

#define FC_SUBTYPE_1P   1
#define FC_SUBTYPE_2P   2
#define FC_SUBTYPE_NES  3

class NintendoFC : public Controller {
private:
  InterfaceFC interface;
public:
  NintendoFC();

  void begin();
  void get();
};

#endif
