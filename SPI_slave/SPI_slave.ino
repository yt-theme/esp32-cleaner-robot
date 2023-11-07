//#include <Servo.h>
//#include<SPI.h>
//
//volatile boolean received;
//volatile byte Slavereceived,Slavesend;
//
//Servo myservo_0;
//Servo myservo_1;
//
//void doAction();
//int pos = 0;    // variable to store the servo position
//
//void setup() {
//  pinMode(LED_BUILTIN, OUTPUT);
//  Serial.begin(115200);
//
//  myservo_0.attach(11);
//  myservo_1.attach(10);
//
//  pinMode(MISO,OUTPUT);
//
//  SPCR |= _BV(SPE);                       //Turn on SPI in Slave Mode
//  received = false;
//
//  SPI.attachInterrupt();                  //Interuupt ON is set for SPI commnucation
//}
//
//ISR (SPI_STC_vect)                        //Inerrrput routine function
//{
//  Slavereceived = SPDR;         // Value received from master if store in variable slavereceived
//  received = true;                        //Sets received as True
//}
//
//void loop() {
//  if (received) {
//    digitalWrite(LED_BUILTIN, HIGH);
//    Serial.println(Slavereceived);
//    delay(20);
//    digitalWrite(LED_BUILTIN, LOW);
//
////     Slavesend=x;
////      SPDR = Slavesend;                           //Sends the x value to master via SPDR
////      delay(1000);
//  }
//
//}
//
//void doAction()
//{
//   for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     myservo_0.write(pos);              // tell servo to go to position in variable 'pos'
//     myservo_1.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15 ms for the servo to reach the position
//   }
//   for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//     myservo_0.write(pos);              // tell servo to go to position in variable 'pos'
//     myservo_1.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15 ms for the servo to reach the position
//   }
//}

#include <SPI.h>
char buff[64];
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
}


ISR (SPI_STC_vect) // SPI中断程序
{
  digitalWrite(LED_BUILTIN, HIGH);
  byte c = SPDR; // 从SPI数据寄存器读取字节
  Serial.println("ISR =>");

  if (indx < sizeof buff) {
    buff [indx++] = c; // 将数据保存在数组buff中的下一个索引中
//    Serial.println(c);
    if (c == '\n') //检查是否是结尾字符,即检测字符是否是\r回车符
      process = true;
  }

  digitalWrite(LED_BUILTIN, LOW);
}

void loop () {
  if (process) {
    Serial.println("process =>");
    process = false; //重置通讯过程
    Serial.println (buff); //在串口监视器上打印接收到的buff数据
    indx = 0; //重置index,即为重置buff索引
  }
}
