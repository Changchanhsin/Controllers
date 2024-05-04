#ifndef Controller_MD_h
#define Controller_MD_h

#include "Arduino.h"
#include "Controller.h"

#define MD_UP     GAMEPAD_UP
#define MD_DOWN   GAMEPAD_DOWN
#define MD_LEFT   GAMEPAD_LEFT
#define MD_RIGHT  GAMEPAD_RIGHT
#define MD_A      GAMEPAD_B
#define MD_B      GAMEPAD_A
#define MD_C      GAMEPAD_R1
#define MD_X      GAMEPAD_Y
#define MD_Y      GAMEPAD_X
#define MD_Z      GAMEPAD_L1
#define MD_MODE   GAMEPAD_SELECT
#define MD_START  GAMEPAD_START

#define MD_PIN_D0   0
#define MD_PIN_D1   1
#define MD_PIN_D2   2
#define MD_PIN_D3   3
#define MD_PIN_D4   4
#define MD_PIN_D5   5
#define MD_PIN_SEL  6

#define MD_SUBTYPE_3BUTTONS 1
#define MD_SUBTYPE_6BUTTONS 2
#define MD_SUBTYPE_STUNNER  3

#define MD_DELAY 6
  
class SegaMegadrive : public Controller {
private:

public:
  SegaMegadrive();

  void get();
};

#endif
