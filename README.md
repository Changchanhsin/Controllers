# Controllers

A arduino based Joypad hardware adaptor.

基于Arduino Leonardo的游戏手柄适配器


# Functional

将游戏机手柄通过该适配器用到PC上。


# Design

## Port/Pinmap/Interface

手柄通过专用接口转到15pin D-SUB公口，Arduino通过15pin接口与手柄连接，Arduino转换手柄信息为键盘按键或手柄

15个pin分为4部分：

    公头
       +---------------------------+
      /   [ 1] [ 2] [ 3] [ 4] [ 5]  \
      \ [ 6] [ 7] [ 8] [ 9] [10]    /
       \  [11] [12] [13] [14] [15] /
        +-------------------------+

- 数据区：1、2、6、7、8、11、12
- 总线区：3（SDA）、13（SCL）
- 电源区：4（3.3V）、9（5V）、14（GND）
- 识别区：5、10
- 保留区：15

电源区负责为手柄和识别区提供电源

数据区用于发送和接收信号。

总线区用于连接IIC总线。

识别区通过电压高低自动识别手柄类型，用于初始化数据区端口定义

保留区不使用。

| TYPE       | POWER          | DATA                                                | IIC  | REG       |
| D-SUB      | 9  | 14  | 4   | 1    | 2    | 6     | 7     | 8       | 11    | 12  | 3/13 | 5   | 10  |
|:----------:|:--:|:---:|:---:|:----:|:----:|:-----:|:-----:|:-------:|:-----:|:---:|:----:|:---:|:---:|
| Atari 2600 | -  | GND | -   | UP   | DOWN | LEFT  | RIGHT | TRIGGER | -     | -   | -    | 3V3 | GND |
| FC/NES     | 5V | GND | -   | DATA | D3   | LATCH | -     | -       | CLOCK | D4  | -    | GND | 5V  |
| SFC/SNES   | 5V | GND | -   | DATA | D3   | LATCH | -     | -       | CLOCK | D4  | -    | GND | 5V  |
| MD         | 5V | GND | -   | D0   | D1   | D2    | D3    | TL      | TR    | TH  | -    | GND | 3V3 |
| SS         | 5V | GND | -   | D0   | D1   | D2    | D3    | -       | S0    | S1  | -    | 3V3 | 3V3 |
| PS         | 9V | GND | 3V3 | DATA | ACK  | ATT   | -     | -       | CMD   | CLK | -    | GND | 5V  |
| N64        | 5V | GND | 3V3 | -    | -    | DATA  | -     | -       | -     | -   | -    | 5V  | 3V3 |
| GC         | 5V | GND | 3V3 | -    | -    | DATA  | -     | -       | -     | -   | -    | 5V  | 3V3 |
| WII        | -  | GND | 3V3 | -    | -    | -     | -     | -       | -     | -   | IIC  |     |     |
| DC         | 5V | GND | -   | D1   | D5   | Sense | -     | -       | -     | -   | -    | 3V3 | 5V  |

## Joystick/Keymap

|   HID   | Gamepad | Keyboard  |     Atari     |   FC   |  SFC   |  MD   | PS/PS2 |  SS   |  DC   |  N64  |  NGC  |  WII  |
|:-------:|:-------:|:---:|:---:|:-------------:|:------:|:------:|:-----:|:------:|:-----:|:-----:|:-----:|:-----:|:-----:|
| Axis X  | Axis X  |     |     |               |        |        |       | Axis X |       | AxisX |       | AxisX | AxisX |
| (L-R+)  | D-PadLR |     |     | Joystick L/R  | D-Pad  | D-Pad  | D-Pad | D-Pad  | D-Pad | D-Pad | D-Pad | D-Pad | D-Pad |
| Axis Y  | Axis Y  |     |     |               |        |        |       | Axis Y |       | AxisY |       | AxisY | AxisY |
| (U-D+)  | D-PadUD |     |     | Joystick U/D  | D-Pad  | D-Pad  | D-Pad | D-Pad  | D-Pad | D-Pad | D-Pad | D-Pad | D-Pad |
| B1      | A       | x   | h   | Trigger       | A      | A      | B     | Cross  | B     | B     | A     | A     | A     |
| B2      | B       | c   | j   | Game Select   | B      | B      | A     | Circle | A     | A     | B     | B     | B     |
| B3      | X       | s   | u   |               |        | X      | Y     | Rect   | Y     | Y     | Cr    | X     | X     |
| B4      | Y       | d   | i   |               |        | Y      | X     | Tri    | X     | X     | Cd    | Y     | Y     |
| B5      | L1      | q   | 6   |               |        | L      | Z     | L1     | C     | L     | Cl    | L     | ZL    |
| B6      | R1      | e   | 8   |               |        | R      | C     | R1     | Z     | R     | Cu    | R     | ZR    |
| B7      | SELECT  |space|     | Player Select | Select | Select | Mode  | Select | Mode  |       | Z     | Z     | -     |
| B8      | START   |enter|     | Reset         | Start  | Start  | Start | Start  | Start | Start | Start | Start | +     |
| B9      | L2      | w   |     |               |        |        |       | L2     | L     |       | L     |       | L     |
| B10     | R2      | r   |     |               |        |        |       | R2     | R     |       | R     |       | R     |
| B11     | LJ      |     |     |               |        |        |       | LJ     |       |       |       |       |       |
| B12     | RJ      |     |     |               |        |        |       | RJ     |       |       |       |       |       |
| B13     | HOME    |     |     |               |        |        |       |        |       |       |       |       | Home  |
| Axis Z  | Axis Z  |     |     |               |        |        |       |        |       |       |       |       |       |
| Axis RX | Axis RX |     |     |               |        |        |       |        |       |       | AxisX |       |       |
| Axis RY | Axis RY |     |     |               |        |        |       |        |       |       | AxisY |       |       |
| Axis RZ | Axis RZ |     |     |               |        |        |       |        |       |       |       |       |       |
| Cap     | Cap     |     |     |               |        |        |       |        |       |       |       |       |       |


# GEN-1 开关型

## Atari 2600

### Principle

Aatari 2600有一个摇杆和一个按钮，使用9针母口，无芯片，5个开关。

有手柄将上下同时按定义为“Player Select”，
左右同时按定义为"Reset"，
上下左同时按定义为“Game Select”。

    +--+--+--+--+--+
    |  |  |  |  |  |
    T  L  R  D  U  |
    |  |  |  |  |  |
    6  3  4  2  1  8
    I  I  I  I  I  COM

### Pseudocode

#### 针脚定义

