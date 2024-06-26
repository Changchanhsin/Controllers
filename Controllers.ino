#define DEBUG_ME_
#define DEBUG_MESSAGE

//  1s  = 1000ms (millsecond )
//  1ms = 1000us (microsecond)

#define PINOUTPUT(pinNumber,outputValue,outputDelay)   digitalWrite((pinNumber), (outputValue)); delayMicroseconds((outputDelay));
#define ANALOG2DIGITAL(val,key1,key2)    if((val)<0x40){(key1)=(key1)|1;}else if((val)>0xB0){(key2)=(key2)|1;}


// Types
#define NO_JOYPAD              0x00
#define SONY_PLAYSTATION       0x70
#define RESERVED_RESERVED      0x05
#define SEGA_MEGADRIVE         0x50
#define SEGA_SATURN            0x55
#define SEGA_DREAMCAST         0x57
#define NINTENDO_SUPERFAMICON  0x07
#define NINTENDO_N64           0x75
#define NINTENDO_GAMECUBE      0x77

int joypadType[2];
int joypadSubType[2];


// Keymap
#define JOYPAD_UP     0
#define JOYPAD_DOWN   1
#define JOYPAD_LEFT   2
#define JOYPAD_RIGHT  3
#define JOYPAD_START  4
#define JOYPAD_SELECT 5
#define JOYPAD_A      6
#define JOYPAD_B      7
#define JOYPAD_C      8
#define JOYPAD_X      9
#define JOYPAD_Y      10
#define JOYPAD_Z      11
#define JOYPAD_L1     12
#define JOYPAD_L2     13
#define JOYPAD_R1     14
#define JOYPAD_R2     15
#define JOYPAD_NONE   16
boolean joypadOld[2][17];
boolean joypadNow[2][17];
const char joypadKeymap[2][17] = {
  {
    KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_RETURN, ' ',  'c', 'x', 'z', 'd', 's', 'a', 'q', 'w', 'e', 'r'  }
  ,
  {
    'p',          ';',            'l',            '\'',            ']',        '\\', 'k', 'j', 'h', 'i', 'u', 'y', '6', '7', '8', '9'  }
};

char *key2str(char c){
  if (c==KEY_UP_ARROW)
    return "up";
  if (c==KEY_DOWN_ARROW)
    return "down";
  if (c==KEY_LEFT_ARROW)
    return "left";
  if (c==KEY_RIGHT_ARROW)
    return "right";
  if (c==KEY_RETURN)
    return "return";
  return &c;
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
  char *s;
  int i,j;

  for(j=0; j<2; j++){
    if(joypadType[j]!=JOYPAD_NONE){
      for(i=0; i<16; i++){
        if(joypadOld[j][i] != joypadNow[j][i]){
          if(joypadNow[j][i]==true){
            Keyboard.press(joypadKeymap[j][i]);
#ifdef DEBUG_MESSAGE
            s=key2str(joypadKeymap[j][i]);
            reportStatusChar("Button pressed", joypadKeymap[j][i], s);
#endif
          }else{
            Keyboard.release(joypadKeymap[j][i]);
#ifdef DEBUG_MESSAGE
            s=key2str(joypadKeymap[j][i]);
            reportStatusChar("Button released", joypadKeymap[j][i], s);
#endif
          }
        }
      }
    }
  }
}

void clearJoypadKey(int player){
  int i;

  for(i=0; i<16; i++){
    joypadOld[player][i] = false;
    joypadNow[player][i] = false;
    Keyboard.release(joypadKeymap[player][i]);
  }
}


