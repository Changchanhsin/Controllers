#include "HID-Project.h"

#define DEBUG_ME
#define DEBUG_MESSAGE

//  1s  = 1000ms (millsecond )
//  1ms = 1000us (microsecond)

#define PINOUTPUT(pinNumber,outputValue,outputDelay)   digitalWrite((pinNumber), (outputValue)); delayMicroseconds((outputDelay));
#define ANALOG2DIGITAL(val,key1,key2)    if((val)<0x40){(key1)=(key1)|1;}else if((val)>0xB0){(key2)=(key2)|1;}

// Types
#define NO_JOYPAD              0x00
#define ATARI_ATARI            0x05
#define NINTENDO_SUPERFAMICON  0x07
#define NINTENDO_N64_GC        0x57
#define NINTENDO_N64             0x005
#define NINTENDO_GAMECUBE        0x050
#define SEGA_SATURN            0x55
#define SEGA_MEGADRIVE         0x50
#define SEGA_DREAMCAST         0x75
//#define SEGA_DREAMCAST           0x007
#define PANASONIC_3DO            0x070
#define SONY_PLAYSTATION       0x70
const int joypadPin[2][7] = {
  { 0, 1, 2, 3, 4, 5, 6 },
  { 7, 8, 9,10,11,12,13 }
};
int joypadType[2];
int joypadSubType[2];

// Keymap
#define JOYPAD_UP     0
#define JOYPAD_DOWN   1
#define JOYPAD_LEFT   2
#define JOYPAD_RIGHT  3
#define JOYPAD_B      4
#define JOYPAD_A      5
#define JOYPAD_Y      6
#define JOYPAD_X      7
#define JOYPAD_C      8
#define JOYPAD_Z      9
#define JOYPAD_L1     8
#define JOYPAD_R1     9
#define JOYPAD_SELECT 10
#define JOYPAD_START  11
#define JOYPAD_L      12
#define JOYPAD_L2     12
#define JOYPAD_R      13
#define JOYPAD_R2     13
#define JOYPAD_NONE   16
boolean joypadOld[2][17];
boolean joypadNow[2][17];
const char joypadKeymap[2][17] = {
  { KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_RETURN, ' ',  'c', 'x', 'z', 'd', 's', 'a', 'q', 'w', 'e', 'r'  },
  { 'p',          ';',            'l',            '\'',            ']',        '\\', 'k', 'j', 'h', 'i', 'u', 'y', '6', '7', '8', '9'  }
};

void clearJoypadKey(int player){
  int i;

  for(i=0; i<16; i++){
    joypadOld[player][i] = false;
    joypadNow[player][i] = false;
    Gamepad.releaseAll();
    Gamepad.write();
  }
}

void saveJoypadKey(){
  int i;

  for(i=0; i<16; i++){
    joypadOld[0][i] = joypadNow[0][i];
    joypadOld[1][i] = joypadNow[1][i];
  }
}

void sendJoypadKey(void)
{
  char c;
  int i,j;

  j=0;
    if(joypadType[j]!=JOYPAD_NONE){
      if(joypadNow[j][3]==1){
        Gamepad.xAxis(0x7FFF);
      }else if(joypadNow[j][2]==1){
        Gamepad.xAxis(0x8000);
      }else{
        Gamepad.xAxis(0x0000);
      }
      if(joypadNow[j][1]==1){
        Gamepad.yAxis(0x7FFF);
      }else if(joypadNow[j][0]==1){
        Gamepad.yAxis(0x8000);
      }else{
        Gamepad.yAxis(0x0000);
      }
      for(i=4; i<16; i++){
        if(joypadOld[j][i] != joypadNow[j][i]){
          if(joypadNow[j][i]==true){
            Gamepad.press(i-3);
#ifdef DEBUG_MESSAGE
            reportStatus("Button pressed", joypadKeymap[j][i]);
#endif
          }else{
            Gamepad.release(i-3);
#ifdef DEBUG_MESSAGE
            reportStatus("Button released", joypadKeymap[j][i]);
#endif
          }
        }
      }
      Gamepad.write();
    }
}

