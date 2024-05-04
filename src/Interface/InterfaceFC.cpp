#include "Arduino.h"
#include "InterfaceFC.h"

void InterfaceFC::init(uint8_t pinData, uint8_t pinSensor, uint8_t pinLatch, uint8_t pinClock, uint8_t pinTrigger, uint8_t pinMic){
  FC_DATA    = pinData;
  FC_SENSOR  = pinSensor;
  FC_LATCH   = pinLatch;
  FC_CLOCK   = pinClock;
  FC_TRIGGER = pinTrigger;
  FC_MIC     = pinMic;

  pinMode(FC_DATA,    INPUT_PULLUP);
  pinMode(FC_SENSOR,  INPUT_PULLUP);
  pinMode(FC_LATCH,   OUTPUT);
  pinMode(FC_CLOCK,   OUTPUT);
  pinMode(FC_TRIGGER, INPUT_PULLUP);
  pinMode(FC_MIC,     INPUT_PULLUP);
  delay(30);

  digitalWrite(FC_LATCH, LOW);
  digitalWrite(FC_CLOCK, LOW);
}

void InterfaceFC::get(void){
  uint8_t i;

  digitalWrite(FC_LATCH, HIGH);
  delayMicroseconds(12);
  digitalWrite(FC_LATCH, LOW);
  delayMicroseconds(6000);
  for(i=0; i<16; i++){
    raw_dump[i] = digitalRead(FC_DATA);
//    mic_dump[i] = digitalRead(FC_MIC);
//    trigger_dump[i] = digitalRead(FC_TRIGGER);
//    sensor_dump[i] = digitalRead(FC_SENSOR);
    digitalWrite(FC_CLOCK, HIGH);
    delayMicroseconds(6);
    digitalWrite(FC_CLOCK, LOW);
    delayMicroseconds(6);
  }
  delayMicroseconds(10466);
}
