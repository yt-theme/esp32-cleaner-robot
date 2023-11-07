#pragma once

#include <SPI.h>
#include "GPIO_define.h"

namespace SpiHandler {

static SPIClass spiObj(HSPI);
static int count = 0;
static uint8_t txbuf[64];
static uint8_t rxbuf[64];

static void init() {
  spiObj.begin();
  pinMode(spiObj.pinSS(), OUTPUT);
  digitalWrite(SLAVE_SELECT_PIN, HIGH);
}

static uint8_t* transfer(const char * data, const uint32_t size) {
  spiObj.beginTransaction(SPISettings(CUSTOM_SPI_SPEED, MSBFIRST, SPI_MODE0));    //设置SPI的传输参数
  digitalWrite(spiObj.pinSS(), LOW);

  for (int i = 0; i<size; i++) {
    spiObj.transfer(data[i]);
    Serial.print(data[i]);
  }
  spiObj.transfer('\n'); 

  digitalWrite(spiObj.pinSS(), HIGH);
  spiObj.endTransaction();

  return rxbuf;
}

// 仅测试
static void testSend() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second

  transfer("direct.02", 9);
  
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second

}

}