#include "Arduino.h"
#include "ControllerPS.h"

#define ANALOG2DIGITAL(val,key1,key2)    if((val)<0x40){(key1)=(key1)|1;}else if((val)>0xB0){(key2)=(key2)|1;}

SonyPlaystation::SonyPlaystation(){
  pin_mode[0] = INPUT_PULLUP; // PS_PIN_DATA
  pin_mode[1] = INPUT;        // PS_PIN_ACK
  pin_mode[2] = OUTPUT;       // PS_PIN_ATT
  pin_mode[3] = INPUT_PULLUP;
  pin_mode[4] = INPUT_PULLUP;
  pin_mode[5] = OUTPUT;       // PS_PIN_CMD
  pin_mode[6] = OUTPUT;       // PS_PIN_CLK
  return;
}

void SonyPlaystation::begin(void){
  Controller::begin();
  digitalWrite(pin_number[PS_PIN_CMD], HIGH);
  digitalWrite(pin_number[PS_PIN_CLK], HIGH);
  digitalWrite(pin_number[PS_PIN_ATT], HIGH);
  delayMicroseconds(50);
}

uint8_t SonyPlaystation::exchangeData(uint8_t cmd){
  uint8_t i,ret;

  ret = 0;
  for(i=0; i<8; i++){
    digitalWrite(pin_number[PS_PIN_CLK], LOW);
    delayMicroseconds(2);
    if(bitRead(cmd, i)){
      digitalWrite(pin_number[PS_PIN_CMD], HIGH);
    }else{
      digitalWrite(pin_number[PS_PIN_CMD], LOW);
    }
    delayMicroseconds(18);
    digitalWrite(pin_number[PS_PIN_CLK], HIGH);
    delayMicroseconds(2);
    if(digitalRead(pin_number[PS_PIN_DATA])){
      bitSet(ret, i);
    }
    delayMicroseconds(2);
  }
  delayMicroseconds(50);  // waiting and pass ACK
  return ret;
}

void SonyPlaystation::get_analogs(void){
  uint8_t ret;
  ret = exchangeData(0xFF); // analog RJ.LR
  keys->axis_value[GAMEPAD_AXIS_RX] = map(ret, PS_AXIS_MIN, PS_AXIS_MAX, AXIS_MIN, AXIS_MAX);
  ret = exchangeData(0xFF); // analog RJ.UD
  keys->axis_value[GAMEPAD_AXIS_RY] = map(ret, PS_AXIS_MIN, PS_AXIS_MAX, AXIS_MIN, AXIS_MAX);
  ret = exchangeData(0xFF); // analog LJ.LR
  keys->axis_value[GAMEPAD_AXIS_X]  = map(ret, PS_AXIS_MIN, PS_AXIS_MAX, AXIS_MIN, AXIS_MAX);
  ret = exchangeData(0xFF); // analog LJ.UD
  keys->axis_value[GAMEPAD_AXIS_Y]  = map(ret, PS_AXIS_MIN, PS_AXIS_MAX, AXIS_MIN, AXIS_MAX);
}