// Pinmap
int pinmapNC[]     = { INPUT_PULLUP, INPUT_PULLUP, INPUT_PULLUP, INPUT_PULLUP, INPUT_PULLUP, INPUT_PULLUP, INPUT_PULLUP };
#define PINMAP_PS_DATA 0  // INPUT   brown
#define PINMAP_PS_ACK  1  // INPUT   green
#define PINMAP_PS_ATT  2  // OUTPUT  yellow    SELECT LOW enabled
#define PINMAP_PS_CMD  5  // OUTPUT  orange
#define PINMAP_PS_CLK  6  // OUTPUT  blue
int pinmapPS[]     = { INPUT, INPUT, OUTPUT, INPUT_PULLUP, INPUT_PULLUP, OUTPUT, OUTPUT };
#define PINMAP_MD_D0  0  // INPUT
#define PINMAP_MD_D1  1  // INPUT
#define PINMAP_MD_D2  2  // INPUT
#define PINMAP_MD_D3  3  // INPUT
#define PINMAP_MD_D4  4  // INPUT
#define PINMAP_MD_D5  5  // INPUT
#define PINMAP_MD_SEL 6  // OUTPUT
int pinmapMD[]     = { INPUT, INPUT, INPUT, INPUT, INPUT, INPUT, OUTPUT };
#define PINMAP_SS_D0 0  // INPUT
#define PINMAP_SS_D1 1  // INPUT
#define PINMAP_SS_D2 2  // INPUT
#define PINMAP_SS_D3 3  // INPUT
#define PINMAP_SS_S0 5  // OUTPUT
#define PINMAP_SS_S1 6  // OUTPUT
int pinmapSS[]     = { INPUT, INPUT, INPUT, INPUT, INPUT_PULLUP, OUTPUT, OUTPUT };
#define PINMAP_DC_D1    0  // BIDIR
#define PINMAP_DC_D5    5  // BIDIR
#define PINMAP_DC_SENSE 2  // UNKNOWN
int pinmapDC[]     = { INPUT, INPUT_PULLUP, INPUT_PULLUP, INPUT_PULLUP, INPUT_PULLUP, OUTPUT, INPUT_PULLUP };
#define PINMAP_SFC_DATA  0  // INPUT
#define PINMAP_SFC_LATCH 2  // OUTPUT
#define PINMAP_SFC_CLOCK 5  // OUTPUT
int pinmapSFC[]    = { INPUT, INPUT_PULLUP, OUTPUT, INPUT_PULLUP, INPUT_PULLUP, OUTPUT, INPUT_PULLUP };
#define PINMAP_N64_DATA 2  // BIDIR
int pinmapN64[]    = { INPUT_PULLUP, INPUT_PULLUP, INPUT, INPUT_PULLUP, INPUT_PULLUP, INPUT_PULLUP, INPUT_PULLUP };
#define PINMAP_GC_DATA  2  // BIDIR
int pinmapGC[]     = { INPUT_PULLUP, INPUT_PULLUP, INPUT, INPUT_PULLUP, INPUT_PULLUP, INPUT_PULLUP, INPUT_PULLUP };

// check and set joypad pinmap, for all type of controller.
int analogToOct(int analogValue)
{
  //      --------- 平均 ---------   ------ 工业 ------
  //  0 = 0.0 | 0.0..0.3 |   0- 71   GND  |       0-150
  //  1 = 0.7 | 0.4..1.0 |  72-218   1V   | 151-204-250
  //  2 = 1.4 | 1.1..1.7 | 219-364   1.5V | 251-307-350
  //  3 = 2.1 | 1.8..2.4 | 365-511   2.4V | 451-492-550
  //  4 = 2.8 | 2.5..3.1 | 512-657   3V   | 561-614-630
  //  5 = 3.5 | 3.2..3.8 | 658-803   3.3V | 631-675-720
  //  6 = 4.2 | 3.9..4.5 | 804-549   4.2V | 811-860-910
  //  7 = 5.0 | 4.7..5.0 | 950-1024  5V   | 910-1023
  if(analogValue < 151) return 0;
//  if(analogValue < 251) return 1;
//  if(analogValue < 451) return 2;
//  if(analogValue < 560) return 3;
//  if(analogValue < 631) return 4;
  if(analogValue < 811) return 5;
//  if(analogValue < 910) return 6;
  return 7;
}