| Define            | Pin   | Mode         | DE15 Pin | Color Joystick |
|:-----------------:|:-----:|:------------:|:--------:|:--------------:|
| A2600_PIN_UP      | 0     | INPUT_PULLUP | 1        | (1)Gray        |
| A2600_PIN_DOWN    | 1     | INPUT_PULLUP | 2        | (2)Blue        |
| A2600_PIN_LEFT    | 2     | INPUT_PULLUP | 6        | (3)Green       |
| A2600_PIN_RIGHT   | 5     | INPUT_PULLUP | 11       | (4)Red         |
| A2600_PIN_TRIGGER | 6     | INPUT_PULLUP | 12       | (6)Orange      |
| A2600_PIN_COM     | 3     | OUTPUT       | 7        | (8)Black       |

#### 按键定义

    #define A2600_UP            GAMEPAD_UP
    #define A2600_DOWN          GAMEPAD_DOWN
    #define A2600_LEFT          GAMEPAD_LEFT
    #define A2600_RIGHT         GAMEPAD_RIGHT
    #define A2600_TRIGGER       GAMEPAD_B
    #define A2600_PLAYERSELECT  GAMEPAD_SELECT
    #define A2600_RESET         GAMEPAD_START
    #define A2600_GAMSELECT     GAMEPAD_HOME

#### 获取数据

    A2600_PIN_COM = LOW
    u = ~A2600_PIN_UP
    d = ~A2600_PIN_DOWN
    l = ~A2600_PIN_LEFT
    r = ~A2600_PIN_RIGHT
    if( u == d == l == HIGH && r == LOW )
      A2600_GAMSELECT    = PRESSED
      A2600_PLAYERSELECT = RELEASE
      A2600_RESET        = RELEASE
      A2600_UP           = RELEASE
      A2600_DOWN         = RELEASE
      A2600_LEFT         = RELEASE
      A2600_RIGHT        = RELEASE
    }else{
      A2600_GAMSELECT = RELEASE
      if( u == d == HIGH && l == r == LOW ){
        A2600_PLAYERSELECT = PRESSED
        A2600_RESET        = RELEASE
        A2600_UP           = RELEASE
        A2600_DOWN         = RELEASE
        A2600_LEFT         = RELEASE
        A2600_RIGHT        = RELEASE
      }else if( u == d == LOW && l == r == HIGH ){
        A2600_PLAYERSELECT = RELEASE
        A2600_RESET        = PRESSED
        A2600_UP           = RELEASE
        A2600_DOWN         = RELEASE
        A2600_LEFT         = RELEASE
        A2600_RIGHT        = RELEASE
      }else{
        A2600_PLAYERSELECT = RELEASE
        A2600_RESET        = RELEASE
        A2600_UP           = u
        A2600_DOWN         = d
        A2600_LEFT         = l
        A2600_RIGHT        = r
    }
  }
  A2600_TRIGGER = ~A2600_PIN_TRIGGER

### Implementation

公共端低电平，五个信号端接5个数据区针，数据区针初始化为INPUT_PULLUP，无操作时为高电平，按下时为低电平。


## Atari 5200

### Principle

Atari 5200有一个模拟摇杆、两个按键、4x4的键盘，
使用15针公口，无芯片，2个模拟方向为0(L/T)-500(R/D)欧电阻，
开关按下接地，4x4键盘通过扫描获取值。

         ROW0 ROW1 ROW2 ROW3
    COL0  1    2    3    
    COL1  4    5    6
    COL2  7    8    9
    COL3  *    0    #

### Pseudocode

#### 针脚定义

| Define                | Pin-15 | Pin-9 | Mode         | DE15 Pin     | Color Joystick |
|:---------------------:|:------:|:-----:|:------------:|:------------:|:--------------:|
| A5200_PIN_POT_X       | 11     | 8     | INPUT_PULLUP | 不能直接连接  | (10)           |
| A5200_PIN_POT_Y       | 10     | 6     | INPUT_PULLUP | 2            | (11)           |
| A5200_PIN_POT_COMMON  | 9      | 7     | OUTPUT       |
| A5200_PIN_KEYBOARD_R0 | 7      |       | OUTPUT       | 6            | (8)            |
| A5200_PIN_KEYBOARD_R1 | 6      |       | OUTPUT       | 6            | (7)            |
| A5200_PIN_KEYBOARD_R2 | 5      |       | OUTPUT       | 6            | (6)            |
| A5200_PIN_KEYBOARD_R3 | 8      |       | OUTPUT       | 6            | (5)            |
| A5200_PIN_KEYBOARD_C0 | 1      |       | INPUT_PULLUP | 6            | (4)            |
| A5200_PIN_KEYBOARD_C1 | 2      |       | INPUT_PULLUP | 6            | (3)            |
| A5200_PIN_KEYBOARD_C2 | 3      |       | INPUT_PULLUP | 6            | (2)            |
| A5200_PIN_KEYBOARD_C3 | 4      |       | INPUT_PULLUP | 6            | (1)            |
| A5200_PIN_SIDE_TOP    | 14     | 4     | INPUT_PULLUP | 11           | (4)Red         |
| A5200_PIN_SIDE_BOTTOM | 13     | 5     | INPUT_PULLUP | 12           | (6)Orange      |
| A5200_PIN_GND         | 15     | 3     | OUTPUT       | 7            | (8)Black       |
| A5200_PIN_VCC         | 12     | 1     | OUTPUT       | 7            | (8)Black       |


#### 按键定义

    A5200_KEYBOARD_0     GAMEPAD_LEFT
    A5200_KEYBOARD_1     GAMEPAD_L1
    A5200_KEYBOARD_2     GAMEPAD_DOWN
    A5200_KEYBOARD_3     GAMEPAD_R1
    A5200_KEYBOARD_4     GAMEPAD_L2
    A5200_KEYBOARD_5     GAMEPAD_RIGHT
    A5200_KEYBOARD_6     GAMEPAD_R2
    A5200_KEYBOARD_7     GAMEPAD_LJ
    A5200_KEYBOARD_8     GAMEPAD_UP
    A5200_KEYBOARD_9     GAMEPAD_RJ
    A5200_KEYBOARD_SHARP GAMEPAD_X       // #
    A5200_KEYBOARD_STAR  GAMEPAD_Y       // *
    A5200_KEYBOARD_START GAMEPAD_START
    A5200_KEYBOARD_PAUSE GAMEPAD_SELECT
    A5200_KEYBOARD_RESET GAMEPAD_HOME
    A5200_SIDE_TOP       GAMEPAD_A
    A5200_SIDE_BOTTOM    GAMEPAD_B
    A5200_POT_X          GAMEPAD_AXIS_X
    A5200_POT_Y          GAMEPAD_AXIS_Y

