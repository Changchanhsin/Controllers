#include "Arduino.h"
#include "Controllers.h"

Controllers::Controllers() {
  joypadType = NO_JOYPAD;
}

uint8_t Controllers::analogToOct(int analogValue) {
  //      --------- 平均 ---------   ------ 工业 ------
  //  0 = 0.0 | 0.0..0.3 |   0- 71   GND  |       0-150
  //  1 = 0.7 | 0.4..1.0 |  72-218   1V   | 151-204-250
  //  2 = 1.4 | 1.1..1.7 | 219-364   1.5V | 251-307-350
  //  3 = 2.1 | 1.8..2.4 | 365-511   2.4V | 451-492-550
  //  4 = 2.8 | 2.5..3.1 | 512-657   3V   | 561-614-630
  //  5 = 3.5 | 3.2..3.8 | 658-803   3.3V | 631-675-720
  //  6 = 4.2 | 3.9..4.5 | 804-549   4.2V | 811-860-910
  //  7 = 5.0 | 4.7..5.0 | 950-1023  5V   | 910-1023
  if(analogValue <= 81)  return 0;
  if(analogValue <= 214) return 1;
  if(analogValue <= 316) return 2;
  if(analogValue <= 419) return 3;
  if(analogValue <= 572) return 4;
  if(analogValue <= 747) return 5;    //811
  if(analogValue <= 920) return 6;
  return 7;
}

uint8_t Controllers::checkJoypad(void) {
  uint8_t ret;

  // 两个模拟端获得类型
  ret = analogToOct(analogRead(joypadTypePin[0]));
  ret = (ret << 3) | analogToOct(analogRead(joypadTypePin[1]));
  return ret;
}

void Controllers::saveJoypadKey(void) {
  memcpy(keys->buttons_value, keysNow.buttons_value, sizeof(keysNow.buttons_value));
  memcpy(keys->axis_value,    keysNow.axis_value,    sizeof(keysNow.axis_value   ));
}

uint8_t Controllers::checkJoypadType(void) {
//  Serial.print("check ");
//  Serial.println(joypadType);
  uint8_t newType = checkJoypad();
  uint8_t ret = -1;

  if (newType != joypadType) {
    delay(200);
    // recheck again to debounce
//    Serial.println("recheck");
    joypadType = checkJoypad();
    ret = joypadType;
    clearJoypadKey();
    //    reportStatus((char*)"Analog Values", analogRead(joypadTypePin[0]), analogRead(joypadTypePin[1]));
    //    reportStatus((char*)"Type : ", joypadType);
    switch (joypadType) {
#ifdef USES_ATARI
      case ATARI_A2600:
        atari.begin();
//        reportStatus((char*)"Atari 2600");
        break;
#endif
#ifdef USES_SS
      case SEGA_SATURN:
        ss.begin();
//        reportStatus((char*)"Sega Saturn");
        break;
#endif
#ifdef USES_MD
      case SEGA_MEGADRIVE:
        md.begin();
//        reportStatus((char*)"Sega Megadrive");
        break;
#endif
#ifdef USES_SFC
      case NINTENDO_SFC:
        sfc.begin();
//        reportStatus((char*)"Nintendo Superfamicon");
        break;
#endif
#ifdef USES_FC
      case NINTENDO_FC:
        fc.begin();
//        reportStatus((char*)"Nintendo Famicon");
        break;
#endif
#ifdef USES_N64
      case NINTENDO_N64:
        n64.begin();
//        reportStatus((char*)"Nintendo 64");
        break;
#endif
#ifdef USES_NGC
      case NINTENDO_NGC:
        ngc.begin();
//        reportStatus((char*)"Nintendo GameCube");
        break;
#endif
#ifdef USES_WII
      case NINTENDO_WII:
        wii.begin();
//        reportStatus((char*)"Nintendo WII");
        break;
#endif
#ifdef USES_PS
      case SONY_PLAYSTATION:
        ps.begin();
//        reportStatus((char*)"Sony Playstation");
        break;
#endif
      case NO_JOYPAD:
        nc.begin();
        nc.get();
//        reportStatus((char*)"Disconected");
        break;
    }
  }
  return ret;
}

