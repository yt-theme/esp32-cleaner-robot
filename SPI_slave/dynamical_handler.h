#pragma once

#include <Arduino.h>
#include <Servo.h>
#include "common.h"

namespace DynamicalHandler {

static Servo myservo_0;
static Servo myservo_1;
static int pos = 0;

static unsigned long currentTimeForStop(0);
static unsigned long currentTime(0);
static long previousTime(0);

static init() {
  myservo_0.attach(SERVO_PIN_0);
  myservo_1.attach(SERVO_PIN_1);
}

/*
  动力信号监听计时结束
*/
static void dynListenTimeForStop() {
  currentTimeForStop = millis();
  if ((currentTimeForStop - previousTime) > BLE_SIGNAL_INTERVAL) {
    previousTime = currentTimeForStop;
    // 关闭动力
    myservo_0.write(90);
    myservo_1.write(90);
  }
}

/*
  统一舵机动力处理
*/
static void pubServoAction(int servoVal_0, int servoVal_1) {
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo_0.write(servoVal_0);              // tell servo to go to position in variable 'pos'
    myservo_1.write(servoVal_1);
    delay(1);                       // waits 15 ms for the servo to reach the position
  }
}

/*
  动力指令处理
*/
static void dynamicalHandler(const char * cmdVal) {
  currentTime = millis();
  long subTime = (currentTime - previousTime);
  // 延时动力
  previousTime = BLE_SIGNAL_INTERVAL - subTime;

  //  Serial.println("cmdVal s =>");
  //  Serial.println(strlen(cmdVal));
  //  Serial.println(cmdVal);
  //  Serial.println(BLE_CMD_FORWARD);
  //  Serial.println(strlen(BLE_CMD_FORWARD));
  //  Serial.println("cmdVal d =>");

  if (compareCmd(cmdVal, BLE_CMD_FORWARD)) {
    Serial.println("BLE_CMD_FORWARD =>");
    pubServoAction(45, 45);

  } else if (compareCmd(cmdVal, BLE_CMD_BACKWARD)) {
    Serial.println("BLE_CMD_BACKWARD =>");
    pubServoAction(135, 135);

  } else if (compareCmd(cmdVal, BLE_CMD_TRUNLEFT)) {
    Serial.println("BLE_CMD_TRUNLEFT =>");
    pubServoAction(135, 45);

  } else if (compareCmd(cmdVal, BLE_CMD_TRUNRIGHT)) {
    Serial.println("BLE_CMD_TRUNRIGHT =>");
    pubServoAction(45, 135);

  } else if (cmdVal == BLE_CMD_GEAR0) {
    Serial.println("BLE_CMD_GEAR0 =>");
  } else if (cmdVal == BLE_CMD_GEAR1) {
    Serial.println("BLE_CMD_GEAR1 =>");
  } else if (cmdVal == BLE_CMD_GEAR2) {
    Serial.println("BLE_CMD_GEAR2 =>");
  }
}
}
