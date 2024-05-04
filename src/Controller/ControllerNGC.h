#ifndef ControllerNGC_h
#define ControllerNGC_h

#include "../Interface/InterfaceN64.h"
#include "Controller.h"

#define NGC_UP      GAMEPAD_UP
#define NGC_DOWN    GAMEPAD_DOWN
#define NGC_LEFT    GAMEPAD_LEFT
#define NGC_RIGHT   GAMEPAD_RIGHT
#define NGC_B       GAMEPAD_B
#define NGC_A       GAMEPAD_A
#define NGC_Y       GAMEPAD_Y
#define NGC_X       GAMEPAD_X
#define NGC_L       GAMEPAD_L1
#define NGC_R       GAMEPAD_R1
#define NGC_Z       GAMEPAD_SELECT
#define NGC_START   GAMEPAD_START
#define NGC_AXIS_X  GAMEPAD_AXIS_X
#define NGC_AXIS_Y  GAMEPAD_AXIS_Y
#define NGC_AXIS_RX GAMEPAD_AXIS_RX
#define NGC_AXIS_RY GAMEPAD_AXIS_RY
#define NGC_AXIS_L  GAMEPAD_AXIS_Z
#define NGC_AXIS_R  GAMEPAD_AXIS_RZ

#define NGC_AXIS_MIN   0
#define NGC_AXIS_MAX 255

#define NGC_PIN_DATA 2

#define NINTENDO_DEVICE_GC_WIRED     0x0009
#define NINTENDO_DEVICE_GC_NONE      0x0000

