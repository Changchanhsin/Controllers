#ifndef Controller_SS_h
#define Controller_SS_h

#include "Arduino.h"
#include "Controller.h"

#define SS_UP     GAMEPAD_UP
#define SS_DOWN   GAMEPAD_DOWN
#define SS_LEFT   GAMEPAD_LEFT
#define SS_RIGHT  GAMEPAD_RIGHT
#define SS_A      GAMEPAD_B
#define SS_B      GAMEPAD_A
#define SS_C      GAMEPAD_R1
#define SS_X      GAMEPAD_Y
#define SS_Y      GAMEPAD_X
#define SS_Z      GAMEPAD_L1
#define SS_MODE   GAMEPAD_SELECT
#define SS_START  GAMEPAD_START
#define SS_L      GAMEPAD_L2
#define SS_R      GAMEPAD_R2
#define SS_1      GAMEPAD_LJ
#define SS_2      GAMEPAD_RJ
#define SS_3      GAMEPAD_HOME

#define SS_PIN_D0   0
#define SS_PIN_D1   1
#define SS_PIN_D2   2
#define SS_PIN_D3   3
#define SS_PIN_S0   5 //TH
#define SS_PIN_S1   6 //TR

#define SS_DELAY  4

#define SS_SUBTYPE_PAD      0x00
#define SS_SUBTYPE_LIGHTGUN 0x01
#define SS_SUBTYPE_ANALOG   0x02
#define SS_SUBTYPE_KEYBOARD 0x10
#define SS_SUBTYPE_MOUSE    0x11
#define SS_SUBTYPE_RACING   0x20

class SegaSaturn : public Controller {
public:
  SegaSaturn();
  void get();
};

#endif
