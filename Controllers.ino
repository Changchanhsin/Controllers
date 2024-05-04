#include "configure.h"
#include "Controllers.h"
#include "Wire.h"

#include "src/Utility/UtilityScreen.h"
#include "src/Utility/UtilityIICSwitch.h"
#include "src/Utility/UtilityJoystick.h"

Controllers      controllers[2];
UtilityScreen    screen;
UtilityIICSwitch iicswitch;

// 动态设置
bool isShowKeys = true;

// 设置最大数量
#define CONTROLLERS_MAX 2

// 获取/设置手柄用的全局变量
static const char player[CONTROLLERS_MAX][3]={"1P","2P"};

// 按键状态存储在此
Keys joypadKeys[CONTROLLERS_MAX];

// Pinmap for Leonardo
uint8_t joypadTypePin[CONTROLLERS_MAX][2]={{A3, A2},{A1, A0}};
uint8_t joypadPin[CONTROLLERS_MAX][7] = {{6, 7, 5, 4, A5, 1, 0},{12, 13, 11, 10, A4, 9, 8}};

// 手柄状态检查
bool isChanged[CONTROLLERS_MAX][47];        // 是否变化
uint16_t isReconnected[CONTROLLERS_MAX];

#ifdef SENT_KEYBOARD
uint8_t joypadKeymap[CONTROLLERS_MAX][17] = {
  { KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_RETURN, ' ',  'c', 'x', 'z', 'd', 's', 'a', 'q', 'w', 'e', 'r'  },
  { 'p',          ';',            'l',            '\'',            ']',        '\\', 'k', 'j', 'h', 'i', 'u', 'y', '6', '7', '8', '9'  }
};

void sendKeyboardKey(uint8_t i){
  uint8_t j;
  if(controllers[i].type() != NO_JOYPAD){
    for(j=1; j<=17; j++){
      if(joypadKeys[i].buttons_value[j]==PRESSED){
        Keyboard.press(joypadKeymap[i][j-1]);
      }else{
        Keyboard.release(joypadKeymap[i][j-1]);
      }
    }
    //Keyboard.write();
  }
}
#endif

#ifdef SENT_GAMEPAD

Joystick_ Joypad[CONTROLLERS_MAX]={Joystick_(0x03), Joystick_(0x04)};

inline uint16_t dpad2axis(bool lu, bool rd, uint16_t axis){
  if( lu>rd ){
    return AXIS_MAX;
  }else if( lu<rd ){
    return AXIS_MIN;
  }else if(lu==RELEASE && rd==RELEASE){
    return axis;
  }
  return AXIS_MID;
}

void sendJoypadKey(uint8_t i){
//  if(controllers[i].type() != NO_JOYPAD){
//    Joypad[i].xAxis(dpad2axis(joypadKeys[i].buttons_value[GAMEPAD_LEFT],joypadKeys[i].buttons_value[GAMEPAD_RIGHT]));
//    Joypad[i].yAxis(dpad2axis(joypadKeys[i].buttons_value[GAMEPAD_UP],  joypadKeys[i].buttons_value[GAMEPAD_DOWN]  ));
    //Joypad[i].xAxis(joypadKeys[i].axis_value[GAMEPAD_AXIS_X]);
    //Joypad[i].yAxis(joypadKeys[i].axis_value[GAMEPAD_AXIS_Y]);
    Joypad[i].setXAxis(dpad2axis(joypadKeys[i].buttons_value[GAMEPAD_LEFT],joypadKeys[i].buttons_value[GAMEPAD_RIGHT],joypadKeys[i].axis_value[GAMEPAD_AXIS_X]));
    Joypad[i].setYAxis(dpad2axis(joypadKeys[i].buttons_value[GAMEPAD_UP],  joypadKeys[i].buttons_value[GAMEPAD_DOWN], joypadKeys[i].axis_value[GAMEPAD_AXIS_Y]));
    Joypad[i].setRxAxis(joypadKeys[i].axis_value[GAMEPAD_AXIS_RX]);
    Joypad[i].setRyAxis(joypadKeys[i].axis_value[GAMEPAD_AXIS_RY]);
    Joypad[i].setZAxis(joypadKeys[i].axis_value[GAMEPAD_AXIS_Z]);
    Joypad[i].setRzAxis(joypadKeys[i].axis_value[GAMEPAD_AXIS_RZ]);
    for(uint8_t j=8; j<24; j++){
      if(joypadKeys[i].buttons_value[j]==PRESSED){
        Joypad[i].pressButton(j-8);
      }else{
        Joypad[i].releaseButton(j-8);
      }
    }
}

#endif

void reset(){
#ifdef SENT_KEYBOARD
  Keyboard.releaseAll();
#endif
#ifdef SENT_GAMEPAD
  Joypad[0].begin();
  Joypad[1].begin();
#endif
}

#ifdef DEBUG_ME
void findwire(){
  uint8_t i,e;
  Serial.print("wire on ");
  for(i=1;i<127;i++){
    Wire.beginTransmission(i);
    e=Wire.endTransmission();
    if(e==0){
      Serial.print(i,HEX);
      Serial.print(" ");
    }
  }
}
#endif

void setup() {
#ifdef DEBUG_ME
  Serial.begin(9600);
#endif
  for(uint8_t i=0;i<CONTROLLERS_MAX;i++){
    controllers[i].setup(player[i], joypadPin[i], joypadTypePin[i], &joypadKeys[i]);
    isReconnected[i]=0;
#ifdef SENT_GAMEPAD
    Joypad[i].begin();
#endif
  }
#ifdef SENT_KEYBOARD
  Keyboard.begin();
#endif
  Wire.begin();
  screen.begin();
  delay(100);
}

void loop() {
  uint16_t t;
  for(uint8_t i=0;i<CONTROLLERS_MAX;i++){
    iicswitch.changeChannel(i+1);
    t=controllers[i].loop(isChanged[i]);
    if(t!=isReconnected[i]){
      screen.printTypes(i, t);
      isReconnected[i]=t;
    }
    if(isChanged[i][0]==1){
      if(isShowKeys)
        screen.printKeys(i, &controllers[i]);
#ifdef SENT_GAMEPAD
      sendJoypadKey(i);
#endif
#ifdef SENT_KEYBOARD
      sendKeyboardKey(i);
#endif
    }
  }
}