// Pinmap
boolean pinmapNC[]     = {  INPUT,  INPUT,  INPUT,  INPUT,  INPUT,  INPUT,  INPUT };
#define PINMAP_PS_DATA  0  // INPUT   brown
#define PINMAP_PS_ACK   1  // INPUT   green
#define PINMAP_PS_ATT   2  // OUTPUT  yellow    SELECT LOW enabled
#define PINMAP_PS_CMD   5  // OUTPUT  orange
#define PINMAP_PS_CLOCK 6  // OUTPUT  blue
boolean pinmapPS[]     = {  INPUT,  INPUT,  OUTPUT, INPUT,  INPUT, OUTPUT, OUTPUT };
#define PINMAP_MD_D0 0   // INPUT
#define PINMAP_MD_D1 1   // INPUT
#define PINMAP_MD_D2 2   // INPUT
#define PINMAP_MD_D3 3   // INPUT
#define PINMAP_MD_D4 4   // INPUT
#define PINMAP_MD_D5 5   // INPUT
#define PINMAP_MD_SEL 6  // OUTPUT
boolean pinmapMD[]     = {  INPUT,  INPUT,  INPUT,  INPUT,  INPUT,  INPUT, OUTPUT };
#define PINMAP_SS_D0 0  // INPUT
#define PINMAP_SS_D1 1  // INPUT
#define PINMAP_SS_D2 2  // INPUT
#define PINMAP_SS_D3 3  // INPUT
#define PINMAP_SS_S0 5  // OUTPUT
#define PINMAP_SS_S1 6  // OUTPUT
boolean pinmapSS[]     = {  INPUT,  INPUT,  INPUT,  INPUT,  INPUT, OUTPUT, OUTPUT };
#define PINMAP_DC_D1    0  // BIDIR
#define PINMAP_DC_D5    5  // BIDIR
#define PINMAP_DC_SENSE 2  // UNKNOWN
boolean pinmapDC[]     = {  INPUT,  INPUT,  INPUT,  INPUT,  INPUT, OUTPUT, OUTPUT };
#define PINMAP_SFC_DATA  0  // INPUT
#define PINMAP_SFC_CLOCK 5  // OUTPUT
#define PINMAP_SFC_LATCH 2  // OUTPUT
boolean pinmapSFC[]    = {  INPUT,  INPUT, OUTPUT,  INPUT,  INPUT, OUTPUT,  INPUT };
#define PINMAP_N64_DATA 2  // BIDIR
boolean pinmapN64[]    = {  INPUT,  INPUT,  INPUT,  INPUT,  INPUT,  INPUT,  INPUT };
#define PINMAP_GC_DATA  2  // BIDIR
boolean pinmapGC[]     = {  INPUT,  INPUT,  INPUT,  INPUT,  INPUT,  INPUT,  INPUT };


