#include "esp32-hal-adc.h"
#pragma once

#include "GPIO_define.h"

namespace SensorHandler
{
  
  #define ADC_SCALE 4095.0//采样值 arduino为1023
  #define VREF 3.3 //参考电压，即满量程模拟电压，arduino为5

  /* *********************************************************
  *               UPS电流传感器读取 ACS712
  ********************************************************* */
  // ACS712 type
  enum ACS712_type {ACS712_05B, ACS712_20A, ACS712_30A};

  // 电流传感器零点
  int currentZero;

  // 电流传感器零点校准
  int currentCalibrate()
  {
    // 零点校准
    uint16_t acc = 0;
    for (int i = 0; i < 50; i++) {// 50次取样平均，原版10次
        acc += analogRead(SENSOR_CURRENT_ADC);
    }
    currentZero = acc / 50;
    return currentZero;
  }

  // read ACS712 (mA)
  float readUPSCurrentValue(ACS712_type type)
  {
    // 根据传感器类型校准灵敏度
    float sensitivity = 0.000; // 灵敏度mV/A
    switch (type) {
        case ACS712_05B: // 5A
            sensitivity = 0.185;
            break;
        case ACS712_20A: // 20A
            sensitivity = 0.100;
            break;
        case ACS712_30A: // 30A
            sensitivity = 0.066;
            break;
    }
    // compute
    int16_t acc = 0;
    for (int i = 0; i < 50; i++) {//50次采样，原版10次
        acc += analogRead(SENSOR_CURRENT_ADC) - currentZero;
    }
    float I = (float)acc / 50.0 / ADC_SCALE * VREF / sensitivity * 1000;
    Serial.print("current =>");
    Serial.println(I);
    return I;
  }

  /* *********************************************************
  *               UPS电压传感器读取  
  ********************************************************* */
  float readUPSVoltageValue()
  {
    // float voltageValue = 0.0;
    // // 参考电压
    // float refVoltage = 12.0;

    // float R1 = 30000.0;
    // float R2 = 5000.0; 

    // int sensorVal = analogRead(SENSOR_VOLTAGE_ADC);
    // float sensorVoltageValue = (sensorVal * refVoltage) / 4096.0;
    // voltageValue = sensorVoltageValue / (R2/(R1+R2));
    // return voltageValue;
    
    int sensorVal = analogRead(SENSOR_VOLTAGE_ADC);
    return (float)sensorVal * 5 / 1023.0;
  }
}