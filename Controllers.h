#ifndef Controllers_h
#define Controllers_h

#include "configure.h"

// ATARI
#ifdef USES_ATARI
#include "src/Controller/ControllerAtari.h"
#endif
// NINTENDO
#ifdef USES_FC
#include "src/Controller/ControllerFC.h"
#endif
#ifdef USES_SFC
#include "src/Controller/ControllerSFC.h"
#endif
#ifdef USES_N64
#include "src/Controller/ControllerN64.h"
#endif
#ifdef USES_NGC
#include "src/Controller/ControllerNGC.h"
#endif
#ifdef USES_WII
#include "src/Controller/ControllerWII.h"
#endif
// SEGA
#ifdef USES_MD
#include "src/Controller/ControllerMD.h"
#endif
#ifdef USES_SS
#include "src/Controller/ControllerSS.h"
#endif
#ifdef USES_DC
#include "src/Controller/ControllerDC.h"
#endif
// SONY
#ifdef USES_PS
#include "src/Controller/ControllerPS.h"
#endif

#define NO_JOYPAD              000
#define ATARI_A2600            005
#define ATARI_A5200            
#define ATARI_A7800            
#define NINTENDO_FC            006  // NES
#define NINTENDO_SFC           007  // SNES
#define NINTENDO_N64           057
#define NINTENDO_NGC           056
#define NINTENDO_WII           066
#define SEGA_MEGADRIVE         050
#define SEGA_SATURN            055
#define SEGA_DREAMCAST         077
#define SONY_PLAYSTATION       070  // PS1/PS2
#define PANASONIC_3DO          
#define SNK_NEOGEO             
#define NEC_PCE                
#define MICROSOFT_XBOX360      
#define NINTENDO_WIIBT           
#define SONY_PLAYSTATION3      

class Controllers {
private:
  char *id;
  Keys *keys;
  Keys keysNow;
  uint8_t joypadType;
  uint8_t subtype;
  uint8_t joypadTypePin[2];
  uint8_t joypadPin[7];

  Controller       nc;
#ifdef USES_ATARI
  Atari2600        atari;
#endif
#ifdef USES_FC
  NintendoFC       fc;
#endif
#ifdef USES_SFC
  NintendoSFC      sfc;
#endif
#ifdef USES_MD
  SegaMegadrive    md;
#endif
#ifdef USES_SS
  SegaSaturn       ss;
#endif
#ifdef USES_N64
  Nintendo64       n64;
#endif
#ifdef USES_NGC
  NintendoGamecube ngc;
#endif
#ifdef USES_WII
  NintendoWII      wii;
#endif
#ifdef USES_PS
  SonyPlaystation  ps;
#endif

  uint8_t analogToOct(int analogValue);
  uint8_t checkJoypad(void);
  void saveJoypadKey(void);
  void clearJoypadKey(void);

//  void reportStatus(char* str);
//  void reportStatus(char* str, int intVal);
//  void reportStatus(char* str, int strVal, int intVal);
//  void reportPINsTitle(void);
//  void reportPINs(void);
  uint8_t checkJoypadType(void);
  void getKey(void);
  inline bool ifChanged(uint16_t oldData, uint16_t newData){
    bool ret = ((newData==oldData)?0:1); oldData=newData; return ret;
  };
  inline bool ifChanged(bool oldData, bool newData){
    bool ret = ((newData==oldData)?0:1); oldData=newData; return ret;
  };

public:
  Controllers(void);
  void setup(char *cid, uint8_t *pins, uint8_t *apins, Keys *k);
  uint16_t loop(bool *changed);
  inline uint8_t type(void){return joypadType;};
  inline Keys *rawkeys(void){return keys;};
};

#endif
