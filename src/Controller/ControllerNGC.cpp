#include "ControllerNGC.h"

NintendoGamecube::NintendoGamecube(){
  pin_mode[0] = INPUT_PULLUP;
  pin_mode[1] = INPUT_PULLUP;
  pin_mode[2] = INPUT;        // NGC_PIN_DATA
  pin_mode[3] = INPUT_PULLUP;
  pin_mode[4] = INPUT_PULLUP; // if LOW, NGC
  pin_mode[5] = INPUT_PULLUP;
  pin_mode[6] = INPUT_PULLUP;
  return;
}

void NintendoGamecube::begin(void){
  uint8_t command[] = { 0x00 };
  interface.pin=pin_number[NGC_PIN_DATA];
  // Send the command and read in data
  uint8_t receivedBytes = interface.send_get(command, sizeof(command), (uint8_t*)&status, sizeof(Gamecube_Status_t));

  // Return status information for optional use.
  // On error the report may have been modified!
  //return (receivedBytes == sizeof(Gamecube_Status_t));
}

void NintendoGamecube::plugged(void){
    // Get the initital controller values when it was plugged in.
    // This is used for calibration of the controller.
    // If you do not call this function the controller data bit "origin" will be set.
    // This results in a not accepting Gamecube host/reinitialization.
    uint8_t command[] = { 0x41 };

    // Send the command and read in data
    uint8_t receivedBytes = interface.send_get(command, sizeof(command), (uint8_t*)&origin, sizeof(Gamecube_Origin_t));

    // Return status information for optional use.
    // On error the report may have been modified!
    //return (receivedBytes == sizeof(Gamecube_Origin_t));
}

void NintendoGamecube::get(void){
  bool rumble = 0;
  uint8_t command[] = { 0x40, 0x03, rumble };

  // Send the command and read in data
  uint8_t receivedBytes = interface.send_get(command, sizeof(command), (uint8_t*)&report, sizeof(Gamecube_Report_t));

  // Return status information for optional use.
  // On error the report may have been modified!
  // return (receivedBytes == sizeof(Gamecube_Report_t));
  report.raw16[0]=~report.raw16[0];
  keys->buttons_value[NGC_UP]    = report.dup;
  keys->buttons_value[NGC_DOWN]  = report.ddown;
  keys->buttons_value[NGC_LEFT]  = report.dleft;
  keys->buttons_value[NGC_RIGHT] = report.dright;
  keys->buttons_value[NGC_START] = report.start;
  keys->buttons_value[NGC_A] = report.a;
  keys->buttons_value[NGC_B] = report.b;
  keys->buttons_value[NGC_X] = report.x;
  keys->buttons_value[NGC_Y] = report.y;
  keys->buttons_value[NGC_Z] = report.z;
  keys->buttons_value[NGC_L] = report.l;
  keys->buttons_value[NGC_R] = report.r;
  keys->axis_value[NGC_AXIS_X]  = map(report.xAxis, NGC_AXIS_MIN,NGC_AXIS_MAX,AXIS_MIN,AXIS_MAX);
  keys->axis_value[NGC_AXIS_Y]  = map(report.yAxis, NGC_AXIS_MIN,NGC_AXIS_MAX,AXIS_MAX,AXIS_MIN);
  keys->axis_value[NGC_AXIS_RX] = map(report.cxAxis,NGC_AXIS_MIN,NGC_AXIS_MAX,AXIS_MIN,AXIS_MAX);
  keys->axis_value[NGC_AXIS_RY] = map(report.cyAxis,NGC_AXIS_MIN,NGC_AXIS_MAX,AXIS_MIN,AXIS_MAX);
  keys->axis_value[NGC_AXIS_L]  = map(report.left,  NGC_AXIS_MIN,NGC_AXIS_MAX,AXIS_MIN,AXIS_MAX);
  keys->axis_value[NGC_AXIS_R]  = map(report.right, NGC_AXIS_MIN,NGC_AXIS_MAX,AXIS_MIN,AXIS_MAX);
}