typedef union{
    // 8 bytes of datareport that we get from the controller
    uint8_t raw8[8];
    uint16_t raw16[0];
    uint32_t raw32[0];

    struct{
        uint8_t buttons0;
        union{
            uint8_t buttons1;
            uint8_t dpad : 4;
        };
    };

    struct {
        // first data byte (bitfields are sorted in LSB order)
        uint8_t a : 1;
        uint8_t b : 1;
        uint8_t x : 1;
        uint8_t y : 1;
        uint8_t start : 1;
        uint8_t origin : 1; // Indicates if GetOrigin(0x41) was called (LOW)
        uint8_t errlatch : 1;
        uint8_t errstat : 1;

        // second data byte
        uint8_t dleft : 1;
        uint8_t dright : 1;
        uint8_t ddown : 1;
        uint8_t dup : 1;
        uint8_t z : 1;
        uint8_t r : 1;
        uint8_t l : 1;
        uint8_t high1 : 1;

        // 3rd-8th data byte
        uint8_t xAxis;
        uint8_t yAxis;
        uint8_t cxAxis;
        uint8_t cyAxis;
        uint8_t left;
        uint8_t right;
    }; // mode3 (default reading mode)

    struct {
        // first data byte (bitfields are sorted in LSB order)
        uint8_t a : 1;
        uint8_t b : 1;
        uint8_t x : 1;
        uint8_t y : 1;
        uint8_t start : 1;
        uint8_t origin : 1; // Indicates if GetOrigin(0x41) was called (LOW)
        uint8_t errlatch : 1;
        uint8_t errstat : 1;

        // second data byte
        uint8_t dleft : 1;
        uint8_t dright : 1;
        uint8_t ddown : 1;
        uint8_t dup : 1;
        uint8_t z : 1;
        uint8_t r : 1;
        uint8_t l : 1;
        uint8_t high1 : 1;

        // 3rd-8th data byte
        uint8_t xAxis;
        uint8_t yAxis;
        uint8_t cxAxis;
        uint8_t cyAxis;
        /*
        uint8_t left;
        uint8_t right;
        */
        uint8_t left : 4;
        uint8_t right : 4;
        uint8_t analogA : 4;
        uint8_t analogB : 4;
    } mode0;

    struct {
        // first data byte (bitfields are sorted in LSB order)
        uint8_t a : 1;
        uint8_t b : 1;
        uint8_t x : 1;
        uint8_t y : 1;
        uint8_t start : 1;
        uint8_t origin : 1; // Indicates if GetOrigin(0x41) was called (LOW)
        uint8_t errlatch : 1;
        uint8_t errstat : 1;

        // second data byte
        uint8_t dleft : 1;
        uint8_t dright : 1;
        uint8_t ddown : 1;
        uint8_t dup : 1;
        uint8_t z : 1;
        uint8_t r : 1;
        uint8_t l : 1;
        uint8_t high1 : 1;

        // 3rd-8th data byte
        uint8_t xAxis;
        uint8_t yAxis;
        uint8_t cxAxis : 4;
        uint8_t cyAxis : 4;
        uint8_t left;
        uint8_t right;
        uint8_t analogA : 4;
        uint8_t analogB : 4;
    } mode1;

    struct {
        // first data byte (bitfields are sorted in LSB order)
        uint8_t a : 1;
        uint8_t b : 1;
        uint8_t x : 1;
        uint8_t y : 1;
        uint8_t start : 1;
        uint8_t origin : 1; // Indicates if GetOrigin(0x41) was called (LOW)
        uint8_t errlatch : 1;
        uint8_t errstat : 1;

        // second data byte
        uint8_t dleft : 1;
        uint8_t dright : 1;
        uint8_t ddown : 1;
        uint8_t dup : 1;
        uint8_t z : 1;
        uint8_t r : 1;
        uint8_t l : 1;
        uint8_t high1 : 1;

        // 3rd-8th data byte
        uint8_t xAxis;
        uint8_t yAxis;
        uint8_t cxAxis : 4;
        uint8_t cyAxis : 4;
        uint8_t left : 4;
        uint8_t right : 4;
        uint8_t analogA;
        uint8_t analogB;
    } mode2;

    struct {
        // first data byte (bitfields are sorted in LSB order)
        uint8_t a : 1;
        uint8_t b : 1;
        uint8_t x : 1;
        uint8_t y : 1;
        uint8_t start : 1;
        uint8_t origin : 1; // Indicates if GetOrigin(0x41) was called (LOW)
        uint8_t errlatch : 1;
        uint8_t errstat : 1;

        // second data byte
        uint8_t dleft : 1;
        uint8_t dright : 1;
        uint8_t ddown : 1;
        uint8_t dup : 1;
        uint8_t z : 1;
        uint8_t r : 1;
        uint8_t l : 1;
        uint8_t high1 : 1;

        // 3rd-8th data byte
        uint8_t xAxis;
        uint8_t yAxis;
        uint8_t cxAxis;
        uint8_t cyAxis;
        uint8_t analogA;
        uint8_t analogB;
    } mode4;

} Gamecube_Report_t;

typedef N64_NGC_Status_t Gamecube_Status_t;

typedef union{
    // GetOrigin requests the initial values of the controller when it was plugged in (powered up)
    // This request consists of a normal data package and 2 unknown bytes.
    // It is possible that those mark the deadzone/toleranz, but are zero in my tests.
    // If the GetOrigin request was not sent to the controller, it will respond it in the data report.
    // The gamecube will NOT accept controller data when no GetOrigin request was made (origin bit in data report set).
    uint8_t raw8[10];
    uint16_t raw16[0];
    uint32_t raw32[0];

    struct{
        Gamecube_Report_t inititalData;
        uint8_t deadzone0;
        uint8_t deadzone1;
    };
} Gamecube_Origin_t;

class NintendoGamecube : public Controller {
private:
  Gamecube_Status_t status;
  Gamecube_Origin_t origin;
  Gamecube_Report_t report;
  InterfaceN64 interface;

public:
  NintendoGamecube();

  void begin(void);
  void plugged(void);
  void report_convert(Gamecube_Report_t* report, Gamecube_Report_t* dest_report, uint8_t mode);
  void get(void);
  uint8_t write(void);
};

#endif
