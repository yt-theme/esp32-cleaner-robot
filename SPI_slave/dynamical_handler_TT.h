#pragma once

#include <Arduino.h>
#include <Servo.h>
#include "common.h"

namespace DynamicalHandlerTT {
void dynCtrl(int IN1, int IN2, int IN3, int IN4, int PWMSpeed);
void timeRec();

static unsigned long currentTimeForStop(0);
static unsigned long currentTime(0);
static long previousTime(0);

// pwm值用于速度
static int pwmSpeed = TT_PWM_SPEED_0;
static bool dynIsClosed = true;

static init() {
  pinMode(TT_A_PIN_IN1, OUTPUT);
  pinMode(TT_A_PIN_IN2, OUTPUT);
  pinMode(TT_B_PIN_IN3, OUTPUT);
  pinMode(TT_B_PIN_IN4, OUTPUT);
  pinMode(TT_A_PIN_ENA, OUTPUT);
  pinMode(TT_B_PIN_ENB, OUTPUT);
}

/*
  动力信号监听计时结束
*/
static void dynListenTimeForStop() {
  currentTimeForStop = millis();
  if ((currentTimeForStop - previousTime) > BLE_SIGNAL_INTERVAL) {
    previousTime = currentTimeForStop;
    if (!dynIsClosed)
    {
      // 关闭动力
      dynCtrl(LOW, LOW, LOW, LOW, 0);
      dynIsClosed = true;
      Serial.println("close dyn =>");
    }
  }
}

void timeRec()
{
  currentTime = millis();
//  long subTime = (currentTime - previousTime);
//  // 延时动力
//  previousTime = BLE_SIGNAL_INTERVAL - subTime;

  previousTime = currentTime;
}

void dynCtrl(int IN1, int IN2, int IN3, int IN4, int PWMSpeed)
{
  digitalWrite(TT_A_PIN_IN1, IN1);
  digitalWrite(TT_A_PIN_IN2, IN2);
  digitalWrite(TT_B_PIN_IN3, IN3);
  digitalWrite(TT_B_PIN_IN4, IN4);
  if (dynIsClosed == true) {
    for (int i=0; i<PWMSpeed; i+=10) {
      analogWrite(TT_A_PIN_ENA, i);
      analogWrite(TT_B_PIN_ENB, i);
      delay(1);
    }
  } else {
    analogWrite(TT_A_PIN_ENA, PWMSpeed);
    analogWrite(TT_B_PIN_ENB, PWMSpeed);
  }
  delay(1);
}

/*
  动力指令处理
*/
static void dynamicalHandler(const char* cmdVal) {
  if (compareCmd(cmdVal, BLE_CMD_FORWARD)) {
    Serial.println("BLE_CMD_FORWARD =>");
    timeRec();
    dynCtrl(HIGH, LOW, HIGH, LOW, pwmSpeed);
    dynIsClosed = false;

  } else if (compareCmd(cmdVal, BLE_CMD_BACKWARD)) {
    Serial.println("BLE_CMD_BACKWARD =>");
    timeRec();
    dynCtrl(LOW, HIGH, LOW, HIGH, pwmSpeed);
    dynIsClosed = false;
    
  } else if (compareCmd(cmdVal, BLE_CMD_TRUNLEFT)) {
    Serial.println("BLE_CMD_TRUNLEFT =>");
    timeRec();
    dynCtrl(LOW, HIGH, HIGH, LOW, pwmSpeed);
    dynIsClosed = false;

  } else if (compareCmd(cmdVal, BLE_CMD_TRUNRIGHT)) {
    Serial.println("BLE_CMD_TRUNRIGHT =>");
    timeRec();
    dynCtrl(HIGH, LOW, LOW, HIGH, pwmSpeed);
    dynIsClosed = false;

  } else if (compareCmd(cmdVal, BLE_CMD_GEAR0)) {
    Serial.println("BLE_CMD_GEAR0 =>");
    pwmSpeed = TT_PWM_SPEED_0;
  } else if (compareCmd(cmdVal, BLE_CMD_GEAR1)) {
    Serial.println("BLE_CMD_GEAR1 =>");
    pwmSpeed = TT_PWM_SPEED_1;
  } else if (compareCmd(cmdVal, BLE_CMD_GEAR2)) {
    Serial.println("BLE_CMD_GEAR2 =>");
    pwmSpeed = TT_PWM_SPEED_2;
  } else {
    String cmdGearCustomVal = compareCmdHeader(cmdVal, BLE_CMD_GEAR_CUSTOM);
    if (cmdGearCustomVal != ""){
      pwmSpeed = cmdGearCustomVal.toInt();
    }
  }
}
}