void NintendoGamecube::report_convert(Gamecube_Report_t* report, Gamecube_Report_t* dest_report, uint8_t mode)
{
    memcpy(dest_report, report, sizeof(Gamecube_Report_t));
    if (mode == 1)
    {
        dest_report->mode1.cxAxis = report->cxAxis >> 4;
        dest_report->mode1.cyAxis = report->cyAxis >> 4;
        dest_report->mode1.left = report->left;
        dest_report->mode1.right = report->right;
        dest_report->mode1.analogA = 0;
        dest_report->mode1.analogB = 0;
    }
    else if (mode == 2)
    {
        dest_report->mode2.cxAxis = report->cxAxis >> 4;
        dest_report->mode2.cyAxis = report->cyAxis >> 4;
        dest_report->mode2.left = report->left >> 4;
        dest_report->mode2.right = report->right >> 4;
        dest_report->mode2.analogA = 0;
        dest_report->mode2.analogB = 0;
    }
    else if (mode == 4)
    {
        dest_report->mode4.cxAxis = report->cxAxis;
        dest_report->mode4.cyAxis = report->cyAxis;
        dest_report->mode4.analogA = 0;
        dest_report->mode4.analogB = 0;
    }
    else if (mode == 3)
    {
        return;
    }
    // Mode 0, 5, 6, 7
    else
    {
        dest_report->mode0.cxAxis = report->cxAxis;
        dest_report->mode0.cyAxis = report->cyAxis;
        dest_report->mode0.left = report->left >> 4;
        dest_report->mode0.right = report->right >> 4;
        dest_report->mode0.analogA = 0;
        dest_report->mode0.analogB = 0;
    }
}

uint8_t NintendoGamecube::write(void){
    // 0 = no input/error, 1 = init, 2 = origin, 3 = read, 4 = read with rumble
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
    // After receiving the init command you have max 80us to respond (for the data command)!
    uint8_t command[3];
    uint8_t receivedBytes = interface.get(command, sizeof(command), modePort, outPort, inPort, bitMask);

    // Init or reset
    if (receivedBytes == 1 && (command[0] == 0x00 || command[0] == 0xFF))
    {
        interface.send(status.raw8, sizeof(Gamecube_Status_t), modePort, outPort, bitMask);
        ret = 1;
    }
    // Get origin or recalibrate
    else if (receivedBytes == 1 && (command[0] == 0x41 || command[0] == 0x42))
    {
        interface.send(origin.raw8, sizeof(Gamecube_Origin_t), modePort, outPort, bitMask);
        ret = 2;
    }
    // Get data. Do not check last byte (command[2]), as the flags are unknown
    else if (receivedBytes == 3 && command[0] == 0x40 && command[1] <= 0x07)
    {
        Gamecube_Report_t dest_report;
        report_convert(&report, &dest_report, command[1]);
        interface.send(dest_report.raw8, sizeof(dest_report), modePort, outPort, bitMask);
        ret = 3;
        // The first byte probably flags a gamecube reading (0x40), as the same
        // protocol is also used for N64. The lower nibble seems to be the mode:
        // 0x40 (followed by 2 bytes) reading
        // 0x41 get origin (1 byte)
        // 0x42 (followed by 2 bytes) seems to force mode 0x02 below
        // 0x43 (followed by 2 bytes) seems to force mode 0x02 below

        // The 2nd byte (command[1]) seems to request a special data format.
        // I've noticed formats that merge the L + R data.
        // There seem to be only 4 data formats, the rest is ignore.
        // 0x00 First 4 bytes: buttons0+1 + X + Y, C-Stick, L+R minimum of both, 0x00 fixed
        // 0x01 First 4 bytes: buttons0+1 + X + Y, C-Stick Horizontal only, R, L, 0x00 fixed
        // 0x02 First 4 bytes: buttons0+1 + X + Y, C-Stick Horizontal only, L+R minimum of both, 0x02 fixed, 0x01 fixed
        // 0x03 Normal reading

        // I've seen 3 last options for the last byte (command[2]):
        // 0x00 Normal reading
        // 0x01 Enable rumble
        // 0x02 Normal reading, rumble was at least called once
        // 0x03 ??? - never seen so far
        // Rumble: 1, 5, 9, 13, 17, ...
        // You can see that only 4 of those requests are possible,
        // the gamecube will ignore the left 6 MSB.
        if((command[2] % 4) == 0x01){
            ret = 4;
        }
        else if((command[2] % 4) == 0x02){
            ret = 5;
        }
        else if((command[2] % 4) == 0x03){
            ret = 6;
        }
    }

    // End of time sensitive code
    SREG = oldSREG;

    return ret;
}

