# Controllers

A arduino based Joypad hardware adaptor.

基于Arduino Leonardo的游戏手柄适配器


# Functional

将游戏机手柄通过该适配器用到PC上。

已测试SFC、MD（3键、6键）、SS。


# Design

（图见PPT文件）

## Port/Pinmap/Interface

手柄通过专用接口转到15pin D-SUB公口，Arduino通过15pin接口与手柄连接，Arduino转换手柄信息为键盘按键

15个pin分为4部分：

- 数据区：1、2、6、7、8、11、12
- 电源区：4（3.3V）、9（5V）、14（GND）
- 识别区：5、10
- 保留针：3、13、15

数据区用于发送和接收信号。

电源区负责为手柄和识别区提供电源

识别区通过电压高低自动识别手柄类型，用于初始化数据区端口定义

保留针不使用。

|D-SUB|9|14|4|1|2|6|7|8|11|12|5|10|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|Atari|-|GND|-|UP|DOWN|LEFT|RIGHT|TRIGGER|-|-|3.3V|GND|
|FC/SFC|5V|GND|-|DATA|D3|LATCH|-|SFC/FC|CLK|D4|GND|5V|
|MD|5V|GND|-|D0|D1|D2|D3|TL|TR|TH|GND|3.3V|
|SS|5V|GND|-|D0|D1|D2|D3|-|S0|S1|3.3V|3.3V|
|PS/PS2|9V|GND|3.3V|DATA|ACK|ATT|-|-|CMD|CLK|GND|5V|
|N64|5V|GND|3.3V|-|-|DATA|-|-|-|GND|5V|3.3V|
|GC|5V|GND|3.3V|-|-|DATA|-|-|-|5V|5V|3.3V|
|DC|5V|GND|-|D1|D5|Sense|-|-|-|GND|3.3V|5V|

## Joystick/Keymap

当采用键盘模式时，一个Leonardo可以同时连接两个手柄；当采用游戏手柄模式时，一个Leonardo仅可以连接第一个手柄

|Gamepad|Keyboard1|Keyboard2|Atari|FC|SFC|MD|PS/PS2|SS|DC|N64|NGC|
|---|---|---|---|---|---|---|---|---|---|---|---|
|X|Left/Right|L/'|Joystick|D-Pad|D-Pad|D-Pad|X|D-Pad|X|X|X|
|Y|Up/Down|P/.|Joystick|D-Pad|D-Pad|D-Pad|Y|D-Pad|Y|Y|Y|
|B1|x|h|Trigger|A|A|B|Cross|B|A|Cd|B|
|B2|c|j|Game Select|B|B|A|Circle|A|B|Cr|A|
|B3|s|u|-|-|X|Y|Rect|X|Y|Cl|X|
|B4|d|i|-|-|Y|X|Tri|Y|X|Cu|Y|
|B5|q|6|-|-|L|Z|L1|L|L|L|L|
|B6|e|8|-|-|R|C|R1|R|R|R|R|
|B7|space||Player Select|Select|Select|Mode|Select|Mode|Start|J|Z|
|B8|enter||Reset|Start|Start|Start|Start|Start|Start|Start|Start|
|B9|w
|B10|r
|B11|
|B12|
|B13|
|Z|
|Cap|
|Xr|
|Yr|
|Zr|


# Reference

## Atari 2600

### Principle

Aatari 2600有一个摇杆和一个按钮，使用9针母口，无芯片，仅5个开关。

有手柄将上下同时按定义为“Player Select”，左右同时按定义为"Reset"，上下左右同时按定义为“Game Select”。

### Pseudocode

    u = 1 - digitalRead(A2600_PIN_U);
    d = 1 - digitalRead(A2600_PIN_D);
    l = 1 - digitalRead(A2600_PIN_L);
    r = 1 - digitalRead(A2600_PIN_R);
    if( u == d == l == r == 1){
      Gamepad.Buttons[B2] = 1;
    }else{
      if( u == d == 1 ){
        Gamepad.Buttons[SELECT] = 1;
      }else{
        Gamepad.Buttons[UP    ] = u;
        Gamepad.Buttons[DOWN  ] = d;
      }
      if( l == r == 1 ){
        Gamepad.Buttons[START ] = 1;
      }else{
        Gamepad.Buttons[LEFT  ] = l;
        Gamepad.Buttons[RIGHT ] = r;
      }
    }
    Gamepad.Buttons[B1   ] = 1 - digitalRead(A2600_PIN_T)