int checkJoypad(int player){
  int ret;

  // reserved A4/A5 for IIC communication.
  if(player ==0){
    ret = analogToOct(analogRead(A1));
    ret = (ret << 4) + analogToOct(analogRead(A0));
  }else{
    ret = analogToOct(analogRead(A3));
    ret = (ret << 4) + analogToOct(analogRead(A2));
  }
  return ret;
}

int checkJoypadType(){
  int i;
  int newType[2];
  
  for(i=0; i<2; i++){
    newType[i] = checkJoypad(i);
    if(newType[i]!=joypadType[i]){
      delay(200);
      // recheck again to debounce
      joypadType[i] = checkJoypad(i);
      clearJoypadKey(i);
#ifdef DEBUG_ME
#ifdef DEBUG_MESSAGE
      if(i==0){
        reportStatusInt("Analog Values", analogRead(A1), analogRead(A0));
        reportStatusInt("Type @ port", joypadType[i], i);
      }else{
        reportStatusInt("Analog Values", analogRead(A3), analogRead(A2));
        reportStatusInt("Type @ port", joypadType[i], i);
      }
#endif
#endif
      switch(joypadType[i]){
      case SEGA_SATURN:
        setPins(pinmapSS,i);
        reportStatus("Controller changed : Sega Saturn", i);
        break;
      case SEGA_MEGADRIVE:
        setPins(pinmapMD,i);
        reportStatus("Controller changed : Sega Megadrive", i);
        break;
      case NINTENDO_SUPERFAMICON:
        setPins(pinmapSFC,i);
        SFC_init(i);
        reportStatus("Controller changed : Nintendo Superfamicon", i);
        break;
      case NINTENDO_N64_GC:
//        setPins(pinmapN64,i);
        reportStatus("Controller changed : Nintendo N64", i);
        break;
      case SONY_PLAYSTATION:
        setPins(pinmapPS,i);
        PS_init(i);
        reportStatus("Controller changed : Sony Playstation", i);
        break;
      case NO_JOYPAD:
        setPins(pinmapNC,i);
        joypadSubType[i]=0;
        reportStatus("Controller changed : disconected", i);
        break;
      }
    }
  }
}

void setPins(int *pinmap, int player){
  int i;

  for(i=0; i<7; i++){
    pinMode(joypadPin[player][i], pinmap[i]);
  }
}

// Get controller keys, for loop
void getKey()
{
  int i;

  saveJoypadKey();
  for(i=0; i<2; i++){
//    reportPINs();
    switch(joypadType[i]){
    case NINTENDO_SUPERFAMICON:
      SFC_getKey(joypadNow[i], i);
      break;
    case NINTENDO_N64_GC:
//      N64_getKey(joypadNow[i], i);
//      GC_getKey(joypadNow[i], i);
      break;
    case SEGA_MEGADRIVE:
      MD_getKey(joypadNow[i], i);
      break;
    case SEGA_SATURN:
      SS_getKey(joypadNow[i], i);
      break;
    case SEGA_DREAMCAST:
//      DC_getKey(joypadNow[i], i);
      break;
    case SONY_PLAYSTATION:
      PS_getKey(joypadNow[i], i);
      break;
    }
  }
}


// Sony Playstation   *** TESTED unOK ***
void PS_init(int player){
  digitalWrite(joypadPin[player][PINMAP_PS_ATT], HIGH);
  digitalWrite(joypadPin[player][PINMAP_PS_CMD], HIGH);
  digitalWrite(joypadPin[player][PINMAP_PS_CLK], HIGH);
}

