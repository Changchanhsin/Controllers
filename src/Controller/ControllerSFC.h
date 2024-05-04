#ifndef Controller_SFC_h
#define Controller_SFC_h

#include "Arduino.h"
#include "../Interface/InterfaceFC.h"
#include "Controller.h"

#define SFC_UP     GAMEPAD_UP
#define SFC_DOWN   GAMEPAD_DOWN
#define SFC_LEFT   GAMEPAD_LEFT
#define SFC_RIGHT  GAMEPAD_RIGHT
#define SFC_A      GAMEPAD_A
#define SFC_B      GAMEPAD_B
#define SFC_X      GAMEPAD_X
#define SFC_Y      GAMEPAD_Y
#define SFC_SELECT GAMEPAD_SELECT
#define SFC_START  GAMEPAD_START
#define SFC_L      GAMEPAD_L1
#define SFC_R      GAMEPAD_R1

#define SFC_PIN_DATA    0
#define SFC_PIN_SENSOR  1
#define SFC_PIN_LATCH   2
#define SFC_PIN_D2      3
#define SFC_PIN_CLOCK   5
#define SFC_PIN_TRIGGER 6

class NintendoSFC : public Controller {
private:
  InterfaceFC interface;
public:
  NintendoSFC();

  void begin();
  void get();
};

#endif