### Implementation

公共端接地，五个信号端接5个数据区针，数据区针初始化为INPUT_PULLUP，无操作时为高电平，按下时为低电平。

## FC/NES

### Principle

手柄采用5V供电，两个输入端LATCH和CLK、一个输出端DATA。

- LATCH常低，通过一个12微秒的高电平激活手柄输入。
- LATCH高电平后等待6000微秒，连续发送CLK高电平脉冲以获得数据，每个脉冲宽度为12微秒高12微秒低，共16组。
- DATA信号常高，CLK信号开始后下降沿读数据，数据顺序为按键A、B、Select、Start、Up、Down、Left、Right。

光枪另有两个短空用于检测扳机和光传感器

### Timing

                  |-------------------------------- 16670 --------------------------------------|
                  |12| 6000 |----------------------- 192 -------------------------------| 10466 |
    LATCH     ____/``\__________________________________________________________________________/``

    CLK       ______________/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\___________

    DATA(FC)  `````AAAAAAAAA`BBB`SEL`STA`UP``DN``LF``RT````````````````````````````````````````````

### Pseudocode

    fc_btn_sequence = { FC_BTN_A,  FC_BTN_B,    FC_BTN_SELECT, FC_BTN_START,
                        FC_BTN_UP, FC_BTN_DOWN, FC_BTN_LEFT,   FC_BTN_RIGHT };

    digitalWrite(FC_PIN_LATCH, HIGH);
    delay(12);
    digitalWrite(FC_PIN_LATCH, LOW);
    delay(6000);

    for( i=0; i<8; i++ ){
      digitalWrite(FC_PIN_CLK, HIGH);
      delay(12);
      Gamepad.Button[ fc_btn_sequence[i] ] = digitalRead(FC_PIN_DATA);
      digitalWrite(FC_PIN_CLK, LOW);
      delay(12);
    }
    
    delay(96);

    delay(10466);

### Implementation




## SFC/SNES

### Principle

手柄采用5V供电，两个输入端LATCH和CLK、一个输出端DATA。

- LATCH常低，通过一个12微秒的高电平激活手柄输入。
- LATCH高电平后等待6000微秒，连续发送CLK高电平脉冲以获得数据，每个脉冲宽度为12微秒高12微秒低，共16组。
- DATA信号常高，CLK信号开始后下降沿读数据，数据顺序为按键B、Y、Select、Start、Up、Down、Left、Right、A、X、L、R。

### Timing

                  |-------------------------------- 16670 --------------------------------------|
                  |12| 6000 |----------------------- 192 -------------------------------| 10466 |
    LATCH     ____/``\__________________________________________________________________________/``

    CLK       ______________/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\___________

    DATA(SFC) `````BBBBBBBBB`YYY`SEL`STA`UP``DN``LF``RT``AAA`XXX`LLL`RRR```````````````````````````

### Pseudocode

    sfc_btn_sequence = { SFC_BTN_B,  SFC_BTN_Y,    SFC_BTN_SELECT, SFC_BTN_START,
                         SFC_BTN_UP, SFC_BTN_DOWN, SFC_BTN_LEFT,   SFC_BTN_RIGHT,
                         SFC_BTN_A,  SFC_BTN_X,    SFC_BTN_L,      SFC_BTN_R,
                         NULL,       NULL,         NULL,           NULL };

    digitalWrite(SFC_PIN_LATCH, HIGH);
    delay(12);
    digitalWrite(SFC_PIN_LATCH, LOW);
    delay(6000);

    for( i=0; i<16; i++ ){
      digitalWrite(SFC_PIN_CLK, HIGH);
      delay(12);
      Gamepad.Button[ sfc_btn_sequence[i] ] = digitalRead(SFC_PIN_DATA);
      digitalWrite(SFC_PIN_CLK, LOW);
      delay(12);
    }

    delay(10466);

### Implementation



## MD

### Principle

手柄采用5V供电，一个输入TH，6个输出TR、TL、D3、D2、D1、D0。

TH通过输入高低电平以获得数据，2组高低电平获得3键手柄的信息，4组获得6键手柄的信息

