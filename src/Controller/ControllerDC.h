#ifndef Controller_DC_h
#define Controller_DC_h

#include "Arduino.h"
#include "Controller.h"

#define DC_UP     GAMEPAD_UP
#define DC_DOWN   GAMEPAD_DOWN
#define DC_LEFT   GAMEPAD_LEFT
#define DC_RIGHT  GAMEPAD_RIGHT
#define DC_A      GAMEPAD_B
#define DC_B      GAMEPAD_A
#define DC_R      GAMEPAD_R1
#define DC_X      GAMEPAD_Y
#define DC_Y      GAMEPAD_X
#define DC_L      GAMEPAD_L1
#define DC_START  GAMEPAD_START

#define DC_PIN_D0     0
#define DC_PIN_D5     1
#define DC_PIN_SENSOR 5

class SegaDreamcast : public Controller {
private:

public:
  SegaDreamcast();

  void get();
};

#endif
