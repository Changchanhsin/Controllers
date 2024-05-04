#include "Arduino.h"
#include "ControllerWII.h"

inline boolean NintendoWII::writePointer(byte ptr, boolean delay = true) {
  Wire.beginTransmission(WII_ADDRESS);
  Wire.write(ptr);
  if (Wire.endTransmission() != 0)
    return false;  // 0 = No Error
  if(delay)
    delayMicroseconds(WII_CONVERSION_DEALAY);  // Wait for data conversion
  return true;
}

inline boolean NintendoWII::writeRegister(byte reg, byte value, boolean delay = true) {
  Wire.beginTransmission(WII_ADDRESS);
  Wire.write(reg);
  Wire.write(value);
  if (Wire.endTransmission() != 0)
    return false;  // 0 = No Error
  if (delay)
    delayMicroseconds(WII_CONVERSION_DEALAY);  // Wait for data conversion
  return true;
}

inline boolean NintendoWII::requestMultiple(uint8_t requestSize, uint8_t * dataOut) {
  uint8_t nBytesRecv = Wire.readBytes(dataOut, Wire.requestFrom(WII_ADDRESS, requestSize));
  return (nBytesRecv == requestSize);  // Success if all bytes received
}

inline boolean NintendoWII::readDataArray(byte ptr, uint8_t requestSize, uint8_t * dataOut) {
  if (!writePointer(ptr))
    return false;  // Set start for data read
  return requestMultiple(requestSize, dataOut);
}

inline boolean NintendoWII::readRegister(byte reg, uint8_t* dataOut) {
  return readDataArray(reg, 1, dataOut);  // read one register
}

void NintendoWII::begin(){
  /* Initialization for unencrypted communication.
   * This *should* work on all devices, genuine + 3rd party.
   * See http://wiibrew.org/wiki/Wiimote/Extension_Controllers
   */
  Wire.begin();
  if (!writeRegister(0xF0, 0x55, false))
    return false;
  delay(10);
  if (!writeRegister(0xFB, 0x00, false))
    return false;
  delay(20);
  if (!readDataArray(0xFA, 6, type))
    return false;
  if (type[2] == 0xA4 && type[3] == 0x20) {  // All valid IDs
    // Nunchuk ID: 0x0000
    if (type[4] == 0x00 && type[5] == 0x00) {
      subtype = WII_SUBTYPE_NUNCHUCK;
    }
    /* Classic Con. ID: 0x0[01] 0x00 0xA4 0x20 0x## 0x01
     *   Where ## =
     *     0x00 - Double initialized. (shouldn't happen in normal use)
     *     0x01 - Standard data mode
     *     0x02 - ??? Reports data, but not in a known format
     *     0x03 - "High resolution" mode, used by mini consoles
     */
    else if (type[0] <= 0x01 && type[1] == 0x00
          && type[4] <= 0x03 && type[5] == 0x01) {
      subtype = WII_SUBTYPE_CLASSIC;
    }
    // Guitar Hero Controllers: 0x##00, 0xA420, 0x0103
    else if (type[1] == 0x00 && type[4] == 0x01 && type[5] == 0x03) {
      // Guitar: 0x00
      if (type[0] == 0x00) {
        subtype = WII_SUBTYPE_GUITAR;
      }
      // Drums: 0x01
      else if (type[0] == 0x01) {
        subtype = WII_SUBTYPE_DRUM;
      }
      // DJ Turntable: 0x03
      else if (type[0] == 0x03) {
        subtype = WII_SUBTYPE_DJTURNTABLE;
      }
    }
    // uDraw Tablet Con. ID: 0x0112
    else if (type[4] == 0x01 && type[5] == 0x12) {
      subtype = WII_SUBTYPE_UDRAWTABLET;
    }
    // Drawsome Tablet Con. ID: 0x0013
    else if (type[4] == 0x00 && type[5] == 0x13) {
      subtype = WII_SUBTYPE_DRAWSOMETABLET;
    }
    else{
      subtype = WII_SUBTYPE_UNKNOWN;  // No matches
    }
  }
  else{
    subtype = WII_SUBTYPE_NOCONCETED;
  }
/*
  Serial.print(type[0],HEX);
  Serial.print(type[1],HEX);
  Serial.print(type[2],HEX);
  Serial.print(type[3],HEX);
  Serial.print(type[4],HEX);
  Serial.println(type[5],HEX);
  Serial.println(subtype);
  */
}

