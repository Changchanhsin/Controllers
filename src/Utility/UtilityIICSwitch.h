#ifndef Utility_IICSwitch_h
#define Utility_IICSwitch_h

#include "Wire.h"

class UtilityIICSwitch{
public:
//  bool needWire[2]={false,false};
//  inline void setChannel(uint8_t ch, bool v){
//    needWire[ch-1]=v;
//  };
  inline void changeChannel(uint8_t ch){
//    if(needWire[ch-1]==true){
    Wire.beginTransmission(0x70);
    Wire.write(ch);
    Wire.endTransmission();
//    }
  };
};

#endif