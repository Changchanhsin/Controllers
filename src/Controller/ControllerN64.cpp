#include <Arduino.h>
#include <pins_arduino.h>

#include "ControllerN64.h"

Nintendo64::Nintendo64(){
  pin_mode[0] = INPUT_PULLUP;
  pin_mode[1] = INPUT_PULLUP;
  pin_mode[2] = INPUT;        // N64_PIN_DATA
  pin_mode[3] = INPUT_PULLUP;
  pin_mode[4] = INPUT_PULLUP; // if LOW, NGC
  pin_mode[5] = INPUT_PULLUP;
  pin_mode[6] = INPUT_PULLUP;
  return;
}


void Nintendo64::begin(void){
  uint8_t command[] = { 0x00 };
  interface.pin = pin_number[N64_PIN_DATA];
  uint8_t receivedBytes = interface.send_get(command, sizeof(command), (uint8_t*)&status, sizeof(N64_Status_t));
  delayMicroseconds(100);
}

void Nintendo64::get(void){
  uint8_t command[] = { 0x01 };
  uint8_t receivedBytes = interface.send_get(command, sizeof(command), (uint8_t*)&report, sizeof(N64_Report_t));
  report.raw16[0]=~report.raw16[0];
  keys->buttons_value[N64_UP   ] = report.dup;
  keys->buttons_value[N64_DOWN ] = report.ddown;
  keys->buttons_value[N64_LEFT ] = report.dleft;
  keys->buttons_value[N64_RIGHT] = report.dright;
  keys->buttons_value[N64_START] = report.start;
  keys->buttons_value[N64_A    ] = report.a;
  keys->buttons_value[N64_B    ] = report.b;
  keys->buttons_value[N64_CU   ] = report.cup;
  keys->buttons_value[N64_CD   ] = report.cdown;
  keys->buttons_value[N64_CL   ] = report.cleft;
  keys->buttons_value[N64_CR   ] = report.cright;
  keys->buttons_value[N64_Z    ] = report.z;
  keys->buttons_value[N64_L    ] = report.l;
  keys->buttons_value[N64_R    ] = report.r;
  keys->axis_value[N64_AXIS_X  ] = map(report.xAxis,N64_AXIS_MIN,N64_AXIS_MAX,AXIS_MIN,AXIS_MAX);
  keys->axis_value[N64_AXIS_Y  ] = map(report.yAxis,N64_AXIS_MIN,N64_AXIS_MAX,AXIS_MAX,AXIS_MIN);
}

uint8_t Nintendo64::write(void){
    // 0 = no input/error, 1 = identify, 2 = status (poll),
    // 3 = read from expansion bus, 4 = write to expansion bus
    uint8_t ret = 0;

    // Get the port mask and the pointers to the in/out/mode registers
    uint8_t bitMask = digitalPinToBitMask(interface.pin);
    uint8_t port = digitalPinToPort(interface.pin);
    volatile uint8_t* modePort = portModeRegister(port);
    volatile uint8_t* outPort = portOutputRegister(port);
    volatile uint8_t* inPort = portInputRegister(port);

    // Don't want interrupts getting in the way
    uint8_t oldSREG = SREG;
    cli();

    // Read in data from the console
    // After receiving the init command you have max 20ms to respond (for the data command)!
    uint8_t command[3];
    uint8_t receivedBytes = interface.get(command, sizeof(command), modePort, outPort, inPort, bitMask);

    // Identify
    if (receivedBytes == 1 && command[0] == 0x00)
    {
        interface.send(status.raw8, sizeof(N64_Status_t), modePort, outPort, bitMask);
        ret = 1;
    }
    // Poll
    else if (receivedBytes == 1 && command[0] == 0x01)
    {
        interface.send(report.raw8, sizeof(N64_Report_t), modePort, outPort, bitMask);
        ret = 2;
    }

    // End of time sensitive code
    SREG = oldSREG;

    return ret;
}
