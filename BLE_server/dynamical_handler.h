#pragma once

#include <Arduino.h>
#include <atomic>
#include "BLE_cmd.h"
#include "GPIO_define.h"

namespace DynamicalHandler {


void onTimerInterrDyn();
void timerCloseDyn();

long previousTime=0;

/*
  计时器设置
*/
std::atomic<hw_timer_t*> timerDyn(NULL);
std::atomic_bool timerDynIsInitted(false);

/*
  计时器初始化方法
*/
void timerInitDyn() {
  if (timerDynIsInitted) {
    return;
  }

  timerDyn = timerBegin(1, 80, true);
  timerAttachInterrupt(timerDyn, &onTimerInterrDyn, true);
  timerAlarmWrite(timerDyn, 100000, true);  //timerBegin的参数二 80位80MHZ，这里为1000000  意思为1秒

  timerDynIsInitted = true;
}
/*
  计时器关闭方法
*/
void timerCloseDyn() {
  if (!timerDynIsInitted) {
    return;
  }
  if (!timerDyn) {
    return;
  }
  timerAlarmDisable(timerDyn);
  timerDetachInterrupt(timerDyn);
  timerEnd(timerDyn);
  timerDyn = NULL;
  timerDynIsInitted = false;
  Serial.println("关闭计时器 =>");
}

/*
  计时器回调
*/
void onTimerInterrDyn() {
  Serial.println("计时器回调 =>");

  // 关闭动力
  ledcWrite(MOTO_CHANNEL_PWM_0, 77);
  ledcWrite(MOTO_CHANNEL_PWM_1, 77);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("关闭动力 =>");
  timerCloseDyn();
}

/*
  计时连续方法
*/
void timerContinuity() {
  // 关闭计时
  timerCloseDyn();
  // 进行计时
  timerInitDyn();
  timerAlarmEnable(timerDyn);
}

/*
  动力指令处理
*/
void dynamicalHandler(const std::string& cmdVal) {

  unsigned long currentTime = millis();
  if ((currentTime - previousTime) > 20) {
    previousTime = currentTime;
  }

  if (cmdVal == BLE_CMD_FORWARD) {
    Serial.println("BLE_CMD_FORWARD =>");
    // 计时连续
    timerContinuity();

    ledcWrite(MOTO_CHANNEL_PWM_0, 52);  //20ms高电平为1ms左右 ，也就是1/20*1024 = 52，此时360度舵机正传5秒
   
    ledcWrite(MOTO_CHANNEL_PWM_1, 52);

    digitalWrite(LED_BUILTIN, HIGH);

  } else if (cmdVal == BLE_CMD_BACKWARD) {
    Serial.println("BLE_CMD_BACKWARD =>");
    // 计时连续
    timerContinuity();
    ledcWrite(MOTO_CHANNEL_PWM_0, 132);
    ledcWrite(MOTO_CHANNEL_PWM_1, 132);
    digitalWrite(LED_BUILTIN, HIGH);

  } else if (cmdVal == BLE_CMD_TRUNLEFT) {
    Serial.println("BLE_CMD_TRUNLEFT =>");
    // 计时连续
    timerContinuity();

    ledcWrite(MOTO_CHANNEL_PWM_0, 52);
    ledcWrite(MOTO_CHANNEL_PWM_1, 102);
    digitalWrite(LED_BUILTIN, HIGH);

  } else if (cmdVal == BLE_CMD_TRUNRIGHT) {
    Serial.println("BLE_CMD_TRUNRIGHT =>");
    // 计时连续
    timerContinuity();

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