void SonyPlaystation::get(void){
  uint8_t ret;

  digitalWrite(pin_number[PS_PIN_CMD], HIGH);
  digitalWrite(pin_number[PS_PIN_CLK], HIGH);
  digitalWrite(pin_number[PS_PIN_ATT], LOW );
  delayMicroseconds(50);

  exchangeData(0x01);
  subtype = exchangeData(0x42);
  switch(subtype){
  case PS_SUBTYPE_DIGITAL:  // normal digital = 0x41
    if(exchangeData(0x00) == 0x5A){
//    {PS_SELECT, PS_NONE, PS_NONE, PS_START, PS_UP, PS_RIGHT, PS_DOWN, PS_LEFT},
//    {PS_L2,     PS_R2,   PS_L1,   PS_R1,    PS_X,  PS_A,     PS_B,    PS_Y   },
      ret = exchangeData(0xFF);
      keys->buttons_value[PS_SELECT] = bitRead(ret, 0);
      keys->buttons_value[PS_START ] = bitRead(ret, 3);
      keys->buttons_value[PS_UP    ] = bitRead(ret, 4);
      keys->buttons_value[PS_RIGHT ] = bitRead(ret, 5);
      keys->buttons_value[PS_DOWN  ] = bitRead(ret, 6);
      keys->buttons_value[PS_LEFT  ] = bitRead(ret, 7);
      ret = exchangeData(0xFF);
      keys->buttons_value[PS_L2    ] = bitRead(ret, 0);
      keys->buttons_value[PS_R2    ] = bitRead(ret, 1);
      keys->buttons_value[PS_L1    ] = bitRead(ret, 2);
      keys->buttons_value[PS_R1    ] = bitRead(ret, 3);
      keys->buttons_value[PS_X     ] = bitRead(ret, 4);
      keys->buttons_value[PS_A     ] = bitRead(ret, 5);
      keys->buttons_value[PS_B     ] = bitRead(ret, 6);
      keys->buttons_value[PS_Y     ] = bitRead(ret, 7);
      keys->axis_value[GAMEPAD_AXIS_X]=AXIS_MID;
      keys->axis_value[GAMEPAD_AXIS_Y]=AXIS_MID;
      keys->axis_value[GAMEPAD_AXIS_RX]=AXIS_MID;
      keys->axis_value[GAMEPAD_AXIS_RY]=AXIS_MID;
    }
    break;
  case PS_SUBTYPE_RED:  // analog RED = 0x73
    if(exchangeData(0x00) == 0x5A){
//    {PS_SELECT, PS_LJ,   PS_RJ,   PS_START, PS_UP, PS_RIGHT, PS_DOWN, PS_LEFT},
//    {PS_L2,     PS_R2,   PS_L1,   PS_R1,    PS_X,  PS_A,     PS_B,    PS_Y   },
      ret = exchangeData(0xFF);
      keys->buttons_value[PS_SELECT] = bitRead(ret, 0);
      keys->buttons_value[PS_LJ    ] = bitRead(ret, 1);
      keys->buttons_value[PS_RJ    ] = bitRead(ret, 2);
      keys->buttons_value[PS_START ] = bitRead(ret, 3);
      keys->buttons_value[PS_UP    ] = bitRead(ret, 4);
      keys->buttons_value[PS_RIGHT ] = bitRead(ret, 5);
      keys->buttons_value[PS_DOWN  ] = bitRead(ret, 6);
      keys->buttons_value[PS_LEFT  ] = bitRead(ret, 7);
      ret = exchangeData(0xFF);
      keys->buttons_value[PS_L2    ] = bitRead(ret, 0);
      keys->buttons_value[PS_R2    ] = bitRead(ret, 1);
      keys->buttons_value[PS_L1    ] = bitRead(ret, 2);
      keys->buttons_value[PS_R1    ] = bitRead(ret, 3);
      keys->buttons_value[PS_X     ] = bitRead(ret, 4);
      keys->buttons_value[PS_A     ] = bitRead(ret, 5);
      keys->buttons_value[PS_B     ] = bitRead(ret, 6);
      keys->buttons_value[PS_Y     ] = bitRead(ret, 7);
      get_analogs();
    }
    break;
/*
  case PS_SUBTYPE_GREEN:  // analog GREEN = 0x53
    if(exchangeData(0x00) == 0x5A){
//      get_buttons(4);
    {PS_NONE,   PS_NONE, PS_NONE, PS_START, PS_UP, PS_RIGHT, PS_DOWN, PS_LEFT},
    {PS_L2,     PS_L1,   PS_Y,    PS_X,     PS_R1, PS_A,     PS_B,    PS_R2  }
      ret = exchangeData(0xFF);
      keys->buttons_value[PS_START ] = bitRead(ret, 3);
      keys->buttons_value[PS_UP    ] = bitRead(ret, 4);
      keys->buttons_value[PS_RIGHT ] = bitRead(ret, 5);
      keys->buttons_value[PS_DOWN  ] = bitRead(ret, 6);
      keys->buttons_value[PS_LEFT  ] = bitRead(ret, 7);
      ret = exchangeData(0xFF);
      keys->buttons_value[PS_L2    ] = bitRead(ret, 0);
      keys->buttons_value[PS_L1    ] = bitRead(ret, 1);
      keys->buttons_value[PS_Y     ] = bitRead(ret, 2);
      keys->buttons_value[PS_X     ] = bitRead(ret, 3);
      keys->buttons_value[PS_R1    ] = bitRead(ret, 4);
      keys->buttons_value[PS_A     ] = bitRead(ret, 5);
      keys->buttons_value[PS_B     ] = bitRead(ret, 6);
      keys->buttons_value[PS_R2    ] = bitRead(ret, 7);
      get_analogs();
    }
    break;
*/
  }
  digitalWrite(pin_number[PS_PIN_ATT], HIGH);
}
