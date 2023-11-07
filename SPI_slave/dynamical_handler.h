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
    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo_0.write(45);              // tell servo to go to position in variable 'pos'
      myservo_1.write(1);
      delay(1);                       // waits 15 ms for the servo to reach the position
    }

  } else if (compareCmd(cmdVal, BLE_CMD_BACKWARD)) {
    Serial.println("BLE_CMD_BACKWARD =>");
    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo_0.write(135);              // tell servo to go to position in variable 'pos'
      myservo_1.write(135);
      delay(1);                       // waits 15 ms for the servo to reach the position
    }

  } else if (compareCmd(cmdVal, BLE_CMD_TRUNLEFT)) {
    Serial.println("BLE_CMD_TRUNLEFT =>");
    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo_0.write(135);              // tell servo to go to position in variable 'pos'
      myservo_1.write(45);
      delay(1);                       // waits 15 ms for the servo to reach the position
    }

  } else if (compareCmd(cmdVal, BLE_CMD_TRUNRIGHT)) {
    Serial.println("BLE_CMD_TRUNRIGHT =>");
    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo_0.write(45);              // tell servo to go to position in variable 'pos'
      myservo_1.write(135);
      delay(1);                       // waits 15 ms for the servo to reach the position
    }

  } else if (cmdVal == BLE_CMD_GEAR0) {
    Serial.println("BLE_CMD_GEAR0 =>");
  } else if (cmdVal == BLE_CMD_GEAR1) {
    Serial.println("BLE_CMD_GEAR1 =>");
  } else if (cmdVal == BLE_CMD_GEAR2) {
    Serial.println("BLE_CMD_GEAR2 =>");
  }
}
}
