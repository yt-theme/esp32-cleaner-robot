#pragma once
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE_cmd.h"
// #include "dynamical_handler2.h"
#include "spi_handler.h"

/*
* BLE server callbacks
*/
class BleServerCallbacks : public BLEServerCallbacks {
public:
  BleServerCallbacks(bool *deviceConnected) {
    this->deviceConnected = deviceConnected;
  }
private:
  bool *deviceConnected = NULL;

  void onConnect(BLEServer *pServer) {
    *deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer) {
    *deviceConnected = false;
  }
};

/*
* BLE receive callbacks
*/
class ReceiveCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();  //接收信息

    if (rxValue.length() > 0) {  //向串口输出收到的值
      Serial.print("RX: ");
      for (int i = 0; i < rxValue.length(); i++)
        Serial.print(rxValue[i]);
      Serial.println();

      /*
          测试灯
        */
      if (rxValue == BLE_CMD_BOARD_LED_1) {
        digitalWrite(LED_BUILTIN, HIGH);
      }

      if (rxValue == BLE_CMD_BOARD_LED_0) {
        digitalWrite(LED_BUILTIN, LOW);
      }

      /*
          动力部分
        */
      if (rxValue == BLE_CMD_FORWARD || 
          rxValue == BLE_CMD_BACKWARD || 
          rxValue == BLE_CMD_TRUNLEFT || 
          rxValue == BLE_CMD_TRUNRIGHT || 
          rxValue == BLE_CMD_GEAR0 || 
          rxValue == BLE_CMD_GEAR1 || 
          rxValue == BLE_CMD_GEAR2 ||
          rxValue.find(BLE_CMD_GEAR_CUSTOM) != std::string::npos
          ) {
        // DynamicalHandler2::dynamicalHandler(rxValue);
        const char * cmdCharArr = rxValue.data();
        SpiHandler::transfer(cmdCharArr, strlen(cmdCharArr));
      }
    }
  }
};