void Controllers::clearJoypadKey(void) {
  nc.clean();
}
/*
void Controllers::reportStatus(char* str) {
#ifdef DEBUG_ME
  Serial.print("(");
  Serial.print(id);
  Serial.print(") ");
  Serial.println(str);
#endif
}

void Controllers::reportStatus(char* str, int intVal) {
#ifdef DEBUG_ME
  Serial.print(str);
  Serial.print(" : ");
  Serial.println(intVal, HEX);
#endif
}

void Controllers::reportStatus(char* str, int strVal, int intVal) {
#ifdef DEBUG_ME
  Serial.print(str);
  Serial.print(" : ");
  Serial.print(strVal, HEX);
  Serial.print(" ");
  Serial.println(intVal, HEX);
#endif
}

void Controllers::reportPINsTitle(void) {
#ifdef DEBUG_ME_DETAIL
  Serial.println("0 1 2 3 4 5 6 - 7 8 9 A B C D ");
  Serial.println("----------------------------");
#endif
}

void Controllers::reportPINs(void) {
#ifdef DEBUG_ME_DETAIL
  for (int i = 0; i < 14; i++) {
    if (digitalRead(i) == LOW)
      Serial.print("| ");
    else
      Serial.print("# ");
    if (i == 6) {
      Serial.print("  ");
    }
  }
  Serial.println("");
#endif
}
*/
void Controllers::setup(char* cid, uint8_t* pins, uint8_t* apins, Keys* k) {
  id = cid;
  joypadTypePin[0] = apins[0];
  joypadTypePin[1] = apins[1];
  keys = k;
  nc.init(pins, k);
#ifdef USES_ATARI
  atari.init(pins, k);
#endif
#ifdef USES_FC
  fc.init(pins, k);
#endif
#ifdef USES_SFC
  sfc.init(pins, k);
#endif
#ifdef USES_MD
  md.init(pins, k);
#endif
#ifdef USES_SS
  ss.init(pins, k);
#endif
#ifdef USES_N64
  n64.init(pins, k);
#endif
#ifdef USES_NGC
  ngc.init(pins, k);
#endif
#ifdef USES_WII
  wii.init(pins, k);
#endif
#ifdef USES_PS
  ps.init(pins, k);
#endif
}

void Controllers::getKey(void) {
  switch (joypadType) {
#ifdef USES_ATARI
    case ATARI_A2600:
      atari.get();
//      subtype=atari.subtype;
      break;
#endif
#ifdef USES_SFC
    case NINTENDO_SFC:
      sfc.get();
//      subtype=sfc.subtype;
      break;
#endif
#ifdef USES_FC
    case NINTENDO_FC:
      fc.get();
//      subtype=fc.subtype;
      break;
#endif
#ifdef USES_N64
    case NINTENDO_N64:
      n64.get();
//      subtype=n64.subtype;
      break;
#endif
#ifdef USES_NGC
    case NINTENDO_NGC:
      ngc.get();
//      subtype=ngc.subtype;
      break;
#endif
#ifdef USES_WII
    case NINTENDO_WII:
      wii.get();
      subtype=wii.subtype;
      break;
#endif
#ifdef USES_MD
    case SEGA_MEGADRIVE:
      md.get();
      subtype=md.subtype;
      break;
#endif
#ifdef USES_SS
    case SEGA_SATURN:
      ss.get();
      subtype=ss.subtype;
      break;
#endif
#ifdef USES_DC
    case SEGA_DREAMCAST:
      //      DC_getKey(joypadNow[i], i);
      break;
#endif
#ifdef USES_PS
    case SONY_PLAYSTATION:
      ps.get();
      subtype=ps.subtype;
      break;
#endif
    case NO_JOYPAD:
      nc.get();
      subtype=0;
      break;
  }
}

uint16_t Controllers::loop(bool* changed) {
  uint8_t i;

  checkJoypadType();
  getKey();
  for (i = 0; i < 40; i++) {
    changed[i+1] = ifChanged(keys->buttons_value[i], keysNow.buttons_value[i]);
    changed[0] |= changed[i+1];
  }
  for (i = 0; i < 6; i++) {
    changed[i + 41] = (keys->axis_value[i], keysNow.axis_value[i]);
    changed[0] |= changed[i+41];
  }
  return joypadType<<8 | subtype;
}