void PS_getKey(boolean *key, int player){
  int i,j;
  byte ret;

  // A=Cir, B=Cross, X=Tri, Y=Rect, C = analog RJ, Z = analog LJ
  // [0,1]=0x41 [2,3]=0x73; [4,5]=0x53
  int keymap[6][8]={
    {JOYPAD_SELECT, JOYPAD_NONE, JOYPAD_NONE, JOYPAD_START, JOYPAD_UP, JOYPAD_RIGHT, JOYPAD_DOWN, JOYPAD_LEFT    },
    {JOYPAD_L2,     JOYPAD_R2,   JOYPAD_L1,   JOYPAD_R1,    JOYPAD_X,  JOYPAD_A,     JOYPAD_B,    JOYPAD_Y       },
    {JOYPAD_SELECT, JOYPAD_C,    JOYPAD_Z,    JOYPAD_START, JOYPAD_UP, JOYPAD_RIGHT, JOYPAD_DOWN, JOYPAD_LEFT    },
    {JOYPAD_L2,     JOYPAD_R2,   JOYPAD_L1,   JOYPAD_R1,    JOYPAD_X,  JOYPAD_A,     JOYPAD_B,    JOYPAD_Y       },
    {JOYPAD_NONE,   JOYPAD_NONE, JOYPAD_NONE, JOYPAD_START, JOYPAD_UP, JOYPAD_RIGHT, JOYPAD_DOWN, JOYPAD_LEFT    },
    {JOYPAD_L2,     JOYPAD_L1,   JOYPAD_Y,    JOYPAD_X,     JOYPAD_R1, JOYPAD_A,     JOYPAD_B,    JOYPAD_R2      }
  };

  digitalWrite(joypadPin[player][PINMAP_PS_CMD], HIGH);
  digitalWrite(joypadPin[player][PINMAP_PS_CLK], HIGH);
  digitalWrite(joypadPin[player][PINMAP_PS_ATT], LOW);

  delayMicroseconds(10);
  PS_exchangeData(player, 0x01);
  ret = PS_exchangeData(player, 0x42);
  switch(ret){
  case 0x41:  // normal digital
    if(joypadSubType[player]!=ret){
      reportStatus("PS Controller type : Normal digital", player);
      joypadSubType[player]=ret;
    }
    ret = PS_exchangeData(player, 0x00);
    if(ret == 0x5A)
      for(i=0; i<2; i++){
        ret = PS_exchangeData(player, 0xFF);
        for(j=0; j<8; j++)
          key[keymap[i][j]] = 1-bitRead(ret, j);
      }
    break;
  case 0x73:  // analog RED
    if(joypadSubType[player]!=ret){
      reportStatus("PS Controller type : Red analog", player);
      joypadSubType[player]=ret;
    }
    ret = PS_exchangeData(player, 0xFF);
    if(ret == 0x5A){
      for(i=2; i<4; i++){
        ret = PS_exchangeData(player, 0xFF);
        for(j=0; j<8; j++)
          key[keymap[i][j]] = 1-bitRead(ret, j);
      }
      ret = PS_exchangeData(player, 0xFF); // analog RJ.LR
      ANALOG2DIGITAL(ret,key[JOYPAD_Y],key[JOYPAD_A]);
      ret = PS_exchangeData(player, 0xFF); // analog RJ.UD
      ANALOG2DIGITAL(ret,key[JOYPAD_X],key[JOYPAD_B]);
      ret = PS_exchangeData(player, 0xFF); // analog LJ.LR
      ANALOG2DIGITAL(ret,key[JOYPAD_LEFT],key[JOYPAD_RIGHT]);
      ret = PS_exchangeData(player, 0xFF); // analog LJ.UD
      ANALOG2DIGITAL(ret,key[JOYPAD_UP],key[JOYPAD_DOWN]);
    }
    break;
  case 0x53:  // analog GREEN
    if(joypadSubType[player]!=ret){
      reportStatus("PS Controller type : Green analog", player);
      joypadSubType[player]=ret;
    }
    ret = PS_exchangeData(player, 0xFF);
    if(ret == 0x5A){
      for(i=2; i<4; i++){
        ret = PS_exchangeData(player, 0xFF);
        for(j=0; j<8; j++)
          key[keymap[i][j]] = 1-bitRead(ret, j);
      }
      ret = PS_exchangeData(player, 0xFF); // analog RJ.LR
      ANALOG2DIGITAL(ret,key[JOYPAD_Y],key[JOYPAD_A]);
      ret = PS_exchangeData(player, 0xFF); // analog RJ.UD
      ANALOG2DIGITAL(ret,key[JOYPAD_X],key[JOYPAD_B]);
      ret = PS_exchangeData(player, 0xFF); // analog LJ.LR
      ANALOG2DIGITAL(ret,key[JOYPAD_LEFT],key[JOYPAD_RIGHT]);
      ret = PS_exchangeData(player, 0xFF); // analog LJ.UD
      ANALOG2DIGITAL(ret,key[JOYPAD_UP],key[JOYPAD_DOWN]);
    }
    break;
  }
  digitalWrite(joypadPin[player][PINMAP_PS_ATT], HIGH);
}

