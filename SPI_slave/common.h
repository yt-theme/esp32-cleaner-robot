#pragma once

// 舵机
#define SERVO_PIN_0 9
#define SERVO_PIN_1 8

// TT电机
/*
使用L298N模块驱动

IN1 & IN2 电机驱动器A的输入引脚，控制电机A转动及旋转角度
IN1输入高电平HIGH，IN2输入低电平LOW，对应电机A正转
IN1输入低电平LOW，IN2输入高电平HIGH，对应电机A反转
IN1、IN2同时输入高电平HIGH或低电平LOW，对应电机A停止转动
调速就是改变IN1、IN2高电平的占空比（需拔掉ENA处跳帽）

IN3 & IN4 电机驱动器B的输入引脚，控制电机B转动及旋转角度
IN3输入高电平HIGH，IN4输入低电平LOW，对应电机B正转
IN3输入低电平LOW，IN4输入高电平HIGH，对应电机B反转
IN3、IN4同时输入高电平HIGH或低电平LOW，对应电机B停止转动
调速就是改变IN3、IN4高电平的占空比（需拔掉ENB处跳帽）
*/
#define TT_A_PIN_ENA 14
#define TT_A_PIN_IN1 15 
#define TT_A_PIN_IN2 16
#define TT_B_PIN_IN3 17
#define TT_B_PIN_IN4 18
#define TT_B_PIN_ENB 19
#define TT_PWM_SPEED_0 200
#define TT_PWM_SPEED_1 300
#define TT_PWM_SPEED_2 400

// 信号间隔ms
#define BLE_SIGNAL_INTERVAL 92

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
