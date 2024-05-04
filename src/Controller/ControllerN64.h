#ifndef ControllerN64_h
#define ControllerN64_h

#include <Arduino.h>
#include "../Interface/InterfaceN64.h"
#include "Controller.h"

#define N64_UP     GAMEPAD_UP
#define N64_DOWN   GAMEPAD_DOWN
#define N64_LEFT   GAMEPAD_LEFT
#define N64_RIGHT  GAMEPAD_RIGHT
#define N64_A      GAMEPAD_A
#define N64_CD     GAMEPAD_Y
#define N64_B      GAMEPAD_B
#define N64_CR     GAMEPAD_X
#define N64_CU     GAMEPAD_R1
#define N64_CL     GAMEPAD_L1
#define N64_L      GAMEPAD_L2
#define N64_R      GAMEPAD_R2
#define N64_Z      GAMEPAD_SELECT
#define N64_START  GAMEPAD_START
#define N64_AXIS_X GAMEPAD_AXIS_X
#define N64_AXIS_Y GAMEPAD_AXIS_Y

#define N64_PIN_DATA 2

#define A_IDX       0
#define B_IDX       1
#define Z_IDX       2
#define START_IDX   3
#define D_UP_IDX    4
#define D_DOWN_IDX  5
#define D_LEFT_IDX  6
#define D_RIGHT_IDX 7
#define L_IDX       10
#define R_IDX       11
#define C_UP_IDX    12
#define C_DOWN_IDX  13
#define C_LEFT_IDX  14
#define C_RIGHT_IDX 15
#define X_IDX       16
#define Y_IDX       24

#define N64_AXIS_MIN -88
#define N64_AXIS_MAX  88

#define NINTENDO_DEVICE_N64_WIRED   0x0005
#define NINTENDO_DEVICE_N64_NONE    0x0000

typedef union{
    // 4 bytes of datareport that we get from the controller
    uint8_t raw8[4];
    uint16_t raw16[0];
    uint32_t raw32[0];

    struct{
        uint8_t dpad : 4;
        uint8_t buttons0 : 4;
        uint8_t cpad : 4;
        uint8_t buttons1 : 4;
    };

    struct {
        // first data byte (bitfields are sorted in LSB order)
        uint8_t dright : 1;
        uint8_t dleft : 1;
        uint8_t ddown : 1;
        uint8_t dup : 1;
        uint8_t start : 1;
        uint8_t z : 1;
        uint8_t b : 1;
        uint8_t a : 1;

        // second data byte
        uint8_t cright : 1;
        uint8_t cleft : 1;
        uint8_t cdown : 1;
        uint8_t cup : 1;
        uint8_t r : 1;
        uint8_t l : 1;
        uint8_t low1 : 1;
        uint8_t low0 : 1;

        // 3rd-4th data byte
        int8_t xAxis;
        int8_t yAxis;
    };
} N64_Report_t;

// Gamecube an N64 use the same status schema
typedef N64_NGC_Status_t N64_Status_t;

class Nintendo64 : public Controller {
private:
  InterfaceN64 interface;

  N64_Status_t status;
  N64_Report_t report;
  uint8_t write();

public:
  Nintendo64();

  void begin();
  void get();
};

#endif