#### 获取数据

    setCol(int pin){
      col[4]={A5200_PIN_KEYBOARD_C0,
              A5200_PIN_KEYBOARD_C1,
              A5200_PIN_KEYBOARD_C2,
              A5200_PIN_KEYBOARD_C3};
      for(int i=0;i<4;i++){
        if(pin==i)
          col[i]=HIGH;
        else
          col[i]=LOW;
      }
    }

    button[4][4]={};
    for(i=0;i<4;i++){
      setCol(i);
      for(j=0;j<4;j++){
        
      }
    }

### Implementation

需要专门一个单片机处理。可以转成自定义协议进来，比如串口/IIC？


## Atari 7800

### Principle

Atari 7800有一个摇杆和两个按钮，使用9针母口，无芯片。
4个方向开关按下为低，放开为高；
2个按键开关按下为高，放开为低。

        +---+------+--+--+--+--+
        |   |      |  |  |  |  |
       620 620     |  |  |  |  |
        |   |      |  |  |  |  |
    +---+   +---+  |  |  |  |  |
    |   |   |   |  |  |  |  |  |
    |   FR  FL  |  L  R  D  U  |
    |   |   |   |  |  |  |  |  |
    |   +---+   |  |  |  |  |  |
    |   |       |  |  |  |  |  |
    5   6       9  4  3  2  1  8
    I   VCC     I  I  I  I  I  GND

### Pseudocode

#### 针脚定义

| Define            | Pin   | Mode         | DE15 Pin | Color Joystick |
|:-----------------:|:-----:|:------------:|:--------:|:--------------:|
| A7800_PIN_UP      | 0     | INPUT_PULLUP | 1        | (1)Yellow      |
| A7800_PIN_DOWN    | 1     | INPUT_PULLUP | 2        | (2)Blue        |
| A7800_PIN_LEFT    | 2     | INPUT_PULLUP | 6        | (3)Green       |
| A7800_PIN_RIGHT   | 5     | INPUT_PULLUP | 11       | (4)Brown       |
| A7800_PIN_FIRE_R  | 6     | INPUT_PULLUP | 12       | (5)Orange      |
| A7800_PIN_FIRECOM | VCC   |              |          | (6)Red         |
| A7800_PIN_COM     | GND   |              |          | (8)Black       |
| A7800_PIN_FIRE_L  | 3     | INPUT_PULLUP | 7        | (9)Violet      |

#### 按键定义

    #define A7800_UP            GAMEPAD_UP
    #define A7800_DOWN          GAMEPAD_DOWN
    #define A7800_LEFT          GAMEPAD_LEFT
    #define A7800_RIGHT         GAMEPAD_RIGHT
    #define A7800_FIRE_L        GAMEPAD_A
    #define A7800_FIRE_R        GAMEPAD_B

#### 获取数据

    A7800_UP     = ~A7800_PIN_UP
    A7800_DOWN   = ~A7800_PIN_DOWN
    A7800_LEFT   = ~A7800_PIN_LEFT
    A7800_RIGHT  = ~A7800_PIN_RIGHT
    A7800_FIRE_L = A7800_PIN_FIRE_L
    A7800_FIRE_R = A7800_PIN_FIRE_R

### Implementation



# GEN-2.1 锁存器型(并行)


## SS

### Principle

手柄采用5V供电，2个输入S0和S1，4个输出D0、D1、D2、D3。

S0和S1通过输入高低电平组合以获得数据。低电平为按下，

### Interface

| Define    | Pin | Mode         | DE15 Pin | Color     | Note |
|:---------:|:---:|:------------:|:--------:|:---------:|:----:|
| 5V        | -   | -            | 4        | (1)BLACK  |      |
| SS_PIN_D1 | 1   | INPUT_PULLUP | 2        | (2)BROWN  |      |
| SS_PIN_D0 | 0   | INPUT_PULLUP | 1        | (3)RED    |      |
| SS_PIN_S0 | 5   | OUTPUT       | 11       | (4)ORANGE |      |
| SS_PIN_S1 | 6   | OUTPUT       | 12       | (5)YELLOW |      |
| 5V        | -   | -            | 4        | (6)GREEN  |      |
| SS_PIN_D3 | 3   | INPUT_PULLUP | 7        | (7)BLUE   |      |
| SS_PIN_D2 | 2   | INPUT_PULLUP | 6        | (8)GREY   |      |
| GND       | -   | -            | 14       | (9)WHITE  |      |

### Pseudocode

#### 按键定义

    #define SS_UP    GAMEPAD_UP
    #define SS_DOWN  GAMEPAD_DOWN
    #define SS_LEFT  GAMEPAD_LEFT
    #define SS_RIGHT GAMEPAD_RIGHT
    #define SS_A     GAMEPAD_B
    #define SS_B     GAMEPAD_A
    #define SS_C     GAMEPAD_R1
    #define SS_START GAMEPAD_START
    #define SS_X     GAMEPAD_Y
    #define SS_Y     GAMEPAD_X
    #define SS_Z     GAMEPAD_L1
    #define SS_L     GAMEPAD_L2
    #define SS_R     GAMEPAD_R2

#### 获取数据

    SS_PIN_S0 = LOW
    SS_PIN_S1 = LOW
    delay(12)
    SS_Z = ~SS_PIN_D0
    SS_Y = ~SS_PIN_D1
    SS_X = ~SS_PIN_D2
    SS_R = ~SS_PIN_D3

    SS_PIN_S0 = HIGH
    SS_PIN_S1 = LOW
    delay(12)
    SS_B     = ~SS_PIN_D0
    SS_C     = ~SS_PIN_D1
    SS_A     = ~SS_PIN_D2
    SS_START = ~SS_PIN_D3

    SS_PIN_S0 = LOW
    SS_PIN_S1 = HIGH
    delay(12)
    SS_UP    = ~SS_PIN_D0
    SS_DOWN  = ~SS_PIN_D1
    SS_LEFT  = ~SS_PIN_D2
    SS_RIGHT = ~SS_PIN_D3

    SS_PIN_S0 = HIGH
    SS_PIN_S1 = HIGH
    delay(12)
    SS_L = ~SS_PIN_D3

### Implementation

非常简单的一种。


## MD

### Principle

手柄采用5V供电，一个输入TH，6个输出TR、TL、D3、D2、D1、D0。

TH通过输入高低电平以获得数据，2组高低电平获得3键手柄的信息，4组获得6键手柄的信息。低电平为按下。

### Pseudocode

#### 针脚定义

    #define MD_PIN_TH PIN_1
    #define MD_PIN_TR PIN_2
    #define MD_PIN_TL PIN_3
    #define MD_PIN_D0 PIN_4
    #define MD_PIN_D1 PIN_5
    #define MD_PIN_D2 PIN_6
    #define MD_PIN_D3 PIN_7

