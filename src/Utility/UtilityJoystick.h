/*
  Joystick.h

  Copyright (c) 2015-2017, Matthew Heironimus

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef JOYSTICK_h
#define JOYSTICK_h

#include "UtilityHID.h"

#if ARDUINO < 10606
#error The Joystick library requires Arduino IDE 1.6.6 or greater. Please update your IDE.
#endif // ARDUINO < 10606

#if ARDUINO > 10606
#if !defined(USBCON)
#error The Joystick library can only be used with a USB MCU (e.g. Arduino Leonardo, Arduino Micro, etc.).
#endif // !defined(USBCON)
#endif // ARDUINO > 10606

#if !defined(_USING_DYNAMIC_HID)

#warning "Using legacy HID core (non pluggable)"

#else // !defined(_USING_DYNAMIC_HID)

//================================================================================
//  Joystick (Gamepad)

#define JOYSTICK_AXIS_DATATYPE   int16_t

#define JOYSTICK_AXIS_MINIMUM 0
#define JOYSTICK_AXIS_MAXIMUM 65535
#define JOYSTICK_SIMULATOR_MINIMUM 0
#define JOYSTICK_SIMULATOR_MAXIMUM 65535

#define JOYSTICK_DEFAULT_REPORT_ID           0x03
#define JOYSTICK_DEFAULT_BUTTON_COUNT          16
#define JOYSTICK_DEFAULT_AXIS_MINIMUM      -32768 //   0
#define JOYSTICK_DEFAULT_AXIS_MAXIMUM       32767 //1023
#define JOYSTICK_DEFAULT_SIMULATOR_MINIMUM -32768 //   0
#define JOYSTICK_DEFAULT_SIMULATOR_MAXIMUM  32767 // 128
#define JOYSTICK_DEFAULT_HATSWITCH_COUNT        1 //   2
#define JOYSTICK_HATSWITCH_COUNT_MAXIMUM        2
#define JOYSTICK_HATSWITCH_RELEASE             -1
#define JOYSTICK_TYPE_JOYSTICK               0x04
#define JOYSTICK_TYPE_GAMEPAD                0x05
#define JOYSTICK_TYPE_MULTI_AXIS             0x08

class Joystick_
{
private:

    // Joystick State
    JOYSTICK_AXIS_DATATYPE   _xAxis;
    JOYSTICK_AXIS_DATATYPE   _yAxis;
    JOYSTICK_AXIS_DATATYPE   _zAxis;
    JOYSTICK_AXIS_DATATYPE   _xAxisRotation;
    JOYSTICK_AXIS_DATATYPE   _yAxisRotation;
    JOYSTICK_AXIS_DATATYPE   _zAxisRotation;
    JOYSTICK_AXIS_DATATYPE   _throttle;
    JOYSTICK_AXIS_DATATYPE   _rudder;
    JOYSTICK_AXIS_DATATYPE   _accelerator;
    JOYSTICK_AXIS_DATATYPE   _brake;
    JOYSTICK_AXIS_DATATYPE   _steering;
    int16_t    _hatSwitchValues[JOYSTICK_HATSWITCH_COUNT_MAXIMUM];
    uint8_t   *_buttonValues = NULL;

    // Joystick Settings
    bool     _autoSendState;
    uint8_t  _buttonCount;
    uint8_t  _buttonValuesArraySize = 0;
    uint8_t  _hatSwitchCount;
    uint8_t  _includeAxisFlags;
    uint8_t  _includeSimulatorFlags;

    uint8_t   _hidReportId;
    uint8_t   _hidReportSize;

protected:
    int buildAndSet16BitValue(bool includeValue, JOYSTICK_AXIS_DATATYPE value, uint8_t dataLocation[]);

public:
    Joystick_(
        uint8_t hidReportId = JOYSTICK_DEFAULT_REPORT_ID,
        uint8_t joystickType = JOYSTICK_TYPE_JOYSTICK,
        uint8_t buttonCount = JOYSTICK_DEFAULT_BUTTON_COUNT,
        uint8_t hatSwitchCount = JOYSTICK_DEFAULT_HATSWITCH_COUNT,
        bool includeXAxis = true,
        bool includeYAxis = true,
        bool includeZAxis = true,
        bool includeRxAxis = true,
        bool includeRyAxis = true,
        bool includeRzAxis = true,
        bool includeRudder = false,
        bool includeThrottle = false,
        bool includeAccelerator = false,
        bool includeBrake = false,
        bool includeSteering = false);

    void begin(bool initAutoSendState = true);
    void end();

    // Set Axis Values
    void setXAxis(JOYSTICK_AXIS_DATATYPE value);
    void setYAxis(JOYSTICK_AXIS_DATATYPE value);
    void setZAxis(JOYSTICK_AXIS_DATATYPE value);
    void setRxAxis(JOYSTICK_AXIS_DATATYPE value);
    void setRyAxis(JOYSTICK_AXIS_DATATYPE value);
    void setRzAxis(JOYSTICK_AXIS_DATATYPE value);

    // Set Simulation Values
    void setRudder(JOYSTICK_AXIS_DATATYPE value);      // 方向舵 
    void setThrottle(JOYSTICK_AXIS_DATATYPE value);    // 油门 
    void setAccelerator(JOYSTICK_AXIS_DATATYPE value); // 加速器 
    void setBrake(JOYSTICK_AXIS_DATATYPE value);       // 刹车 
    void setSteering(JOYSTICK_AXIS_DATATYPE value);    // 转向 

    // Set Button Values 
    void setButton(uint8_t button, uint8_t value);
    void pressButton(uint8_t button);
    void releaseButton(uint8_t button);

    // Set Hat Values 
    void setHatSwitch(int8_t hatSwitch, int16_t value);

    void sendState();
};

#endif // !defined(_USING_DYNAMIC_HID)
#endif // JOYSTICK_h
