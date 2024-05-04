#include "Arduino.h"
#include "Controller.h"

Controller::Controller(){
  // 默认设置各端口为拉高输入
  pin_mode[0] = INPUT_PULLUP;
  pin_mode[1] = INPUT_PULLUP;
  pin_mode[2] = INPUT_PULLUP;
  pin_mode[3] = INPUT_PULLUP;
  pin_mode[4] = INPUT_PULLUP;
  pin_mode[5] = INPUT_PULLUP;
  pin_mode[6] = INPUT_PULLUP;
  return;
}

void Controller::clean(void){
  memset(keys->buttons_value, RELEASE,  sizeof(keys->buttons_value));
  keys->axis_value[0]=AXIS_MID;
  keys->axis_value[1]=AXIS_MID;
  keys->axis_value[2]=AXIS_MID;
  keys->axis_value[3]=AXIS_MID;
  keys->axis_value[4]=AXIS_MID;
  keys->axis_value[5]=AXIS_MID;
}

void Controller::begin(void){
  uint8_t i;
  // 定义端口
  for(i=0; i<7; i++){
    pinMode(pin_number[i], pin_mode[i]);
  }
  // 所有按键设为未按下
  clean();
  // 等待设备就绪
  delay(30);
}

// 至少指定了每个信号线的端口号
void Controller::init(uint8_t *pins, Keys *k){
  keys = k;
  for(int i=0; i<7; i++){
    pin_number[i] = pins[i];
  }
}

// 获得手柄当前按键状态
void Controller::get(void){
  int i;
  // 当为空手柄时，初始化所有按键为未按下
  clean();
  return;
}

// 返回指定数字键的状态
bool Controller::button(int no){
  return keys->buttons_value[no];
}

// 返回指定模拟键的状态
uint16_t Controller::axis(int no){
  return keys->axis_value[no];
}

// 将所有键的状态传回到一个数组。（没有做安全检查）
void Controller::clone_buttons(bool *button, int16_t *axis){
  uint8_t i;
  for (i=0; i<40; i++){
    button[i] = keys->buttons_value[i];
  }
  for (i=0; i<6; i++){
    axis[i] = keys->axis_value[i];
  }
}