void NintendoWII::get(void){
  /*
  Serial.print(data[0],HEX);
  Serial.print(data[1],HEX);
  Serial.print(data[2],HEX);
  Serial.print(data[3],HEX);
  Serial.print(data[4],HEX);
  Serial.println(data[5],HEX);
  */
  switch(subtype){
    case WII_SUBTYPE_NUNCHUCK:
      /* Nunchuck
       *     7   6   5   4   3   2   1   0
       * 0   Joy X<7:0>
       * 1   Joy Y<7:0>
       * 2   Accel X<9:2>
       * 3   Accel Y<9:2>
       * 4   Accel Z<9:2>
       * 5   AZ<0:1> AY<0:1> AX<0:1> BC BZ
       */
      readDataArray(0x00, 6, data);
      keys->buttons_value[WII_Z] = bitRead(data[5], 0);
      keys->buttons_value[WII_C] = bitRead(data[5], 1);
      keys->axis_value[GAMEPAD_AXIS_X] = map(data[0], 0, 0xFF, AXIS_MIN, AXIS_MAX);
      keys->axis_value[GAMEPAD_AXIS_Y] = map(data[1], 0, 0xFF, AXIS_MAX, AXIS_MIN);
      keys->axis_value[GAMEPAD_AXIS_RX] = map(((data[2]<<2)|((data[5]&0b00001100)>>2)), 0, 1024, AXIS_MIN, AXIS_MAX);
      keys->axis_value[GAMEPAD_AXIS_RY] = map(((data[3]<<2)|((data[5]&0b00110000)>>4)), 0, 1024, AXIS_MAX, AXIS_MIN);
      keys->axis_value[GAMEPAD_AXIS_RZ] = map(((data[4]<<2)|((data[5]&0b11000000)>>6)), 0, 1024, AXIS_MIN, AXIS_MAX);
      break;
    case WII_SUBTYPE_CLASSIC:
      /* Classic Controller "Standard" Mode
       *     7   6   5   4   3   2   1   0
       * 0   RX<4:3> LX <5:0> 
       * 1   RX<2:1> LY <5:0>
       * 2   RX<0>   LT<4:3> RY<4:0>
       * 3   LT<2:0> RT<4:0>
       * 4   BDR BDD BLT B-  BH  B+  BRT 1
       * 5   BZL BB  BY  BA  BX  BZR BDL BDU
       */
      // LX = data[0]&0b00111111
      // LY = data[1]&0b00111111
      // RX = (((data[0]&0b11000000)>>3) | ((data[1]&0b11000000)>>5) | ((data[2]&0b10000000)>>7))
      // RY = data[2]&0b00011111
      // LT = data[2]&0b01100000 | ((data[3]&0b11100000)>>5)
      // RT = data[3]&0b00011111
      readDataArray(0x00, 6, data);
      keys->buttons_value[WII_UP]    = bitRead(data[5], 0);
      keys->buttons_value[WII_DOWN]  = bitRead(data[4], 6);
      keys->buttons_value[WII_LEFT]  = bitRead(data[5], 1);
      keys->buttons_value[WII_RIGHT] = bitRead(data[4], 7);
      keys->buttons_value[WII_A]     = bitRead(data[5], 4);
      keys->buttons_value[WII_B]     = bitRead(data[5], 6);
      keys->buttons_value[WII_X]     = bitRead(data[5], 3);
      keys->buttons_value[WII_Y]     = bitRead(data[5], 5);
      keys->buttons_value[WII_MINUS] = bitRead(data[4], 4);
      keys->buttons_value[WII_HOME]  = bitRead(data[4], 3);
      keys->buttons_value[WII_PLUS]  = bitRead(data[4], 2);
      keys->buttons_value[WII_L]     = bitRead(data[4], 5);
      keys->buttons_value[WII_R]     = bitRead(data[4], 1);
      keys->buttons_value[WII_ZL]    = bitRead(data[5], 7);
      keys->buttons_value[WII_ZR]    = bitRead(data[5], 2);
      keys->axis_value[GAMEPAD_AXIS_X]  = map(data[0]&0b00111111, 0, 0b00111111, AXIS_MIN, AXIS_MAX);
      keys->axis_value[GAMEPAD_AXIS_Y]  = map(data[1]&0b00111111, 0, 0b00111111, AXIS_MAX, AXIS_MIN);
      keys->axis_value[GAMEPAD_AXIS_RX] = map((((data[0]&0b11000000)>>3) | ((data[1]&0b11000000)>>5) | ((data[2]&0b10000000)>>7)), 0, 0b00011111, AXIS_MIN, AXIS_MAX);
      keys->axis_value[GAMEPAD_AXIS_RY] = map(data[2]&0b00011111, 0, 0b00011111, AXIS_MAX, AXIS_MIN);
      keys->axis_value[GAMEPAD_AXIS_Z]  = map(((data[2]&0b01100000)>>2)|((data[3]&0b11100000)>>5), 0, 0b00011111, AXIS_MIN, AXIS_MAX);
      keys->axis_value[GAMEPAD_AXIS_RZ] = map(data[3]&0b00011111, 0, 0b00011111, AXIS_MIN, AXIS_MAX);
      break;
//    case WII_SUBTYPE_CLASSICHIGH:
      /* Classic Controller "High Resolution" Mode
       *     7   6   5   4   3   2   1   0
       * 0   LX<7:0>
       * 1   RX<7:0>
       * 2   LY<7:0>
       * 3   RY<7:0>
       * 4   LT<7:0>
       * 5   RT<7:0>
       * 6   BDR BDD BLT B-  BH  B+  BRT 1
       * 7   BZL BB  BY  BA  BX  BZR BDL BDU
       */
//      break;
  }
}
