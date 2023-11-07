#pragma once

// 舵机
#define SERVO_PIN_0 7
#define SERVO_PIN_1 6

// 信号间隔ms
#define BLE_SIGNAL_INTERVAL 100

//动力部分
#define BLE_CMD_FORWARD "direct.01"
#define BLE_CMD_BACKWARD "direct.02"
#define BLE_CMD_TRUNLEFT "direct.03"
#define BLE_CMD_TRUNRIGHT "direct.04"
#define BLE_CMD_GEAR0 "gear.00"
#define BLE_CMD_GEAR1 "gear.01"
#define BLE_CMD_GEAR2 "gear.02"

// 判断指令是否相符
bool compareCmd(const char * target, const char * origin)
{
  return String(target) == String(origin);
}