#### 针脚初始化

    pinMode(MD_PIN_TH, OUTPUT      );
    pinMode(MD_PIN_TL, INPUT_PULLUP);
    pinMode(MD_PIN_TR, INPUT_PULLUP);
    pinMode(MD_PIN_D0, INPUT_PULLUP);
    pinMode(MD_PIN_D1, INPUT_PULLUP);
    pinMode(MD_PIN_D2, INPUT_PULLUP);
    pinMode(MD_PIN_D3, INPUT_PULLUP);

#### 按键定义

    #define MD_UP    GAMEPAD_UP
    #define MD_DOWN  GAMEPAD_DOWN
    #define MD_LEFT  GAMEPAD_LEFT
    #define MD_RIGHT GAMEPAD_RIGHT
    #define MD_A     GAMEPAD_B
    #define MD_B     GAMEPAD_A
    #define MD_C     GAMEPAD_R1
    #define MD_START GAMEPAD_START
    #define MD_MODE  GAMEP AD_SELECT
    #define MD_X     GAMEPAD_Y
    #define MD_Y     GAMEPAD_X
    #define MD_Z     GAMEPAD_L1

#### 获取数据

    for( i=0; i<4; i++){
      MD_PIN_TH = HIGH
      delay(12);
      MD_C = ~MD_PIN_TR
      MD_B = ~MD_PIN_TL
      if( i!=3 ){
        MD_RIGHT = ~MD_PIN_D3
        MD_LEFT  = ~MD_PIN_D2
        MD_DOWN  = ~MD_PIN_D1
        MD_UP    = ~MD_PIN_D0
      }else{
        MD_MODE = ~MD_PIN_D3
        MD_X    = ~MD_PIN_D2
        MD_Y    = ~MD_PIN_D1
        MD_Z    = ~MD_PIN_D0
      }
      MD_PIN_TH = LOW
      delay(12);
      MD_START = ~MD_PIN_TR
      MD_A     = ~MD_PIN_TL
      MD_DOWN  = ~MD_PIN_D1
      MD_UP    = ~MD_PIN_D0
    }

### Implementation



# GEN-2.2 锁存器型(串行)

## FC/NES

### Principle

手柄采用5V供电，两个输入端LATCH和CLOCK、一个输出端DATA和若干其他DATA端口。

- LATCH常低，通过一个12微秒的高电平激活手柄输入。
- LATCH高电平后等待6000微秒，连续发送CLOCK高电平脉冲以获得数据，每个脉冲宽度为12微秒高12微秒低，共16组。
- DATA信号常高，CLOCK信号开始后下降沿读数据，低电平为按下，数据顺序为按键A、B、Select、Start、Up、Down、Left、Right。
- 2P有DATA2用于麦克风
- 光枪有两个短空用于检测扳机和光传感器

