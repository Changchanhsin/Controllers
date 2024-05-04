#ifndef Controller_h
#define Controller_h

#include "Arduino.h"

#define PRESSED  0
#define RELEASE  1

//      按键名         按键号
#define GAMEPAD_UP      0
#define GAMEPAD_DOWN    1
#define GAMEPAD_LEFT    2
#define GAMEPAD_RIGHT   3
#define GAMEPAD_UP2     4
#define GAMEPAD_DOWN2   5
#define GAMEPAD_LEFT2   6
#define GAMEPAD_RIGHT2  7
#define GAMEPAD_A       8  //  1
#define GAMEPAD_B       9  //  2
#define GAMEPAD_X      10  //  3
#define GAMEPAD_Y      11  //  4
#define GAMEPAD_L1     12  //  5
#define GAMEPAD_R1     13  //  6
#define GAMEPAD_Z      12
#define GAMEPAD_C      13
#define GAMEPAD_SELECT 14  //  7
#define GAMEPAD_START  15  //  8
#define GAMEPAD_L      16  //  9
#define GAMEPAD_R      17  // 10
#define GAMEPAD_L2     16
#define GAMEPAD_R2     17
#define GAMEPAD_LJ     18  // 11
#define GAMEPAD_RJ     19  // 12
#define GAMEPAD_HOME   20  // 13
#define GAMEPAD_21     21  // 14
#define GAMEPAD_22     22  // 15
#define GAMEPAD_23     23  // 16
#define GAMEPAD_NONE   24

#define GAMEPAD_AXIS_X   0
#define GAMEPAD_AXIS_Y   1
#define GAMEPAD_AXIS_Z   4
#define GAMEPAD_AXIS_RX  2
#define GAMEPAD_AXIS_RY  3
#define GAMEPAD_AXIS_RZ  5

#define AXIS_MAX  65535
#define AXIS_MID  32768
#define AXIS_MIN      0

#define DIR_UP    1
#define DIR_DOWN  2
#define DIR_LEFT  4
#define DIR_RIGHT 8
//                         0000 000U 00D0 00DU 0L00 0L0U 0LD0 0LDU R000 R00U R0D0 R0DU RL00 RL0U RLD0 RLDU
const static uint8_t dir2dpad[16]={0,   1,   5,   0,   7,   8,   6,   7,   3,   2,   4,   3,   0,   1,   5,   0    };

  inline uint8_t dpad2dPad(uint8_t u,uint8_t d,uint8_t l,uint8_t r){
    uint8_t m[]={0,1,5,0, 7,8,6,0, 3,2,4,0, 0,0,0,0};
    return m[r<<3+l<<2+d<<1+u];
  }

struct Keys {
  bool buttons_value[40];  // b[0..3]=dpad1, b[4..7]=dpad2
                           // b[8..39]=32buttons
  uint16_t axis_value[6];  // 6个16位模拟量，x、y、rx、ry，值0=中间，+为左/上，-为右/下，取值范围0..32768(8000)..6553567(7FFF)
  inline uint8_t dPad1(){return dpad2dPad(buttons_value[GAMEPAD_UP],buttons_value[GAMEPAD_DOWN],buttons_value[GAMEPAD_LEFT],buttons_value[GAMEPAD_RIGHT]);};
  inline uint8_t dPad2(){return dpad2dPad(buttons_value[GAMEPAD_UP2],buttons_value[GAMEPAD_DOWN2],buttons_value[GAMEPAD_LEFT2],buttons_value[GAMEPAD_RIGHT2]);};
};

class Controller {
protected:
  Keys *keys;
  uint8_t pin_mode[7];       // 端口模式
  uint8_t pin_number[7];     // 端口号

public:
  uint8_t subtype;

  Controller();
  void init(uint8_t *pins, Keys *k); // 初始化
  void clean(void);         // 清空所有按键和摇杆值
  void begin(void);         // 开始使用，初始化硬件和数据，初始化信号电平
  virtual void get(void);   // 获得手柄当前按键状态
  inline bool button(int no);      // 返回指定键的状态
  inline uint16_t axis(int no);     // 返回指定摇杆方向的状态
  void clone_buttons(bool *button, int16_t *axis); // 将所有键的状态传回到一个数组。
};

#endif
