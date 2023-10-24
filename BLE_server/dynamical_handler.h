#pragma once

#include <Arduino.h>
#include "BLE_cmd.h"
#include "GPIO_define.h"

void timerCloseDyn();

/*
  计时器设置
*/
hw_timer_t* timerDyn = NULL;
bool timerDynIsInitted = false;

/*
  计时器回调
*/
void onTimerInterrDyn()
{
  Serial.println("计时器回调 =>");
  if (timerDyn)
  {
    // timerAlarmDisable(timerDyn);
    timerCloseDyn();
  }
  // 关闭动力
  ledcWrite(MOTO_CHANNEL_PWM_0, 77);
  ledcWrite(MOTO_CHANNEL_PWM_1, 77);
  Serial.println("关闭动力 =>");
}

/*
  计时器初始化方法
*/
void timerInitDyn()
{
  if (timerDynIsInitted)
  {
    return;
  }

  timerDyn = timerBegin(1, 80, true);
  timerAttachInterrupt(timerDyn, &onTimerInterrDyn, true);
  timerAlarmWrite(timerDyn, 200000, true);        //timerBegin的参数二 80位80MHZ，这里为1000000  意思为1秒

  timerDynIsInitted = true;
}
/*
  计时器关闭方法
*/
void timerCloseDyn()
{
  if (timerDyn)
  {
    timerAlarmDisable(timerDyn);
    timerDetachInterrupt(timerDyn);
    timerEnd(timerDyn);
    timerDynIsInitted = false;
    timerDyn = NULL;
  }
}

/*
  动力指令处理
*/
void dynamicalHandler(std::string cmdVal)
{

  if (cmdVal == BLE_CMD_FORWARD)
  {
    Serial.println("BLE_CMD_FORWARD =>");
    // 关闭计时
    timerCloseDyn();
    ledcWrite(MOTO_CHANNEL_PWM_0, 52); //20ms高电平为1ms左右 ，也就是1/20*1024 = 52，此时360度舵机正传5秒
    ledcWrite(MOTO_CHANNEL_PWM_1, 52);
    // 进行计时
    timerInitDyn();
    timerAlarmEnable(timerDyn);
  }
  else if (cmdVal == BLE_CMD_BACKWARD)
  {
    Serial.println("BLE_CMD_BACKWARD =>");
    // 关闭计时
    timerCloseDyn();
    ledcWrite(MOTO_CHANNEL_PWM_0, 102);
    ledcWrite(MOTO_CHANNEL_PWM_1, 102);
    // 进行计时
    timerInitDyn();
    timerAlarmEnable(timerDyn);
  }
  else if (cmdVal == BLE_CMD_TRUNLEFT)
  {
    Serial.println("BLE_CMD_TRUNLEFT =>");
    // 关闭计时
    timerCloseDyn();
    // 进行计时
    timerInitDyn();
    timerAlarmEnable(timerDyn);
  }
  else if (cmdVal == BLE_CMD_TRUNRIGHT)
  {
    Serial.println("BLE_CMD_TRUNRIGHT =>");
    // 关闭计时
    timerCloseDyn();
    // 进行计时
    timerInitDyn();
    timerAlarmEnable(timerDyn);
  }
  else if (cmdVal == BLE_CMD_GEAR0)
  {
    Serial.println("BLE_CMD_GEAR0 =>");
  }
  else if (cmdVal == BLE_CMD_GEAR1)
  {
    Serial.println("BLE_CMD_GEAR1 =>");
  }
  else if (cmdVal == BLE_CMD_GEAR2)
  {
    Serial.println("BLE_CMD_GEAR2 =>");

  }
}
