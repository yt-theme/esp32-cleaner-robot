#pragma once
#include "BLE_cmd.h"
#include "GPIO_define.h"

void dynamicalHandler(std::string cmdVal)
{
  if (cmdVal == BLE_CMD_FORWARD)
  {
    ledcWrite(MOTO_CHANNEL_PWM_0, 52); //20ms高电平为1ms左右 ，也就是1/20*1024 = 52，此时360度舵机正传5秒
    ledcWrite(MOTO_CHANNEL_PWM_1, 52);
  }
  else if(cmdVal == BLE_CMD_BACKWARD)
  {
    
  }
  else if (cmdVal == BLE_CMD_TRUNLEFT)
  {
    
  }
  else if (cmdVal == BLE_CMD_TRUNRIGHT)
  {
    
  }
  else if (cmdVal == BLE_CMD_GEAR0)
  {
    
  }
  else if (cmdVal == BLE_CMD_GEAR1)
  {
    
  }
  else if (cmdVal == BLE_CMD_GEAR2)
  {
    
  }
}
