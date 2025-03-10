/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by yummycloud
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "GPIO_define.h"
#include "BLE_callbacks.h"
// #include "dynamical_handler2.h"
#include "spi_handler.h"
#include "I2C_oled.h"
#include "sensor_handler.h"
// #include <string>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define LOOP_CIRCLE_MAX 65535

BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pCharacteristic = NULL;

bool deviceConnected = false;     //本次连接状态
bool oldDeviceConnected = false;  //上次连接状态d

BleServerCallbacks *bleServerCallbacks = new BleServerCallbacks(&deviceConnected);
ReceiveCallbacks *receiveCallbacks = new ReceiveCallbacks();

long loopCircleCounter = 0;
// 蓝牙通知字符串数据内容
String BLENotifyValueStr = "";
// 传感器电流值
String sensorUPSCurrentStr = "";
// 传感器电压值
String sensorUPSVoltageStr = "";

void setup() {
  // serial
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // gpio
  GPIO_define_init();

  // ble
  BLEDevice::init("esp32");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(bleServerCallbacks);  //设置回调
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE);
  pCharacteristic->setCallbacks(receiveCallbacks);  //设置回调

  pCharacteristic->setValue("WAIT");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  // spi init
  SpiHandler::init();

  // i2c oled
  I2COled::init();

  // sensor
  SensorHandler::currentCalibrate(); // 电流传感器零点校准
}

void loop() {

  // 记录loop圈数用于模拟delay
  if (loopCircleCounter >= LOOP_CIRCLE_MAX) {
    loopCircleCounter = 0;
  }
  loopCircleCounter++;

  //测试spi
  // SpiHandler::testSend();

  ///
  // 动力信号监听计时结束
  // DynamicalHandler2::dynListenTimeForStop();
  ///

  // 状态显示
  if (loopCircleCounter == LOOP_CIRCLE_MAX) {
    // 清屏
    I2COled::clear();
    // 电流
    sensorUPSCurrentStr = String(SensorHandler::readUPSCurrentValue(SensorHandler::ACS712_05B), 2);
    I2COled::displayUpsCurrent(sensorUPSCurrentStr);
    // 电压
    sensorUPSVoltageStr = String(SensorHandler::readUPSVoltageValue(), 2);
    I2COled::displayUpsVoltage(sensorUPSVoltageStr);
    // 蓝牙连接状态
    I2COled::displayBLEConnectStatus(deviceConnected);

    // 往遥控客户端推送
    if (pCharacteristic) {
      BLENotifyValueStr = "UPSCurrent=" + sensorUPSCurrentStr + "&UPSVoltage=" + sensorUPSVoltageStr;
      pCharacteristic->setValue(BLENotifyValueStr.c_str());
      pCharacteristic->notify();
    }
  }

  // disconnecting  断开连接
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);                   // 留时间给蓝牙缓冲
    pServer->startAdvertising();  // 重新广播
    Serial.println(" 开始广播 ");
    oldDeviceConnected = deviceConnected;
  }
  // connecting  正在连接
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }

}
