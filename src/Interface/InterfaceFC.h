#ifndef InterfaceFC_h
#define InterfaceFC_h

class InterfaceFC {
private:
  uint8_t FC_DATA;
  uint8_t FC_SENSOR;
  uint8_t FC_LATCH;
  uint8_t FC_CLOCK;
  uint8_t FC_TRIGGER;
  uint8_t FC_MIC;

public:
  bool raw_dump[16];  // 获取的16个布尔数据
//  bool mic_dump[16];
//  bool sensor_dump[16];
//  bool trigger_dump[16];

  void init(uint8_t pinData, uint8_t pinSensor, uint8_t pinLatch, uint8_t pinClock, uint8_t pinTrigger, uint8_t pinMic);  // 设置五个端口
  void get(void);   // 获取按键数据
};

#endif
