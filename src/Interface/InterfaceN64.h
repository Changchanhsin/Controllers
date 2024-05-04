#ifndef InterfaceN64_h
#define InterfaceN64_h

#include <Arduino.h>

typedef union{
    // 3 bytes of statusreport that we get from the controller
    uint8_t raw8[3];
    uint16_t raw16[0];
    struct {
        // Device information, needs to be swapped to fit the documentation below:
        // 15 wireless (1: wireless Controller)
        // 14 wireless receive (0: not wireless 1: wireless)
        // 13 Rumble Motor (0: available 1: not available)
        // 12 Type of Controller (always 0 ?)
        // 11 Type of Controller (0: N64 1: Dolphin)
        // 10 wireless type (0:IF 1:RF)
        //  9 wireless state (0: variable 1: fixed)
        //  8 0: non standard Controller 1: Dolphin Standard Controller
        //  7
        //  6
        //  5 wireless origin (0:invalid 1:valid)
        //  4 wireless fix id (0:not fixed 1:fixed)
        //  3 wireless type - 0:normal 1: non-controller (?)
        //  2 wireless type - 0:normal 1: lite controller
        //  1 wireless type -
        //  0 wireless type -
        uint16_t device;

        // controller status (only rumble is known)
        union{
            uint8_t status;
            struct{
                uint8_t status0 : 3;
                uint8_t rumble : 1;
                uint8_t status1 : 4;
            };
        };
    };
} N64_NGC_Status_t;

class InterfaceN64 {
public:
  uint8_t pin;
//  InterfaceN64();
  uint8_t send_get(uint8_t* command, const uint8_t commandLen,
                   uint8_t* report,  const uint8_t reportLen) __attribute__((noinline));
  void send(const uint8_t* buff, uint8_t len,
    volatile uint8_t* modePort, volatile uint8_t* outPort, uint8_t bitMask)
    __attribute__((noinline));

  uint8_t get(uint8_t* buff, uint8_t len,
    volatile uint8_t* modePort, volatile uint8_t* outPort, volatile uint8_t * inPort, uint8_t bitMask)
    __attribute__((noinline));
};


#endif