byte PS_exchangeData(int player, byte cmd){
  byte i,ret,dt[8];
  ret = 0;
  for(i=0; i<8; i++){
    digitalWrite(joypadPin[player][PINMAP_PS_CLK], LOW);
    delayMicroseconds(2);
    if( cmd & (1<<i) )
       digitalWrite(joypadPin[player][PINMAP_PS_CMD], HIGH);
    else
       digitalWrite(joypadPin[player][PINMAP_PS_CMD], LOW);
    delayMicroseconds(18);
    digitalWrite(joypadPin[player][PINMAP_PS_CLK], HIGH);
    delayMicroseconds(2);
    dt[i] = digitalRead(joypadPin[player][PINMAP_PS_DATA]);
    if(dt[i])
      ret = ret | (B00000001 << i);
    delayMicroseconds(18);
  }
/*  Serial.print(dt[7]);
  Serial.print(dt[6]);
  Serial.print(dt[5]);
  Serial.print(dt[4]);
  Serial.print(dt[3]);
  Serial.print(dt[2]);
  Serial.print(dt[1]);
  Serial.print(dt[0]);
*/
  reportStatusInt(" exchange", cmd, ret);
  delayMicroseconds(10);  // waiting and pass ACK
  return ret;
}

// Nintendo SFC   *** TESTED ***
void SFC_init(int player){
  PINOUTPUT(joypadPin[player][PINMAP_SFC_CLOCK], LOW, 0);
  PINOUTPUT(joypadPin[player][PINMAP_SFC_LATCH], LOW, 0);
}

void SFC_getKey(boolean *key, int player){
  int i;
  int joypadmap[]={
    JOYPAD_B,  JOYPAD_Y,    JOYPAD_SELECT, JOYPAD_START, 
    JOYPAD_UP, JOYPAD_DOWN, JOYPAD_LEFT,   JOYPAD_RIGHT, 
    JOYPAD_A,  JOYPAD_X,    JOYPAD_L1,     JOYPAD_R1     };
  
  PINOUTPUT(joypadPin[player][PINMAP_SFC_LATCH], HIGH, 12);
  PINOUTPUT(joypadPin[player][PINMAP_SFC_LATCH], LOW, 6000);
  for(i=0; i<12; i++){
    key[joypadmap[i]] = 1-digitalRead(joypadPin[player][PINMAP_SFC_DATA]);
    PINOUTPUT(joypadPin[player][PINMAP_SFC_CLOCK], HIGH, 6);
    PINOUTPUT(joypadPin[player][PINMAP_SFC_CLOCK], LOW, 6);
  }
  for(i=12; i<15; i++){
    PINOUTPUT(joypadPin[player][PINMAP_SFC_CLOCK], LOW, 6);
    PINOUTPUT(joypadPin[player][PINMAP_SFC_CLOCK], HIGH, 6);
  }
  PINOUTPUT(joypadPin[player][PINMAP_SFC_CLOCK], LOW, 6);
  PINOUTPUT(joypadPin[player][PINMAP_SFC_LATCH], LOW, 10466);
};


// Sega MD   *** TESTED OK ***
//  BE CAREFULL, check logic first, or will misunderstand.
//  Parallel is more dangerous.

