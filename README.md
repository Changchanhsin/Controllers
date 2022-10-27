# Controllers

A arduino based Joypad hardware adaptor.

基于Arduino Leonardo的游戏手柄适配器

# Functional
将游戏机手柄通过该适配器用到PC上。

已测试SFC、MD（3键、6键）、SS。

# Design

（图见PPT文件）

## Port/Interface

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

## Keymap

当采用键盘模式时，一个Leonardo可以同时连接两个手柄；当采用游戏手柄模式时，一个Leonardo仅可以连接第一个手柄

# Reference

## Atari 2600
### Principle
Atari 2600有一个摇杆和一个按钮，使用9针母口，无芯片，仅5个开关。

### Pseudocode

### Implementation
公共端接地，五个信号端接5个数据区针，数据区针初始化为INPUT_PULLUP，无操作时为高电平，按下时为低电平。

## FC/NES、SFC/SNES
### Principle

手柄采用5V供电，两个输入端LATCH和CLK、一个输出端DATA。

- LATCH常低，通过一个12微秒的高电平激活手柄输入。
- LATCH高电平后等待6000微秒，连续发送CLK高电平脉冲以获得数据，每个脉冲宽度为12微秒高12微秒低，共16组。
- DATA信号常高，CLK信号开始后下降沿读数据，数据顺序为按键B、Y、Select、Start、Up、Down、Left、Right、A、X、L、R。

### Pseudocode


### Implementation

任天堂FC和SFC接口虽然形式不同，但信号处理相同，按一种手柄处理。

## MD
### Principle
手柄采用5V供电，一个输入TH，6个输出TR、TL、D3、D2、D1、D0。

TH通过输入高低电平以获得数据，2组高低电平获得3键手柄的信息，4组获得6键手柄的信息

### Pseudocode

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

手柄采用5V供电，类似MD

### Pseudocode

### Implementation
