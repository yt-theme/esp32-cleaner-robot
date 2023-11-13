#include <SPI.h>
// #include "dynamical_handler.h"
#include "dynamical_handler_TT.h"
#include "common.h"

char buff[64];
char cmdBuff[64]; // 处理后的指令
volatile byte indx;
volatile boolean process;

void setup () {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MISO, OUTPUT); //将MISO设置为输出以便数据发送主机
  SPCR |= _BV(SPE); //在从机模式下打开SPI通讯
  indx = 0; // 初始化变量
  process = false;
  SPI.attachInterrupt(); //打开中断
  Serial.println("slave =>");

  // 动力部分初始化
  DynamicalHandlerTT::init();
}

ISR (SPI_STC_vect) // SPI中断程序
{
  digitalWrite(LED_BUILTIN, HIGH);
  byte c = SPDR; // 从SPI数据寄存器读取字节

  if (indx < sizeof buff) {
    buff [indx++] = c; // 将数据保存在数组buff中的下一个索引中
    //    Serial.println(c);
    if (c == '\n') //检查是否是结尾字符,即检测字符是否是\r回车符
    {
      // 处理指令去掉换行
      memset(cmdBuff, 0, sizeof cmdBuff);
      for (byte tmpInd = 0; tmpInd < indx; tmpInd++)
      {
        if (buff[tmpInd] == '\n')
        {
          continue;
        }
        cmdBuff[tmpInd] = buff[tmpInd];
      }
      process = true;
    }
  }

  digitalWrite(LED_BUILTIN, LOW);
}

void loop () {

  // 用于延时关闭动力
  DynamicalHandlerTT::dynListenTimeForStop();

  if (process) {
    Serial.print("process =>");
    process = false; //重置通讯过程
    Serial.println (cmdBuff); //在串口监视器上打印接收到的buff数据
    indx = 0; //重置index,即为重置buff索引

    // 处理动力命令
    DynamicalHandlerTT::dynamicalHandler(cmdBuff);
  }
}
