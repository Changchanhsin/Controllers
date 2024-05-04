#ifndef Controller_WII_h
#define Controller_WII_h

#include "Arduino.h"
#include "Controller.h"
#include "Wire.h"

#define WII_SUBTYPE_NOCONCETED     0x00
#define WII_SUBTYPE_NUNCHUCK       0x10 //             0xFF00A4200000
#define WII_SUBTYPE_CLASSIC        0x20 //     classic:0x0000A4200101
                                        // classic pro:0x0100A4200101
#define WII_SUBTYPE_CLASSICHIGH    0x21 //     classic:0x0000A4200101
                                        // classic pro:0x0100A4200101
// Guitar Hero
#define WII_SUBTYPE_GUITAR         0x30
#define WII_SUBTYPE_DRUM           0x31
#define WII_SUBTYPE_DJTURNTABLE    0x32
#define WII_SUBTYPE_UDRAWTABLET    0x40
#define WII_SUBTYPE_DRAWSOMETABLET 0x41
#define WII_SUBTYPE_BLANCE         0x50 //             0x0000A4200402
#define WII_SUBTYPE_UNKNOWN        0xFF

#define WII_UP     GAMEPAD_UP
#define WII_DOWN   GAMEPAD_DOWN
#define WII_LEFT   GAMEPAD_LEFT
#define WII_RIGHT  GAMEPAD_RIGHT
#define WII_A      GAMEPAD_A
#define WII_B      GAMEPAD_B
#define WII_X      GAMEPAD_X
#define WII_Y      GAMEPAD_Y
#define WII_Z      GAMEPAD_A
#define WII_C      GAMEPAD_B
#define WII_SELECT GAMEPAD_SELECT
#define WII_MINUS  GAMEPAD_SELECT
#define WII_START  GAMEPAD_START
#define WII_PLUS   GAMEPAD_START
#define WII_HOME   GAMEPAD_HOME
#define WII_ZL     GAMEPAD_L1
#define WII_ZR     GAMEPAD_R1
#define WII_L      GAMEPAD_L2
#define WII_R      GAMEPAD_R2

#define WII_ADDRESS 0x52

// Calibration accelerometer values, depends on your Nunchuk
#define NUNCHUK_ACCEL_X_ZERO 512
#define NUNCHUK_ACCEL_Y_ZERO 512
#define NUNCHUK_ACCEL_Z_ZERO 512

// Calibration joystick values
#define NUNCHUK_JOYSTICK_X_ZERO 127
#define NUNCHUK_JOYSTICK_Y_ZERO 128

// Whether to disable encryption. Enabling encryption means that every packet must be decrypted, which wastes cpu cycles. Cheap Nunchuk clones have problems with the encrypted init sequence, so be sure you know what you're doing
#define NUNCHUK_DISABLE_ENCRYPTION

#define WII_CONVERSION_DEALAY 175

class NintendoWII : public Controller {
private:
  uint8_t data[8];
  uint8_t type[6];

  inline boolean writePointer(byte ptr, boolean delay = true);
  inline boolean writeRegister(byte reg, byte value, boolean delay = true);
  inline boolean requestMultiple(uint8_t requestSize, uint8_t * dataOut);
  inline boolean readDataArray(byte ptr, uint8_t requestSize, uint8_t * dataOut);
  inline boolean readRegister(byte reg, uint8_t* dataOut);

public:
  void begin();
  void get();
};

#endif
