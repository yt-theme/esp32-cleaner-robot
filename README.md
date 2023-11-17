esp32-devkit-v1 coded by arduino ide 1.8.57.0
### 主要硬件
1. esp32-devkit-v1 及其扩展板 1个
2. arduino-nano 及其扩展板 1个
3. l298n 红板 1个
4. 12v电源板 1个
6. 坦克底盘一套(带2个tt电机)

### 接线
* 所有板共地
* spi通信, esp32作为master与arduino-nano作为slave:
    1. esp32的D15与arduino-nano的10连接.
    2. esp32的D13与arduino-nano的11连接.
    3. esp32的D12与arduino-nano的12连接.
    4. esp32的D14与arduino-nano的13连接.

* l298n作为电机驱动模块与arduino-nano连接:
    1. l298n的ENA与arduino-nano的5连接.
    2. l298n的ENB与arduino-nano的6连接.
    3. l298n的IN1与arduino-nano的A1连接.
    4. l298n的IN2与arduino-nano的A2连接.
    5. l298n的IN3与arduino-nano的A3连接.
    6. l298n的IN4与arduino-nano的A4连接.

* l298n与tt电机连接:
    1. l298n的OUT1、OUT2与电机A连接.
    2. l298n的OUT3、OUT4与电机B连接.

* 电源连接:
    1. l298n的+12v与12v电源+连接.
    2. l298n的+5v给所有单片机供电.
    3. 所有GND共地.
