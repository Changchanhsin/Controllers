#ifndef Controller_PS_h
#define Controller_PS_h

#include "Arduino.h"
#include "Controller.h"

#define PS_NONE   GAMEPAD_NONE
#define PS_UP     GAMEPAD_UP
#define PS_DOWN   GAMEPAD_DOWN
#define PS_LEFT   GAMEPAD_LEFT
#define PS_RIGHT  GAMEPAD_RIGHT
#define PS_A      GAMEPAD_A        // A=Cir
#define PS_B      GAMEPAD_B        // B=Cross
#define PS_X      GAMEPAD_X        // X=Tri
#define PS_Y      GAMEPAD_Y        // Y=Rect
#define PS_SELECT GAMEPAD_SELECT
#define PS_START  GAMEPAD_START
#define PS_L1     GAMEPAD_L1
#define PS_R1     GAMEPAD_R1
#define PS_L2     GAMEPAD_L2
#define PS_R2     GAMEPAD_R2
#define PS_LJ     GAMEPAD_LJ
#define PS_RJ     GAMEPAD_RJ

#define PS_AXIS_MIN   0
#define PS_AXIS_MAX 255

#define PS_PIN_DATA 0  // INPUT   brown
#define PS_PIN_ACK  1  // INPUT   green
#define PS_PIN_ATT  2  // OUTPUT  yellow    SELECT LOW enabled
#define PS_PIN_CMD  5  // OUTPUT  orange
#define PS_PIN_CLK  6  // OUTPUT  blue

#define PS_SUBTYPE_DIGITAL 0x41 // Normal digital
#define PS_SUBTYPE_RED     0x73 // Red analog
#define PS_SUBTYPE_GREEN   0x53 // Green analog

class SonyPlaystation : public Controller {
private:
  uint8_t exchangeData(uint8_t cmd);
//  void get_buttons(uint8_t row);
  void get_analogs(void);

public:
  SonyPlaystation();

  void begin();
  void get();
};

#endif
