#pragma once

#include <Wire.h>
#include <string>
#include "SSD1306.h"
#include "GPIO_define.h"

namespace I2COled {
  extern SSD1306 display(0x3c, I2COLED_SDA, I2COLED_SCL);

  void init()
  {
    
    display.init();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "init..");
    display.display();
  }

  void clear()
  {
    display.clear();
  }

  /*
  * 显示ups电流
  */
  void displayUpsCurrent(const String& valStr)
  {
    display.drawString(0, 1, "I(mA) = " + valStr);
    display.display();
  }

  /*
  * 显示ups电压
  */
  void displayUpsVoltage(const String& valStr)
  {
    display.drawString(0, 12, "U(V)  = " + valStr);
    display.display();
  }

  /*
  * 显示蓝牙接收的命令
  */
  void displayBLECommand(const std::string& cmdStr)
  {
    display.drawString(0, 24, cmdStr.data());
    display.display();
  }

  /*
  * 显示debug信息
  */
  void displayDebugMessage(const char* debugStr)
  {
    display.drawString(0, 36, debugStr);
    display.display();
  }
}