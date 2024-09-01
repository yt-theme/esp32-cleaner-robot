esp32-devkit-v1 coded by arduino ide 1.8.57.0

### 项目目录介绍
1. /BLE_client_uniapp 手机app, 蓝牙遥控端, uniapp编写, 已对android优化
2. /BLE_server 用于到esp32板子上的程序
3. /SPI_slave 用于arduino-nano板子的程序(与esp32经过SPI通信)
4. /doc 相关参考资料选购件接线图等

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

* 电流电压传感器
    1. 电流传感器(ACS712)与esp32的34连接.
    2. 电压传感器(Voltage Sensor)与esp32的35连接.
    3. 相关其它引脚按标注连接.

* oled屏(0.96吋, i2c)
    1. 屏SDA与esp32的21连接.
    2. 屏SCL与esp32的22连接.
    3. 相关其它引脚按标注连接.