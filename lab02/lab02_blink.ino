/*
 * 实验2: 基础IO实验 - LED闪烁控制
 * 使用 ESP32 GPIO 输出控制基本方法
 * 
 * 硬件: ESP32开发板 (板载LED在GPIO2)
 * 功能: LED以1秒间隔闪烁
 */

// 定义LED引脚，ESP32通常板载LED连接在GPIO 2
const int ledPin = 2;

void setup() {
  // 初始化串口通信，设置波特率为115200
  Serial.begin(115200);
  
  // 将LED引脚设置为输出模式
  pinMode(ledPin, OUTPUT);
  
  Serial.println("Lab02 - LED Blink Started");
}

void loop() {
  digitalWrite(ledPin, HIGH);   // 点亮LED
  Serial.println("LED ON");     // 串口输出提示
  delay(1000);                  // 保持1秒（1000毫秒）

  digitalWrite(ledPin, LOW);    // 熄灭LED
  Serial.println("LED OFF");    // 串口输出提示
  delay(1000);                  // 保持1秒
}