### Timing

                  |-------------------------------- 16670 --------------------------------------|
                  |12| 6000 |----------------------- 192 -------------------------------| 10466 |
    LATCH(us) ____/``\__________________________________________________________________________/`

    CLOCK     ______________/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\___________

    DATA(FC)  ``````AAAAAAAAA`BBB`SEL`STA`UP``DN``LF``RT``````````````````````````````````````````

### Pseudocode

#### 针脚定义

| Define         | Pin | Mode         | DE15 Pin | Color P1  | Color P2  | Color NES | Note |
|:--------------:|:---:|:------------:|:--------:|:---------:|:---------:|:---------:|:----:|
| GND            | -   | -            | 14       | (1)Brown  | (1)Red    | (1)Brown  |      |
| FC_PIN_DATA    | 0   | INPUT_PULLUP | 1        | (2)Red    | (2)Orange | (2)Red    |      |
| FC_PIN_LATCH   | 2   | OUTPUT       | 6        | (3)Orange | (3)Yellow | (3)Orange |      |
| FC_PIN_CLOCK   | 5   | OUTPUT       | 11       | (4)Yellow | (4)White  | (4)Yellow |      |
| 5V             | -   | -            | 9        | (5)White  | (5)Blue   | (5)White  |      |
| FC_PIN_MIC     | 3   | ???          | 7        | -         | (6)Brown  |           | D2   |
| FC_PIN_TRIGGER | 6   | ???          | 12       | -         |           | (6)Purple | D4   |
| FC_PIN_SENSOR  | 1   | ???          | 2        | -         |           | (7)Cyan   | D3   |

#### 按键定义

    #define FC_UP     GAMEPAD_UP
    #define FC_DOWN   GAMEPAD_DOWN
    #define FC_LEFT   GAMEPAD_LEFT
    #define FC_RIGHT  GAMEPAD_RIGHT
    #define FC_A      GAMEPAD_B
    #define FC_B      GAMEPAD_A
    #define FC_START  GAMEPAD_START
    #define FC_SELECT GAMEPAD_SELECT

#### 获取数据

    fc_btn_sequence = { FC_A,  FC_B,    FC_SELECT, FC_START,
                        FC_UP, FC_DOWN, FC_LEFT,   FC_RIGHT };

    digitalWrite(FC_PIN_LATCH, HIGH);
    delay(12);
    digitalWrite(FC_PIN_LATCH, LOW);
    delay(6000);

    for( i=0; i<8; i++ ){
       fc_btn_sequence[i] ] = ~FC_PIN_DATA);
      digitalWrite(FC_PIN_CLOCK, HIGH);
      delay(6);
      digitalWrite(FC_PIN_CLOCK, LOW);
      delay(6);
    }
    
    delay(96);

    delay(10466);

### Implementation


## SFC/SNES

### Principle

手柄采用5V供电，两个输入端LATCH和CLOCK、一个输出端DATA。

- LATCH常低，通过一个12微秒的高电平激活手柄输入。
- LATCH高电平后等待6000微秒，连续发送CLOCK高电平脉冲以获得数据，每个脉冲宽度为12微秒高12微秒低，共16组。
- DATA信号常高，CLOCK信号开始后下降沿读数据，低电平为按下，数据顺序为按键B、Y、Select、Start、Up、Down、Left、Right、A、X、L、R。

### Timing

                  |-------------------------------- 16670 --------------------------------------|
                  |12| 6000 |----------------------- 192 -------------------------------| 10466 |
    LATCH(us) ____/``\__________________________________________________________________________/`

    CLOCK     ______________/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\_/`\___________

    DATA(SFC) ``````BBBBBBBBB`YYY`SEL`STA`UP``DN``LF``RT``AAA`XXX`LLL`RRR`````````````````````````

### Pseudocode

#### 针脚定义

| Define          | Pin | Mode         | DE15 Pin | Color P1  | Note |
|:---------------:|:---:|:------------:|:--------:|:---------:|:----:|
| 5V              | -   | -            | 9        | (1)White  |      |
| SFC_PIN_CLOCK   | 5   | OUTPUT       | 11       | (2)Yellow |      |
| SFC_PIN_LATCH   | 2   | OUTPUT       | 6        | (3)Orange |      |
| SFC_PIN_DATA    | 0   | INPUT_PULLUP | 1        | (4)Red    |      |
| SFC_PIN_TRIGGER | 6   | ???          | 12       | (5)???    | D4   |
| SFC_PIN_SENSOR  | 1   | ???          | 2        | (6)???    | D3   |
| GND             | -   | -            | 14       | (7)Brown  |      |


#### 按键定义

    #define SFC_UP     GAMEPAD_UP
    #define SFC_DOWN   GAMEPAD_DOWN
    #define SFC_LEFT   GAMEPAD_LEFT
    #define SFC_RIGHT  GAMEPAD_RIGHT
    #define SFC_A      GAMEPAD_B
    #define SFC_B      GAMEPAD_A
    #define SFC_X      GAMEPAD_X
    #define SFC_Y      GAMEPAD_Y
    #define SFC_START  GAMEPAD_START
    #define SFC_SELECT GAMEPAD_SELECT
    #define SFC_L      GAMEPAD_L1
    #define SFC_R      GAMEPAD_R1

#### 获取数据

    sfc_btn_sequence = { SFC_B,  SFC_Y,    SFC_SELECT, SFC_START,
                         SFC_UP, SFC_DOWN, SFC_LEFT,   SFC_RIGHT,
                         SFC_A,  SFC_X,    SFC_L,      SFC_R,
                         NULL,   NULL,     NULL,       NULL };

    digitalWrite(SFC_PIN_LATCH, HIGH);
    delay(12);
    digitalWrite(SFC_PIN_LATCH, LOW);
    delay(6000);

    for( i=0; i<16; i++ ){
       sfc_btn_sequence[i] ] = ~SFC_PIN_DATA);
      digitalWrite(SFC_PIN_CLOCK, HIGH);
      delay(6);
      digitalWrite(SFC_PIN_CLOCK, LOW);
      delay(6);
    }

    delay(10466);

### Implementation


## NEC PCE

### Principle


### Timing


### Pseudocode

#### 针脚定义

| Define         | Pin | Mode         | DE15 Pin | Color P1  | Color P2  | Color NES | Note |
|:--------------:|:---:|:------------:|:--------:|:---------:|:---------:|:---------:|:----:|
| GND            | -   | -            | 14       | (1)Brown  | (1)Red    | (1)Brown  |      |
| PCE_PIN_DATA   | 0   | INPUT_PULLUP | 1        | (2)Red    | (2)Orange | (2)Red    |      |
| PCE_PIN_LATCH  | 2   | OUTPUT       | 6        | (3)Orange | (3)Yellow | (3)Orange |      |
| FC_PIN_CLOCK   | 5   | OUTPUT       | 11       | (4)Yellow | (4)White  | (4)Yellow |      |
| 5V             | -   | -            | 9        | (5)White  | (5)Blue   | (5)White  |      |
| FC_PIN_MIC     | 3   | ???          | 7        | -         | (6)Brown  |           | D2   |
| FC_PIN_TRIGGER | 6   | ???          | 12       | -         |           | (6)Purple | D4   |
| FC_PIN_SENSOR  | 1   | ???          | 2        | -         |           | (7)Cyan   | D3   |

#### 按键定义

//https://docs.libretro.com/library/beetle_pce_fast/#joypad


    #define PCE_UP        GAMEPAD_UP
    #define PCE_DOWN      GAMEPAD_DOWN
    #define PCE_LEFT      GAMEPAD_LEFT
    #define PCE_RIGHT     GAMEPAD_RIGHT
    #define PCE_I         GAMEPAD_A
    #define PCE_II        GAMEPAD_B
    #define PCE_III       GAMEPAD_Y
    #define PCE_TURBO2    GAMEPAD_Y
    #define PCE_IV        GAMEPAD_X
    #define PCE_TURBO1    GAMEPAD_X
    #define PCE_V         GAMEPAD_L1
    #define PCE_VI        GAMEPAD_R1
    #define PCE_RUN       GAMEPAD_START
    #define PCE_SELECT    GAMEPAD_SELECT
    #define PCE_MODE      GAMEPAD_L2
    #define PCE_MODE1     GAMEPAD_L2
    #define PCE_MODE2     GAMEPAD_R2
    #define PCE_TURBOALT1 GAMEPAD_LJ
    #define PCE_TURBOALT2 GAMEPAD_RJ

#### 获取数据

### Implementation



## SNK NEO-GEO

### Principle


### Timing


### Pseudocode

#### 针脚定义

手柄端口：10输入, 3输出, 2电源线.
DA-15公头，正面：

NEOGEO_PIN_GND    GND
NEOGEO_PIN_OUT2   OUTPUT
NEOGEO_PIN_SELECT INPUT
NEOGEO_PIN_D      INPUT
NEOGEO_PIN_B      INPUT
NEOGEO_PIN_RIGHT  INPUT
NEOGEO_PIN_DOWN   INPUT
NEOGEO_PIN_VCC    5V

NEOGEO_PIN_OUT3   OUTPUT
NEOGEO_PIN_OUT1   OUTPUT
NEOGEO_PIN_START  INPUT
NEOGEO_PIN_C      INPUT
NEOGEO_PIN_A      INPUT
NEOGEO_PIN_LEFT   INPUT
NEOGEO_PIN_UP     INPUT


#### 按键定义

#### 获取数据

### Implementation





# GEN-3 总线型


## Panasonic 3DO

### Principle

### Protocol

3DO PBUS / Player Bus. 

Since the above described arrangement permits 448 bits to be
transferred per field and the minimum bit transfer per device is
8bits, a maximum of approximately 56 devices can be on the bus at
one time.

Each of the player devices has an ID code embedded in its response
data. An ID code is mapped by software in the system to the number
of input and output bits for the device identified. Since the ID
code is embedded, it is received by the system 100 both at
initialization and when data is being read in under normal
operation. In this manner, it is possible to reconfigure the
system (add or delete devices) without re-initializing and without
adversely affecting those device already on the line.

During an initialization sequence, a string of zeros is shifted
out from the system 100. A requirement for all of the player
devices is that they see a string of zeros either as an
initialization or don't cares.

The first 4 bits of data stream received by the system 100 from
each device is that device's basic identification code (ID
code). The embedding of an ID code in the data stream functions as
follows for a joystick 15. The basic joystick 15 will have 8 bits
of data that are (inverted): down, up, right, left, switch-2,
switch-1, fire-2 and fire-1. Using the inability of the joystick
to indicate both up and down at the same time, the ID codes for a
basic joystick are 01QQ, 10QQ and 11QQ, where QQ is 01, 10 or
11. All other devices start with 00QQ. All joysticks are incapable
of generating 00 for their first 2 bits. With regard to the escape
function, the 1100 sequence indicates escape for the joystick. In
all other devices, the escape function is indicated by the bit
immediately following the 4 bit identification code. A string of
zeros indicate the end of input data into the system 100 device.

Control Pad / Joypad
Bits (MSB → LSB)	Description
1bit	set, the high order bit from the ID (0x80)
2bit	unused, unset, part of the joypad mask
1bit	set if DPAD DOWN pressed
1bit	set if DPAD UP pressed
1bit	set if DPAD RIGHT pressed
1bit	set if DPAD LEFT pressed
1bit	set if A pressed
1bit	set if B pressed
1bit	set if C pressed
1bit	set if P pressed
1bit	set if X pressed
1bit	set if RIGHT TRIGGER pressed
1bit	set if LEFT RIGGER pressed
2bit	unused? unset with normal joypad

Flightstick / Analog Controller
Bits (MSB → LSB)	Description
8bit	identifier 0 (0x01)
8bit	identifier 1 (0x7B)
8bit	length? (0x08)
8bit	horizontal position
2bit	unused? / unset
8bit	vertical position
2bit	unused? / unset
8bit	depth position
4bit	0x2 (unsure meaning)
1bit	set if TRIGGER / FIRE pressed
1bit	set if A pressed
1bit	set if B pressed
1bit	set if C pressed
1bit	set if DPAD UP pressed
1bit	set if DPAD DOWN pressed
1bit	set if DPAD RIGHT pressed
1bit	set if DPAD LEFT pressed
1bit	set if P pressed
1bit	set if X pressed
1bit	set if LEFT TRIGGER pressed
1bit	set if RIGHT TRIGGER pressed
4bit	unused? / unset
Mouse
Bits (MSB → LSB)	Description
8bit	identifier (0x49)
1bit	set if LEFT BUTTON pressed
1bit	set if MIDDLE BUTTON pressed
1bit	set if RIGHT BUTTON pressed
1bit	set if SHIFT BUTTON pressed
10bit	x delta (signed)
10bit	y delta (signed)
Lightgun
According to the 3DO SDK's Examples/EventBroker/LightGun/lightgun.c:

DESCRIPTION OF LIGHTGUN HARDWARE
--------------------------------
The lightgun is not capable of returning an (X, Y) position value when
reporting events.  It does report a counter value that returns the
time it took between the beginning of a field (vertical blank) and
when the scan-beam passes into the field of view of the light sensor
in the lightgun.  The X and Y value can be calculated from the timer
value returned from the lightgun based on how long it takes to draw
from the start of one scan line to the start of the scan line
immediately below it (YSCANTIME) and how long it takes to draw from
the first pixel to the last pixel on the same scan line (XSCANTIME).
A third required value, is a time offset value that takes into
consideration the time between the beginning of a field and when
scan-beam begings drawing to the first pixel on the first scan line
(TIMEOFFSET).

#define NTSC_DEFAULT_XSCANTIME    1030
#define NTSC_DEFAULT_YSCANTIME    12707
#define NTSC_DEFAULT_TIMEOFFSET   -12835
#define PAL_DEFAULT_XSCANTIME     1051
#define PAL_DEFAULT_YSCANTIME     12796
#define PAL_DEFAULT_TIMEOFFSET    -58995
#define PAL_WIDTH                 388
#define NTSC_WIDTH                320

x = ((((10 * counter) % NTSC_DEFAULT_YSCANTIME) * NTSC_WIDTH) /
     (NTSC_DEFAULT_XSCANTIME * 10));
y = ((10 * counter) / NTSC_DEFAULT_YSCANTIME);
From the 3DO SDK's Interfaces/2p5/includes/event.h:

-  The "Lightgun" generic class and event structures are for the use
   of pods/devices which can't report X and Y positions, but only a
      time (counter value) between the beginning of field, and the time
      that the scan-beam passed into the field-of-view of the lightgun
      sensor.  Some lightguns can also send in a "signal quality" pulse
      counter, this being the number of successive horizontal scan lines
      during which the beam was seen at roughly the same horizontal
      posotion.  A line pulse count of 0 means "no hit".

typedef struct LightGunEventData {
  uint32         lged_ButtonBits;       /* left justified, zero fill */
  uint32         lged_Counter;          /* counter at top-center of hit */
  uint32         lged_LinePulseCount;   /* # of scan lines which were hit */
} LightGunEventData;

#define LightGunTrigger      0x80000000
Bits (MSB → LSB)	Description
8bit	identifier (0x4D)
1bit	set if TRIGGER pressed
1bit	set if SERVICE pressed (arcade)
1bit	set if COIN pressed (arcade)
1bit	set if START pressed (arcade)
1bit	set if HOLSTER (arcade) or OPTION (retail) pressed
1bit	unused? / unset
1bit	unused? / unset
20bit	Counter: counter at top-center of hit
5bit	LinePulseCount: number of scanlines which were hit
Orbatak Trackball
Orbatak has two device types. The trackballs are the same as the regular mouse as described above.

Orbatak Coin / Start / Service
The extra buttons needed for the coin slots, start buttons, and service button are provided by the “SILLY_CONTROL_PAD” as it's defined in the 3DO SDK (event.h).

Bits (MSB → LSB)	Description
8bit	identifier (0xC0)
8bit	0x00 / unset
3bit	unused / unset
1bit	set if COIN (P1) pressed
1bit	set if COIN (P2) pressed
1bit	set if START (P2) pressed
1bit	set if START (P1) pressed
1bit	set if SERVICE pressed
8bit	0x00 / unset
Portfolio Drivers
The 3DO's Portfolio operating system has the ability to dynamically load drivers for peripherals. These “ROM” files, which appear to be AIF files of dynamically loadable code, can live on the filesystem or be sent over the PBUS by the device.

Details about PBUS devices can primarily be found in two locations:

event.h : The header provided in the SDK for developers of the platform
portfolio_os/src/input : The core operating system code that manages the high level POD abstraction
Here is a list of known drivers:

Device	Device ID	Driver	Notes
Default	none	DefaultDriver.c	
control pad	embedded into first byte	ControlPadDriver.c	
analog stick	0x01	StickDriver.c and cport1.rom	Unsure about there being apparently a builtin and dynamically loaded driver. Unsure what the difference is between 0x01 device ID version and the 0x4C version.
keyboard	0x02 or 0x4B	KeyboardDriver.c	KeyboardDriver.c appears incomplete and non-functional.
mouse	0x49	MouseDriver.c and cport49.rom	ROM can be found in numerous SDKs.
stereoscopic glasses	0x41	cport41.rom	ROM can be found in numerous SDKs.
lightgun	0x4D	LightGunRom.c and cport4D.rom	ROM can be found in numerous SDKs.
splitter	0x56	SplitterDriver.c	
splitter 2	0x57	?? SplitterDriver.c	A different kind of splitter? Or a second splitter in the same device chain?
ir receiver	0x03	cport3.rom	ROM only found in the "3DO Dev Tools" archive under “3DO_OS/net1p0d8/remote/System/Drivers/“


### Pseudocode

#### 针脚定义

3DO_PIN_RAG INPUT  Audio Ground for Right Channel
3DO_PIN_VCC GND   
3DO_PIN_RA  INPUT  High impedance audio signal for Right Channel
3DO_PIN_LA  INPUT  High impedance audio signal for Left Channel
3DO_PIN_VCC 5V
3DO_PIN_DO  INPUT  Data output from System to Controllers
3DO_PIN_CLK INPUT  Clock signal, indicates each cycle at the Control Port and register transfer times
3DO_PIN_LAG INPUT  Audio Ground for Left Channel
3DO_PIN_DI  OUTPUT Data input from System to Controllers

#### 按键定义

3DO_UP    UP
3DO_DOWN  DOWN
3DO_LEFT  LEFT
3DO_RIGHT RIGHT
3DO_A     B
3DO_B     A
3DO_C     X
3DO_L     L1
3DO_R     R1
3DO_X     SELECT
3DO_STOP  SELECT
3DO_P     START
3DO_PLAY  START
3DO_PAUSE START


#### 子类型

3DO_SUBTYPE_CONTROLPAD  0x80
3DO_SUBTYPE_FLIGHTSTICK 0x01 0x7B
3DO_SUBTYPE_KEYBOARD    0x02/0x4B
3DO_SUBTYPE_MOUSE       0x49
3DO_SUBTYPE_GLASSES     0x41
3DO_SUBTYPE_LIGHTGUN    0x4D
3DO_SUBTYPE_SPLITTER    0x56
3DO_SUBTYPE_SPLITTER2   0x57
3DO_SUBTYPE_IRRECEIVER  0x03
3DO_SUBTYPE_ORBATAK     0xCD

### Implementation


## PS/PS2

### Principle

手柄采用3.3V供电，三个输入端ATT、CLK和CMD，两个输出端DATA和ACK。

### Pseudocode

#### 针脚定义

| Define      | Pin | Mode         | DE15 Pin | Color     | Note |
|:-----------:|:---:|:------------:|:--------:|:---------:|:----:|
| PS_PIN_DATA | 0   | INPUT_PULLUP | 1        | (1)BROWN  |      |
| PS_PIN_CMD  | 5   | OUTPUT       | 11       | (2)ORANGE |      |
| MOTOR       | -   | -            | 4        | (3)GREY   | 9V   |
| GND         | -   | -            | 14       | (4)BLACK  |      |
| 3.3V        | -   | -            | 4        | (5)RED    |      |
| PS_PIN_ATT  | 2   | OUTPUT       | 6        | (6)YELLOW |      |
| PS_PIN_CLK  | 6   | OUTPUT       | 12       | (7)BLUE   |      |
| PS_PIN_ACK  | 1   | INPUT_PULLUP | 2        | (9)GREEN  |      |

#### 手柄类型定义

    #define PS_TYPE_DIGITAL 0x41 // Normal digital
    #define PS_TYPE_RED     0x73 // Red analog
    #define PS_TYPE_GREEN   0x53 // Green analog

#### 按键定义

    #define PS_NONE   GAMEPAD_NONE
    #define PS_UP     GAMEPAD_UP
    #define PS_DOWN   GAMEPAD_DOWN
    #define PS_LEFT   GAMEPAD_LEFT
    #define PS_RIGHT  GAMEPAD_RIGHT
    #define PS_A      GAMEPAD_A        // A=O=Cir
    #define PS_B      GAMEPAD_B        // B=X=Cross
    #define PS_X      GAMEPAD_X        // X=A=Tri
    #define PS_Y      GAMEPAD_Y        // Y= =Rect
    #define PS_SELECT GAMEPAD_SELECT
    #define PS_START  GAMEPAD_START
    #define PS_L1     GAMEPAD_L1
    #define PS_R1     GAMEPAD_R1
    #define PS_L2T    GAMEPAD_L2
    #define PS_R2T    GAMEPAD_R2
    #define PS_LJT    GAMEPAD_LT
    #define PS_RJT    GAMEPAD_RT
    #define PS_LJLR   GAMEPAD_AXIS_X
    #define PS_LJUD   GAMEPAD_AXIS_Y
    #define PS_RJLR   GAMEPAD_AXIS_RX
    #define PS_RJUD   GAMEPAD_AXIS_RY
    #define PS_L2     GAMEPAD_AXIS_Z
    #define PS_R2     GAMEPAD_AXIS_Z

#### 初始化

    digitalWrite(pin_number[PS_PIN_CMD], HIGH);
    digitalWrite(pin_number[PS_PIN_CLK], HIGH);
    digitalWrite(pin_number[PS_PIN_ATT], HIGH);
    delayMicroseconds(50);

### 总线读写

    ret = 0;
    for(i=0; i<8; i++){
      digitalWrite(PS_PIN_CLK, LOW);
      delayMicroseconds(2);
      if(bitRead(cmd, i)){
        digitalWrite(PS_PIN_CMD, HIGH);
      }else{
        digitalWrite(PS_PIN_CMD, LOW);
      }
      delayMicroseconds(18);
      digitalWrite(PS_PIN_CLK, HIGH);
      delayMicroseconds(18);
      if(digitalRead(PS_PIN_DATA)){
        bitSet(ret, i);
      }
      delayMicroseconds(2);
    }
    delayMicroseconds(50);  // waiting and pass ACK
    return ret;

#### 获取数据

    digitalWrite(pin_number[PS_PIN_CMD], HIGH);
    digitalWrite(pin_number[PS_PIN_CLK], HIGH);
    digitalWrite(pin_number[PS_PIN_ATT], LOW );
    delayMicroseconds(50);

    exchangeData(0x01);
    SubType = exchangeData(0x42);
    switch(SubType){
    case PS_TYPE_DIGITAL:  // normal digital = 0x41
      if(exchangeData(0x00) == 0x5A){
        uint8_t keymap41[2][8]={
          {PS_SELECT, PS_NONE, PS_NONE, PS_START, PS_UP, PS_RIGHT, PS_DOWN, PS_LEFT},
          {PS_L2T,    PS_R2T,  PS_L1,   PS_R1,    PS_X,  PS_A,     PS_B,    PS_Y   }
        };

        for(i=0; i<(2); i++){
          ret = exchangeData(0xFF);
          for(j=0; j<8; j++){
            keys->buttons_value[keymap41[i][j]] = 1-bitRead(ret, j);
          }
        }
      }
      break;
    case PS_TYPE_RED:  // analog RED = 0x73
      if(exchangeData(0x00) == 0x5A){
        uint8_t keymap73[2][8]={
          {PS_SELECT, PS_RJ,   PS_LJ,   PS_START, PS_UP, PS_RIGHT, PS_DOWN, PS_LEFT},
          {PS_L2T,    PS_R2T,  PS_L1,   PS_R1,    PS_X,  PS_A,     PS_B,    PS_Y   }
        };

        for(i=0; i<(2); i++){
          ret = exchangeData(0xFF);
          for(j=0; j<8; j++){
            keys->buttons_value[keymap73[i][j]] = 1-bitRead(ret, j);
          }
        }
        ret = exchangeData(0xFF); // analog RJ.LR
        keys->axis_value[GAMEPAD_AXIS_X]=ret<<8;
        ret = exchangeData(0xFF); // analog RJ.UD
        keys->axis_value[GAMEPAD_AXIS_Y]=ret<<8;
        ret = exchangeData(0xFF); // analog LJ.LR
        keys->axis_value[GAMEPAD_AXIS_RX]=ret<<8;
        ret = exchangeData(0xFF); // analog LJ.UD
        keys->axis_value[GAMEPAD_AXIS_RY]=ret<<8;
      }
      break;
    case PS_TYPE_GREEN:  // analog GREEN = 0x53
      if(exchangeData(0x00) == 0x5A){
        uint8_t keymap53[2][8]={
          {PS_SELECT, PS_RJ,   PS_LJ,   PS_START, PS_UP, PS_RIGHT, PS_DOWN, PS_LEFT},
          {PS_L2T,    PS_R2T,  PS_L1,   PS_R1,    PS_X,  PS_A,     PS_B,    PS_Y   }
        };

        for(i=0; i<(2); i++){
          ret = exchangeData(0xFF);
          for(j=0; j<8; j++){
            keys->buttons_value[keymap53[i][j]] = 1-bitRead(ret, j);
          }
        }
        ret = exchangeData(0xFF); // analog RJ.LR
        keys->axis_value[GAMEPAD_AXIS_X]=ret<<8;
        ret = exchangeData(0xFF); // analog RJ.UD
        keys->axis_value[GAMEPAD_AXIS_Y]=ret<<8;
        ret = exchangeData(0xFF); // analog LJ.LR
        keys->axis_value[GAMEPAD_AXIS_RX]=ret<<8;
        ret = exchangeData(0xFF); // analog LJ.UD
        keys->axis_value[GAMEPAD_AXIS_RY]=ret<<8;
      }
      break;
    }
    digitalWrite(pin_number[PS_PIN_ATT], HIGH);

### Implementation


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

### Pseudocode

| Define       | Pin | Mode         | DE15 Pin | Color    | Note |
|:------------:|:---:|:------------:|:--------:|:--------:|:----:|
| DC_PIN_D1    | 0   | ???          | 1        | (1)RED   |      |
| 5V           | -   | -            | 9        | (2)BLUE  |      |
| GND          | -   | -            | 14       | (3)WHITE |      |
| DC_PIN_SENSE | 2   | INPUT_PULLUP | 6        | (4)GREEN |      |
| DC_PIN_D5    | 1   | ???          | 2        | (5)WHITE |      |


### Implementation

Maple Bus的实现需参考 [libmaple](https://github.com/Xerxes3rd/libmaple), [maple-bus-arduino](https://github.com/kilgariff/maple-bus-arduino), [arduino-maple](https://github.com/requeijaum/arduino-maple)


## WII

### Principle

The Wii controllers use a standard I2C interface, and there's existing code for both Arduino and CircuitPython/Python for quick integration with an Arduino UNO, Feather, or even a Raspberry Pi. We like to use these with the Wii Nunchuck, as you can get an X-Y joystick, two buttons and an accelerometer all in one hand-held package. All data is transmitted over I2C address 0x52, and the address can not be changed.

#### Pinout

    +-------+
    | 1 2 3 |
    | 4 5 6 |
    +--^^^--+

 Y B G
 R   W

SDA --- 3v3
GND --- SCL

For the original Nunchuk, the following is the color and pin-layout of the connector: 

Green → SDA    nc    Red → 3v3 
White → GND    nc    Yellow → SCL 


### Pseudocode

### Implementation

WII参考: [Nunchuk](https://github.com/infusion/Fritzing/tree/master/Nunchuk)



# GEN-4 通用型

# GEN-4.2 USB


## NINTENDO SWITCH

### Principle

Nintendo Switch Pro Congroller

    axis X/Y(0/1)    axisL    // pygame UL=-1/DR=1 /HID 
    axis RX/RY(2/3)  axisR    // pygame/HID
    axis Z(4)        ZL -1/1  // pygame
    axis RZ(5)       ZR -1/1  // pygame

    B0 B1 B2 B3 B4 B5   B6 B7
    A  B  X  Y  -  Home +  LJ  // pygame
    B  A  Y  X  L  R    ZL ZR  // HID

    B8 B9 B10 B11 B12  B13  B14 B15
    RJ L  R   U   D    L    R   Capt   // pygame
    -  +  LJ  RJ  Home Capt ?   ?      // HID

    B16 B17 B18 B19
    ?   ?   ?   ?     // pygame

    CAP1: UDLR //HID

#### Pinout

BT

### Pseudocode

#### 按键定义

    SWITCH_UP      GAMEPAD_UP
    SWITCH_DOWN    GAMEPAD_DOWN
    SWITCH_LEFT    GAMEPAD_LEFT
    SWITCH_RIGHT   GAMEPAD_RIGHT
    SWITCH_A       GAMEPAD_A
    SWITCH_B       GAMEPAD_B
    SWITCH_X       GAMEPAD_X
    SWITCH_Y       GAMEPAD_Y
    SWITCH_L       GAMEPAD_L1
    SWITCH_R       GAMEPAD_R1
    SWITCH_MIUS    GAMEPAD_SELECT
    SWITCH_PLUS    GAMEPAD_START
    SWITCH_ZL      GAMEPAD_L2
    SWITCH_ZR      GAMEPAD_R2
    SWITCH_LJ      GAMEPAD_LJ
    SWITCH_RJ      GAMEPAD_RJ
    SWITCH_HOME    GAMEPAD_HOME
    SWITCH_CAPTURE GAMEPAD_CAPTURE

    SWITCH_AXIS_X  GAMEPAD_AXIS_X
    SWITCH_AXIS_Y  GAMEPAD_AXIS_Y
    SWITCH_AXIS_RX GAMEPAD_AXIS_RX
    SWITCH_AXIS_RY GAMEPAD_AXIS_RY
    SWITCH_AXIS_Z  GAMEPAD_AXIS_Z
    SWITCH_AXIS_RZ GAMEPAD_AXIS_RZ


### Implementation


