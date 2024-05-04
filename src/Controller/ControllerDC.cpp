#include "Arduino.h"
#include "ControllerDC.h"

SegaDreamcast::SegaDreamcast(){
  pin_mode[0] = INPUT;   // DC_PIN_D1
  pin_mode[1] = INPUT;   // DC_PIN_D5
  pin_mode[2] = INPUT;   // 
  pin_mode[3] = INPUT;   // 
  pin_mode[4] = INPUT;   // 
  pin_mode[5] = INPUT;   // DC_PIN_SENSOR
  pin_mode[6] = INPUT;   // 
  return;
}

void SegaDreamcast::get(void){
}
