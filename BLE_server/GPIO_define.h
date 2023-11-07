#include "esp32-hal-gpio.h"
#pragma once

// spi
#define CUSTOM_SPI_SPEED 5000
#define SLAVE_SELECT_PIN GPIO_NUM_15

// 测试灯
#define TEST_LED LED_BUILTIN

// 动力部分
// #define GPIO_DYNAMICAL_MOTO_0 GPIO_NUM_13
// #define GPIO_DYNAMICAL_MOTO_1 GPIO_NUM_21
// #define MOTO_CHANNEL_PWM_0 1  // 使用1号通道
// #define MOTO_CHANNEL_PWM_1 2  // 使用2号通道
// #define MOTO_FREQ_PWM 50      // 舵机频率，那么周期也就是1/50，也就是20ms ，PWM⼀共有16个通道，0-7位⾼速通道由80Mhz时钟驱动，后⾯8个为低速通道由1Mhz
// #define MOTO_RES_PWM 10       // 分辨率  0-1024  共1025

void GPIO_define_init() {
  // spi
  // pinMode(SLAVE_SELECT_PIN, OUTPUT);

  // 测试灯
  pinMode(TEST_LED, OUTPUT);

  // // 动力部分
  // ledcSetup(MOTO_CHANNEL_PWM_0, MOTO_FREQ_PWM, MOTO_RES_PWM);  // 设置通道 0
  // ledcSetup(MOTO_CHANNEL_PWM_1, MOTO_FREQ_PWM, MOTO_RES_PWM);  // 设置通道 2
  // ledcAttachPin(GPIO_DYNAMICAL_MOTO_0, MOTO_CHANNEL_PWM_0);    // 将引脚绑定到通道上 0
  // ledcAttachPin(GPIO_DYNAMICAL_MOTO_1, MOTO_CHANNEL_PWM_1);    // 将引脚绑定到通道上 1
}