### Pseudocode

    for( i=0; i<4; i++){
      digitalWrite(MD_PIN_TH, HIGH);
      delay(12);
      Gamepad.Button[MD_BTN_C    ] = digitalRead(MD_PIN_TR);
      Gamepad.Button[MD_BTN_B    ] = digitalRead(MD_PIN_TL);
      if( i!=3 ){
        Gamepad.Button[MD_BTN_RIGHT] = digitalRead(MD_PIN_D3);
        Gamepad.Button[MD_BTN_LEFT ] = digitalRead(MD_PIN_D2);
        Gamepad.Button[MD_BTN_DOWN ] = digitalRead(MD_PIN_D1);
        Gamepad.Button[MD_BTN_UP   ] = digitalRead(MD_PIN_D0);
      }else{
        Gamepad.Button[MD_BTN_MODE ] = digitalRead(MD_PIN_D3);
        Gamepad.Button[MD_BTN_X    ] = digitalRead(MD_PIN_D2);
        Gamepad.Button[MD_BTN_Y    ] = digitalRead(MD_PIN_D1);
        Gamepad.Button[MD_BTN_Z    ] = digitalRead(MD_PIN_D0);
      }
      digitalWrite(MD_PIN_TH, LOW);
      delay(12);
      Gamepad.Button[MD_BTN_START] = digitalRead(MD_PIN_TR);
      Gamepad.Button[MD_BTN_A    ] = digitalRead(MD_PIN_TL);
      Gamepad.Button[MD_BTN_DOWN ] = digitalRead(MD_PIN_D1);
      Gamepad.Button[MD_BTN_UP   ] = digitalRead(MD_PIN_D0);
    }

### Implementation

唯一一个用满了所有信号端口的手柄

## PS/PS2

### Principle

手柄采用3.3V供电，三个输入端ATT、CLK和CMD，两个输出端DATA和ACK。

### Pseudocode

### Implementation

之前测试通过了，后来又不能用了，不知道怎么回事？？？


## SS

### Principle

手柄采用5V供电，2个输入S0和S1，4个输出D0、D1、D2、D3。

S0和S1通过输入高低电平组合以获得数据。

### Pseudocode

    digitalWrite(SS_PIN_S0, LOW);
    digitalWrite(SS_PIN_S1, LOW);
    delay(12);
    Gamepad.Button[SS_BTN_Z    ] = digitalRead(SS_PIN_D0);
    Gamepad.Button[SS_BTN_Y    ] = digitalRead(SS_PIN_D1);
    Gamepad.Button[SS_BTN_X    ] = digitalRead(SS_PIN_D2);
    Gamepad.Button[SS_BTN_R    ] = digitalRead(SS_PIN_D3);

    digitalWrite(SS_PIN_S0, HIGH);
    digitalWrite(SS_PIN_S1, LOW);
    delay(12);
    Gamepad.Button[SS_BTN_B    ] = digitalRead(SS_PIN_D0);
    Gamepad.Button[SS_BTN_C    ] = digitalRead(SS_PIN_D1);
    Gamepad.Button[SS_BTN_A    ] = digitalRead(SS_PIN_D2);
    Gamepad.Button[SS_BTN_START] = digitalRead(SS_PIN_D3);

    digitalWrite(SS_PIN_S0, LOW);
    digitalWrite(SS_PIN_S1, HIGH);
    delay(12);
    Gamepad.Button[SS_BTN_UP   ] = digitalRead(SS_PIN_D0);
    Gamepad.Button[SS_BTN_DOWN ] = digitalRead(SS_PIN_D1);
    Gamepad.Button[SS_BTN_LEFT ] = digitalRead(SS_PIN_D2);
    Gamepad.Button[SS_BTN_RIGHT] = digitalRead(SS_PIN_D3);

    digitalWrite(SS_PIN_S0, HIGH);
    digitalWrite(SS_PIN_S1, HIGH);
    delay(12);
    Gamepad.Button[SS_BTN_L    ] = digitalRead(SS_PIN_D3);

### Implementation

非常简单的一种。


## N64

### Principle

手柄采用3.3V供电

### Pseudocode

### Implementation

需参考N64Pad


## NGC

### Principle

手柄采用3.3V供电

### Pseudocode

### Implementation

需参考N64Pad


## DC

### Principle

手柄采用5V供电，使用Maple Bus进行通信。

Maple Bus参考

### Pseudocode

### Implementation

Maple Bus的实现需参考 [libmaple](https://github.com/Xerxes3rd/libmaple), [maple-bus-arduino](https://github.com/kilgariff/maple-bus-arduino), [arduino-maple](https://github.com/requeijaum/arduino-maple)


