#ifndef Controller_ATARI_h
#define Controller_ATARI_h

#include "Arduino.h"
#include "Controller.h"

#define A2600_UP           GAMEPAD_UP
#define A2600_DOWN         GAMEPAD_DOWN
#define A2600_LEFT         GAMEPAD_LEFT
#define A2600_RIGHT        GAMEPAD_RIGHT
#define A2600_TRIGGER      GAMEPAD_A
#define A2600_PLAYERSELECT GAMEPAD_SELECT
#define A2600_RESET        GAMEPAD_START
#define A2600_GAMESELECT   GAMEPAD_HOME

#define A2600_PIN_UP      0
#define A2600_PIN_DOWN    1
#define A2600_PIN_LEFT    2
#define A2600_PIN_COM     3
#define A2600_PIN_RIGHT   5
#define A2600_PIN_TRIGGER 6


class Atari2600 : public Controller {
public:
  Atari2600();

  void get();
};

#endif
