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

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pCharacteristic = NULL;

bool deviceConnected = false;                //本次连接状态
bool oldDeviceConnected = false;             //上次连接状态d

BleServerCallbacks *bleServerCallbacks = new BleServerCallbacks(&deviceConnected);
ReceiveCallbacks *receiveCallbacks = new ReceiveCallbacks();

void setup() {
  GPIO_define_init();

  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("esp32");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(bleServerCallbacks); //设置回调
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                       CHARACTERISTIC_UUID,
                       BLECharacteristic::PROPERTY_READ |
                       BLECharacteristic::PROPERTY_WRITE |
                       BLECharacteristic::PROPERTY_NOTIFY |
                       BLECharacteristic::PROPERTY_INDICATE
                     );
  pCharacteristic->setCallbacks(receiveCallbacks); //设置回调

  pCharacteristic->setValue("HELLO BLE =>");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  // disconnecting  断开连接
  if (!deviceConnected && oldDeviceConnected)
  {
    delay(500);                  // 留时间给蓝牙缓冲
    pServer->startAdvertising(); // 重新广播
    Serial.println(" 开始广播 ");
    oldDeviceConnected = deviceConnected;
  }
  // connecting  正在连接
  if (deviceConnected && !oldDeviceConnected)
  {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}