// check and set joypad pinmap, for all type of controller.
int analogToOct(int analogValue)
{
  //      --------- 骞冲潎 ---------   ------ 宸ヤ笟 ------
  //  0 = 0.0 | 0.0..0.3 |   0- 71   GND  |       0-150
  //  1 = 0.7 | 0.4..1.0 |  72-218   1V   | 151-204-250
  //  2 = 1.4 | 1.1..1.7 | 219-364   1.5V | 251-307-350
  //  3 = 2.1 | 1.8..2.4 | 365-511   2.4V | 451-492-550
  //  4 = 2.8 | 2.5..3.1 | 512-657   3V   | 561-614-630
  //  5 = 3.5 | 3.2..3.8 | 658-803   3.3V | 631-675-720
  //  6 = 4.2 | 3.9..4.5 | 804-949   4.2V | 811-860-910
  //  7 = 5.0 | 4.7..5.0 | 950-1024  5V   | 910-1023
  if(analogValue < 151) return 0;
  if(analogValue < 251) return 1;
  if(analogValue < 451) return 2;
  if(analogValue < 560) return 3;
  if(analogValue < 631) return 4;
  if(analogValue < 811) return 5;
  if(analogValue < 910) return 6;
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
      // recheck again to de~~~~
      joypadType[i] = checkJoypad(i);
      clearJoypadKey(i);
#ifdef DEBUG_MESSAGE
      if(i==0){
        reportStatusInt("Analog Values = ", analogRead(A1), analogRead(A0));
        reportStatusInt("Type = ", joypadType[i], i);
      }else{
        reportStatusInt("Analog Values = ", analogRead(A3), analogRead(A2));
        reportStatusInt("Type = ", joypadType[i], i);
      }
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
        reportStatus("Controller changed : Nintendo Superfamicon", i);
        break;
      case NINTENDO_N64:
        setPins(pinmapN64,i);
        reportStatus("Controller changed : Nintendo N64", i);
        break;
      case SONY_PLAYSTATION:
        setPins(pinmapPS,i);
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

void setPins(boolean *pinmap, int player){
  int i;

  player=player*7;
  for(i=0; i<7; i++){
    pinMode(i+player,pinmap[i]);
  }
}


// Get controller keys, for loop
void getKey()
{
  int i;

  saveJoypadKey();
  for(i=0; i<2; i++){
    switch(joypadType[i]){
    case NINTENDO_SUPERFAMICON:
      SFC_getKey(joypadNow[i], i);
      break;
    case NINTENDO_N64:
      N64_getKey(joypadNow[i], i);
      break;
    case NINTENDO_GAMECUBE:
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

// Nintendo N64
#define N64_PIN_MASK   0x04   // PIN 2
#define N64_PIN_MASK2  0x02   // PIN 9
#define N64_PIN_DIR    DDRD   // 0..7
#define N64_PIN_DIR2   DDRB   // 8..13
#define N64_HIGH       DDRD &= ~N64_PIN_MASK
#define N64_HIGH2      DDRB &= ~N64_PIN_MASK2
#define N64_LOW        DDRD |= N64_PIN_MASK
#define N64_LOW2       DDRB |= N64_PIN_MASK2
#define N64_QUERY     (PIND & N64_PIN_MASK)
#define N64_QUERY2    (PINB & N64_PIN_MASK2)

char N64_raw_dump[33]; // 1 received bit per byte

void N64_getKey(boolean *key, int player){
  unsigned char initialize = 0x00;
  noInterrupts();
  if(player==0){
    N64_send(&initialize, 1);
    int x,y;
    y=0;
    for (x=0; x<64; x++) {
      // make sure the line is idle for 64 iterations, should
      // be plenty.
      y++;
      if (!N64_QUERY)
        x = 0;
      if(y>10000)
        return;
    }
    unsigned char command[] = {
      0x01    };
    N64_send(command, 1);
    N64_get();
    interrupts();
    translate_raw_data(key); 
  }
  else{
    N64_send2(&initialize, 1);
    int x,y;
    y=0;
    for (x=0; x<64; x++) {
      // make sure the line is idle for 64 iterations, should
      // be plenty.
      y++;
      if (!N64_QUERY2)
        x = 0;
      if(y>10000)
        return;
    }
    unsigned char command[] = {
      0x01    };
    N64_send2(command, 1);
    N64_get2();
    interrupts();
    translate_raw_data(key); 
  }
}

void translate_raw_data(boolean *key)
{
  int i;
  int keymap[6][8]={
    {
      JOYPAD_A,    JOYPAD_B,    JOYPAD_Z,  JOYPAD_START, JOYPAD_UP, JOYPAD_DOWN, JOYPAD_LEFT, JOYPAD_RIGHT    }
    ,
    {
      JOYPAD_NONE, JOYPAD_NONE, JOYPAD_L1, JOYPAD_R1,    JOYPAD_UP, JOYPAD_DOWN, JOYPAD_LEFT, JOYPAD_RIGHT    }
  };
  // bits: A, B, Z, Start, Dup, Ddown, Dleft, Dright
  for (i=0; i<8; i++) {
    key[keymap[0][i]] |= N64_raw_dump[i] ? (0x80 >> i) : 0;
  }
  // bits: 0, 0, L, R, Cup, Cdown, Cleft, Cright
  for (i=0; i<8; i++) {
    key[keymap[1][i]] |= N64_raw_dump[8+i] ? (0x80 >> i) : 0;
  }
  // bits: joystick x value
  // These are 8 bit values centered at 0x80 (128)
  //for (i=0; i<8; i++) {
  //  N64_status.stick_x |= N64_raw_dump[16+i] ? (0x80 >> i) : 0;
  //}
  //for (i=0; i<8; i++) {
  //  N64_status.stick_y |= N64_raw_dump[24+i] ? (0x80 >> i) : 0;
  //}
}

void N64_send(unsigned char *buffer, char length)
{
  char bits;
  bool bit;
outer_loop:
  {
    bits=8;
inner_loop:
    {
      N64_LOW;
      if (*buffer >> 7) {
        asm volatile ("nop\nnop\nnop\nnop\nnop\n");
        N64_HIGH;
        asm volatile (
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          );
      }
      else{
        asm volatile (
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\n");
        N64_HIGH;
      }
      --bits;
      if (bits != 0) {
        asm volatile (
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\n");
        *buffer <<= 1;
        goto inner_loop;
      } // fall out of inner loop
    }
    --length;
    if (length != 0) {
      ++buffer;
      goto outer_loop;
    } // fall out of outer loop
  }
  asm volatile ("nop\nnop\nnop\nnop\n");
  N64_LOW;
  asm volatile ("nop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\n"  
    "nop\nnop\nnop\nnop\n");
  N64_HIGH;
}

void N64_send2(unsigned char *buffer, char length)
{
  char bits;
  bool bit;

outer_loop2:
  {
    bits=8;
inner_loop2:
    {
      N64_LOW2;
      if (*buffer >> 7) {
        asm volatile ("nop\nnop\nnop\nnop\nnop\n");
        N64_HIGH2;
        asm volatile ("nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          );
      }
      else{
        asm volatile ("nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\nnop\n"  
          "nop\n");
        N64_HIGH2;
      }
      --bits;
      if (bits != 0) {
        asm volatile ("nop\nnop\nnop\nnop\nnop\n"  
          "nop\nnop\nnop\nnop\n");
        *buffer <<= 1;
        goto inner_loop2;
      } // fall out of inner loop
    }
    --length;
    if (length != 0) {
      ++buffer;
      goto outer_loop2;
    } // fall out of outer loop
  }
  asm volatile ("nop\nnop\nnop\nnop\n");
  N64_LOW2;
  asm volatile ("nop\nnop\nnop\nnop\nnop\n"
    "nop\nnop\nnop\nnop\nnop\n"  
    "nop\nnop\nnop\nnop\n");
  N64_HIGH2;
}

void N64_get()
{
  unsigned char timeout;
  char bitcount = 32;
  char *bitbin = N64_raw_dump;
  reportStatus("get data start.",!N64_QUERY);

read_loop:
  timeout = 0x3f;
  while (N64_QUERY) {
    if (!--timeout){
      reportStatus("get data time out.",!N64_QUERY);
      return;
    }
  }
  asm volatile (
    "nop\nnop\nnop\nnop\nnop\n"  
    "nop\nnop\nnop\nnop\nnop\n"  
    "nop\nnop\nnop\nnop\nnop\n"   
    "nop\nnop\nnop\nnop\nnop\n"  
    "nop\nnop\nnop\nnop\nnop\n" 
    "nop\nnop\nnop\nnop\nnop\n" 
    );
  *bitbin = N64_QUERY;
  ++bitbin;
  --bitcount;
  if (bitcount == 0){
    reportStatus("get no bit.",!N64_QUERY);
    return;
  }

  timeout = 0x3f;
  while (!N64_QUERY) {
    if (!--timeout){
      reportStatus("get data time out 2.",N64_raw_dump[0]);
      return;
    }
  }
  goto read_loop;
}

void N64_get2()
{
  unsigned char timeout;
  char bitcount = 32;
  char *bitbin = N64_raw_dump;

read_loop2:
  timeout = 0x3f;
  while (N64_QUERY2) {
    if (!--timeout)
      return;
  }
  asm volatile ("nop\nnop\nnop\nnop\nnop\n"  
    "nop\nnop\nnop\nnop\nnop\n"  
    "nop\nnop\nnop\nnop\nnop\n"  
    "nop\nnop\nnop\nnop\nnop\n"  
    "nop\nnop\nnop\nnop\nnop\n"  
    "nop\nnop\nnop\nnop\nnop\n"  
    );
  *bitbin = N64_QUERY2;
  ++bitbin;
  --bitcount;
  if (bitcount == 0)
    return;

  timeout = 0x3f;
  while (!N64_QUERY2) {
    if (!--timeout)
      return;
  }
  goto read_loop2;
}


// Sony Playstation   *** TESTED OK ***
void PS_getKey(boolean *key, int player){
  int i,j;
  byte ret;
  byte thisplayerpin;

  // A=Cir, B=X, X=Tri, Y=Rect, C = analog RJ, Z = analog LJ
  // [0,1]=0x41 [2,3]=0x73; [3,4]=0x53
  int keymap[4][8]={
    {JOYPAD_SELECT, JOYPAD_C,    JOYPAD_Z,    JOYPAD_START, JOYPAD_UP, JOYPAD_RIGHT, JOYPAD_DOWN, JOYPAD_LEFT    },
    {JOYPAD_L2,     JOYPAD_R2,   JOYPAD_L1,   JOYPAD_R1,    JOYPAD_X,  JOYPAD_A,     JOYPAD_B,    JOYPAD_Y       },
    {JOYPAD_NONE,   JOYPAD_NONE, JOYPAD_NONE, JOYPAD_START, JOYPAD_UP, JOYPAD_RIGHT, JOYPAD_DOWN, JOYPAD_LEFT    },
    {JOYPAD_L2,     JOYPAD_L1,   JOYPAD_Y,    JOYPAD_X,     JOYPAD_R1, JOYPAD_A,     JOYPAD_B,    JOYPAD_R2      }
  };

  thisplayerpin = player*7;
  digitalWrite(PINMAP_PS_CMD+thisplayerpin, HIGH);
  digitalWrite(PINMAP_PS_CLOCK+thisplayerpin, HIGH);
  digitalWrite(PINMAP_PS_ATT+thisplayerpin, LOW);

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
      for(i=0; i<2; i++){
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
  digitalWrite(PINMAP_PS_ATT+thisplayerpin, HIGH);
}

byte PS_exchangeData(int player, byte cmd){
  byte i,ret,dt;
  int pinCmd, pinClock, pinData;

  ret = 0;
  pinCmd = PINMAP_PS_CMD+player*7;
  pinData = PINMAP_PS_DATA+player*7;
  pinClock = PINMAP_PS_CLOCK+player*7;
  for(i=0; i<8; i++){
    if( cmd & (1<<i) )
       digitalWrite(pinCmd, HIGH);
    else
       digitalWrite(pinCmd, LOW);
    digitalWrite(pinClock, LOW);
    delayMicroseconds(2);
    dt = digitalRead(pinData);
    if(dt)
      ret = ret | (B00000001 << i);
    digitalWrite(pinClock, HIGH);
    delayMicroseconds(2);
  }
  delayMicroseconds(10);  // waiting and pass ACK
  return ret;
}


// Nintendo SFC   *** TESTED ***
void SFC_getKey(boolean *key, int player){
  player = player*7;
  int i;
  int joypadmap[]={
    JOYPAD_B, JOYPAD_Y, JOYPAD_SELECT, JOYPAD_START, JOYPAD_UP, JOYPAD_DOWN, JOYPAD_LEFT, JOYPAD_RIGHT, JOYPAD_A, JOYPAD_X, JOYPAD_L1, JOYPAD_R1  };

  PINOUTPUT(PINMAP_SFC_LATCH+player, HIGH, 12);
  PINOUTPUT(PINMAP_SFC_LATCH+player, LOW, 6000);
  PINOUTPUT(PINMAP_SFC_CLOCK+player, HIGH, 6);
  for(i=0; i<12; i++){
    PINOUTPUT(PINMAP_SFC_CLOCK+player, LOW, 6);
    key[joypadmap[i]] = 1-digitalRead(PINMAP_SFC_DATA);
    PINOUTPUT(PINMAP_SFC_CLOCK+player, HIGH, 6);
  }
};


// Sega MD   *** TESTED OK ***
//  BE CAREFULL, check logic first, or will misunderstand.
//  Parallel is more dangerous.

void MD_getKey(boolean *key, int player){
  player = player*7;
  int i;
  boolean k[20];
  int pinout[]={
    PINMAP_MD_D0,PINMAP_MD_D1,PINMAP_MD_D2,PINMAP_MD_D3,PINMAP_MD_D4,PINMAP_MD_D5  };

  // Try HIGH/LOW cycles
  // HIGH first
  PINOUTPUT(PINMAP_MD_SEL+player, HIGH, 4);
  // k0=up, k1=down, k2=left, k3=right, k4=B, k5=C(A)
  for(i=0; i<6; i++){
    k[i] = 1-digitalRead(pinout[i]+player);
  }
  PINOUTPUT(PINMAP_MD_SEL+player, LOW, 4); 
  // k6=up, k7=down, k8=0, k9=0, k10=A(C), k11=start
  for(i=0; i<6; i++){
    k[i+6] = 1-digitalRead(pinout[i]+player);
  }
  if(k[6]==1 && k[7]==1 && k[8]==1 && k[9]==1) // is a 6 tirggers controller? need a next circle?
  {
    // Try HIGH/LOW again for external tirggers, this is the forth H/L circle.
    PINOUTPUT(PINMAP_MD_SEL+player, HIGH, 4);
    // k12=Z, k13=Y, k14=X, k15=MODE(SELECT), k16=B, k17=C(A)
    for(i=0; i<6; i++){
      k[i+12] = 1-digitalRead(pinout[i]+player);
    }
    PINOUTPUT(PINMAP_MD_SEL+player, LOW, 4);
    // for k18=A(C), k19=START, other 4 are ignore.
    k[18] = 1-digitalRead(PINMAP_MD_D4+player);
    k[19] = 1-digitalRead(PINMAP_MD_D5+player);
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
  player = player*7;
  int i;
  boolean k[16];
  int pinout[]={
    PINMAP_SS_D0,PINMAP_SS_D1,PINMAP_SS_D2,PINMAP_SS_D3
  };
  int keymap[]={
    JOYPAD_X,    JOYPAD_Y,    JOYPAD_Z,    JOYPAD_R1,
    JOYPAD_UP,   JOYPAD_DOWN, JOYPAD_LEFT, JOYPAD_RIGHT,
    JOYPAD_A,    JOYPAD_B,    JOYPAD_C,    JOYPAD_START,
    JOYPAD_NONE, JOYPAD_NONE, JOYPAD_NONE, JOYPAD_L1
  };

  PINOUTPUT(PINMAP_SS_S0+player, LOW, 0);
  PINOUTPUT(PINMAP_SS_S1+player, LOW, 4);
  // k1=Z(X), k2=Y,    k3=X(Z),  k4=R(R1)
  for (i=0; i<4; i++)
    k[i] = 1-digitalRead(pinout[i]+player);
  PINOUTPUT(PINMAP_SS_S0+player, HIGH, 0);
  PINOUTPUT(PINMAP_SS_S1+player, LOW,  4);
  // k4=UP,   k5=DOWN, k6=LEFT,  k7=RIGHT
  for (i=0; i<4; i++)
    k[i+4] = 1-digitalRead(pinout[i]+player);
  PINOUTPUT(PINMAP_SS_S0+player, LOW,  0);
  PINOUTPUT(PINMAP_SS_S1+player, HIGH, 4);
  // k8=C(A), k9=B,    k10=A(C), k11=START
  for (i=0; i<4; i++)
    k[i+8] = 1-digitalRead(pinout[i]+player);
  PINOUTPUT(PINMAP_SS_S0+player, HIGH, 0);
  PINOUTPUT(PINMAP_SS_S1+player, HIGH, 4);
  // k12=GND, k13=GND, k14=GND,  k15=L(L1)
  for (i=0; i<4; i++)
    k[i+12] = 1-digitalRead(pinout[i]+player);
#ifdef DEBUG_ME
  for(i=0; i<16; i++){
    Serial.print(k[i]);
    Serial.print(" ");
  }
  Serial.println("");
#endif
  if(k[12]==1 && k[13]==1 && k[14]==0 && k[4]+k[5]<2 && k[6]+k[7]<2){
    for (i=0; i<16; i++){
      key[keymap[i]] = k[i];
    }
  }
};


// ---- Main functions start here ----

void setup(){
  Keyboard.begin();
//  Mouse.begin();
  joypadType[0]=0;
  joypadType[1]=0;
  clearJoypadKey(0);
  clearJoypadKey(1);
}

void loop(){
  checkJoypadType();
  getKey();
  sendJoypadKey();
  delay(10);  
}

void reportStatus(char* str, int intVal){
  Serial.print(str);
  Serial.print(" (0x");
  Serial.print(intVal, HEX);
  Serial.println(")");
}

void reportStatusChar(char *str, int intVal, char *cVal){
  Serial.print(str);
  Serial.print(" '");
  Serial.print(cVal);
  Serial.print("'(0x");
  Serial.print(intVal, HEX);
  Serial.println(")");
}

void reportStatusInt(char* str, int strVal, int intVal){
  Serial.print(str);
  Serial.print(" : ");
  Serial.print(strVal, HEX);
  Serial.print(" ");
  Serial.print(intVal, HEX);
  Serial.println("");
}

#ifdef DEBUG_ME
void reportPINsTitle(){
  Serial.println("0 1 2 3 4 5 6 7 8 9 A B C D ");
  Serial.println("----------------------------");
}

void reportPINs(){
  int i;
  
  for (i=0; i<14; i++){
    if(digitalRead(i)==0)
      Serial.print("| ");
    else
      Serial.print("# ");
  }
  Serial.println("");
}
#endif


