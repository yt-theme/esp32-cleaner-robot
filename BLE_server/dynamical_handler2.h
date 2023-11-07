#pragma once

#include <Arduino.h>
#include <atomic>
#include "BLE_cmd.h"
#include "GPIO_define.h"

namespace DynamicalHandler2 {

static std::atomic<unsigned long> currentTimeForStop(0);
static std::atomic<unsigned long> currentTime(0);
static std::atomic_long previousTime(0);

/*
  动力信号监听计时结束
*/
static void dynListenTimeForStop() {
  currentTimeForStop = millis();
  if ((currentTimeForStop - previousTime) > BLE_SIGNAL_INTERVAL) {
    previousTime = currentTimeForStop;
    // 关闭动力
    ledcWrite(MOTO_CHANNEL_PWM_0, 77);
    digitalWrite(LED_BUILTIN, LOW);
    ledcWrite(MOTO_CHANNEL_PWM_1, 77);
    Serial.println("关闭动力 =>");
  }
}

/*
  动力指令处理
*/
static void dynamicalHandler(const std::string& cmdVal) {
  currentTime = millis();
  long subTime = (currentTime - previousTime);
  // 延时动力
  previousTime = BLE_SIGNAL_INTERVAL - subTime;

  if (cmdVal == BLE_CMD_FORWARD) {
    ledcWrite(MOTO_CHANNEL_PWM_0, 52);  //20ms高电平为1ms左右 ，也就是1/20*1024 = 52，此时360度舵机正传5秒
    ledcWrite(MOTO_CHANNEL_PWM_1, 52);
    digitalWrite(LED_BUILTIN, HIGH);

  } else if (cmdVal == BLE_CMD_BACKWARD) {
    ledcWrite(MOTO_CHANNEL_PWM_0, 102);
    ledcWrite(MOTO_CHANNEL_PWM_1, 102);
    digitalWrite(LED_BUILTIN, HIGH);

  } else if (cmdVal == BLE_CMD_TRUNLEFT) {
    ledcWrite(MOTO_CHANNEL_PWM_0, 52);
    ledcWrite(MOTO_CHANNEL_PWM_1, 102);
    digitalWrite(LED_BUILTIN, HIGH);

  } else if (cmdVal == BLE_CMD_TRUNRIGHT) {
    ledcWrite(MOTO_CHANNEL_PWM_0, 102);
    ledcWrite(MOTO_CHANNEL_PWM_1, 52);
    digitalWrite(LED_BUILTIN, HIGH);

  } else if (cmdVal == BLE_CMD_GEAR0) {
    Serial.println("BLE_CMD_GEAR0 =>");
  } else if (cmdVal == BLE_CMD_GEAR1) {
    Serial.println("BLE_CMD_GEAR1 =>");
  } else if (cmdVal == BLE_CMD_GEAR2) {
    Serial.println("BLE_CMD_GEAR2 =>");
  }
}
}