void MD_getKey(boolean *key, int player){
  int i;
  boolean k[20];
  int pinout[]={
    PINMAP_MD_D0,PINMAP_MD_D1,PINMAP_MD_D2,PINMAP_MD_D3,PINMAP_MD_D4,PINMAP_MD_D5  };

  // Try HIGH/LOW cycles
  // HIGH first
  PINOUTPUT(joypadPin[player][PINMAP_MD_SEL], HIGH, 4);
  // k0=up, k1=down, k2=left, k3=right, k4=B, k5=C(A)
  for(i=0; i<6; i++){
    k[i] = 1-digitalRead(joypadPin[player][pinout[i]]);
  }
  PINOUTPUT(joypadPin[player][PINMAP_MD_SEL], LOW, 4); 
  // k6=up, k7=down, k8=0, k9=0, k10=A(C), k11=start
  for(i=0; i<6; i++){
    k[i+6] = 1-digitalRead(joypadPin[player][pinout[i]]);
  }
  if(k[6]==1 && k[7]==1 && k[8]==1 && k[9]==1) // is a 6 tirggers controller? need a next circle?
  {
    // Try HIGH/LOW again for external tirggers, this is the forth H/L circle.
    PINOUTPUT(joypadPin[player][PINMAP_MD_SEL], HIGH, 4);
    // k12=Z, k13=Y, k14=X, k15=MODE(SELECT), k16=B, k17=C(A)
    for(i=0; i<6; i++){
      k[i+12] = 1-digitalRead(joypadPin[player][pinout[i]]);
    }
    PINOUTPUT(joypadPin[player][PINMAP_MD_SEL], LOW, 4);
    // for k18=A(C), k19=START, other 4 are ignore.
    k[18] = 1-digitalRead(joypadPin[player][PINMAP_MD_D4]);
    k[19] = 1-digitalRead(joypadPin[player][PINMAP_MD_D5]);
    if(k[11]==k[19] && k[10]==k[18] && k[4]==k[16] && k[5]==k[17] && (k[0]+k[1])<2 && (k[2]+k[3])<2){
      // if Start=Start, A=A, B=B,C=C then these two circle signals is OK.
      // k0=UP,    k1=DOWN,  k2=LEFT, k3=RIGHT,         k4=B,     k5=C(A)
      // k6=UP,    k7=DOWN,  k8=GND,  k9=GND,           k10=A(C), k11=START
      // k12=Z,    k13=Y,    k14=X,   k15=MODE(SELECT), k16=B,    k17=C(A)
      // k18=A(C), k19=START
      key[JOYPAD_UP] = k[0];
      key[JOYPAD_DOWN] = k[1];
      key[JOYPAD_LEFT] = k[2];
      key[JOYPAD_RIGHT] = k[3];
      key[JOYPAD_SELECT] = k[15];
      key[JOYPAD_START] = k[11];
      key[JOYPAD_A] = k[5];
      key[JOYPAD_B] = k[4];
      key[JOYPAD_C] = k[10];
      key[JOYPAD_X] = k[14];
      key[JOYPAD_Y] = k[13];
      key[JOYPAD_Z] = k[12];      
    }
  }
  else if(k[0]==k[6] && k[1]==k[7] && k[8]==1 && k[9]==1 && (k[0]+k[1])<2 && (k[2]+k[3])<2){
    // is a 3 tirggers controller? up=up, down=down, and TOW GND,
    // k0=UP, k1=DOWN, k2=LEFT, k3=RIGHT, k4=B,     k5=C(A)
    // k6=UP, k7=DOWN, k8=GND,  k9=GND,   k10=A(C), k11=START
    key[JOYPAD_UP] = k[0];
    key[JOYPAD_DOWN] = k[1];
    key[JOYPAD_LEFT] = k[2];
    key[JOYPAD_RIGHT] = k[3];
    key[JOYPAD_START] = k[11];
    key[JOYPAD_A] = k[5];
    key[JOYPAD_B] = k[4];
    key[JOYPAD_C] = k[10];
  }
};

//  Sega Saturn   *** TESTED OK ***
void SS_getKey(boolean *key, int player){
  int i;
  boolean k[16];
  int pinout[]={
    PINMAP_SS_D0,PINMAP_SS_D1,PINMAP_SS_D2,PINMAP_SS_D3
  };
  int keymap[]={
    JOYPAD_X,    JOYPAD_Y,    JOYPAD_Z,    JOYPAD_R,
    JOYPAD_B,    JOYPAD_A,    JOYPAD_C,    JOYPAD_START,
    JOYPAD_UP,   JOYPAD_DOWN, JOYPAD_LEFT, JOYPAD_RIGHT,
    JOYPAD_NONE, JOYPAD_NONE, JOYPAD_NONE, JOYPAD_L
  };
  
  PINOUTPUT(joypadPin[player][PINMAP_SS_S0], LOW, 0);
  PINOUTPUT(joypadPin[player][PINMAP_SS_S1], LOW, 4);
  // k1=Z(X), k2=Y,    k3=X(Z),  k4=R(R1)
  for (i=0; i<4; i++)
    k[i] = 1-digitalRead(joypadPin[player][pinout[i]]);
  PINOUTPUT(joypadPin[player][PINMAP_SS_S0], HIGH, 0);
  PINOUTPUT(joypadPin[player][PINMAP_SS_S1], LOW,  4);
  // k4=UP,   k5=DOWN, k6=LEFT,  k7=RIGHT
  for (i=0; i<4; i++)
    k[i+4] = 1-digitalRead(joypadPin[player][pinout[i]]);
  PINOUTPUT(joypadPin[player][PINMAP_SS_S0], LOW,  0);
  PINOUTPUT(joypadPin[player][PINMAP_SS_S1], HIGH, 4);
  // k8=C(A), k9=B,    k10=A(C), k11=START
  for (i=0; i<4; i++)
    k[i+8] = 1-digitalRead(joypadPin[player][pinout[i]]);
  PINOUTPUT(joypadPin[player][PINMAP_SS_S0], HIGH, 0);
  PINOUTPUT(joypadPin[player][PINMAP_SS_S1], HIGH, 4);
  // k12=GND, k13=GND, k14=GND,  k15=L(L1)
  for (i=0; i<4; i++)
    k[i+12] = 1-digitalRead(joypadPin[player][pinout[i]]);
#ifdef DEBUG_ME
//  for(i=0; i<16; i++){
//    Serial.print(k[i]);
//    Serial.print(" ");
//  }
//  Serial.println("");
#endif
  if(k[12]==1 && k[13]==1 && k[14]==0 && k[4]+k[5]<2 && k[6]+k[7]<2){
    for (i=0; i<16; i++){
      key[keymap[i]] = k[i];
    }
  }
};

void setup() {
//  pinMode(pinLed, OUTPUT);
//  pinMode(pinButton, INPUT_PULLUP);
#ifdef DEBUG_ME
  Serial.begin(9660);
#endif
  joypadType[0]=0;
  joypadType[1]=0;
  clearJoypadKey(0);
  clearJoypadKey(1);
  // Sends a clean report to the host. This is important on any Arduino type.
  Gamepad.begin();
}

void loop() {
  checkJoypadType();
  getKey();
  sendJoypadKey();
    // Move x/y Axis to a new position (16bit)
//    Gamepad.xAxis(random(0xFFFF));
//    Gamepad.yAxis(random(0xFFFF));

    // Go through all dPad positions
    // values: 0-8 (0==centered)
//    static uint8_t dpad1 = GAMEPAD_DPAD_CENTERED;
//    Gamepad.dPad1(dpad1++);
//    if (dpad1 > GAMEPAD_DPAD_UP_LEFT)
//      dpad1 = GAMEPAD_DPAD_CENTERED;

//    static int8_t dpad2 = GAMEPAD_DPAD_CENTERED;
//    Gamepad.dPad2(dpad2--);
//    if (dpad2 < GAMEPAD_DPAD_CENTERED)
//      dpad2 = GAMEPAD_DPAD_UP_LEFT;

    // Simple debounce
  delay(30);
}


void reportStatus(char* str, int intVal){
#ifdef DEBUG_ME
  Serial.print(str);
  Serial.print(" (0x");
  Serial.print(intVal, HEX);
  Serial.println(")");
#endif
}

void reportStatusInt(char* str, int strVal, int intVal){
#ifdef DEBUG_ME
  Serial.print(str);
  Serial.print(" : ");
  Serial.print(strVal, HEX);
  Serial.print(" ");
  Serial.print(intVal, HEX);
  Serial.println("");
#endif
}

void reportPINsTitle(){
#ifdef DEBUG_ME
  Serial.println("0 1 2 3 4 5 6 - 7 8 9 A B C D ");
  Serial.println("----------------------------");
#endif
}

void reportPINs(){
#ifdef DEBUG_ME__
  int i;
  
  for (i=0; i<14; i++){
    if(digitalRead(i)==LOW)
      Serial.print("| ");
    else
      Serial.print("# ");
    if(i==6){
      Serial.print("  ");
    }
  }
  Serial.println("");
#endif
}
