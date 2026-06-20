/*
 * 实验2: 基础IO实验 - LED闪烁控制 + SOS进阶实验
 * 使用 ESP32 GPIO 输出控制基本方法
 * 
 * 硬件: ESP32开发板 (板载LED在GPIO2)
 * 功能1: LED以1秒间隔闪烁
 * 功能2: SOS求救信号模式 (三短三长三短)
 */

#define LED_PIN 2

void setup() {
  // 初始化串口通信，设置波特率为115200
  Serial.begin(115200);
  
  // 初始化板载LED引脚为输出模式
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("Lab02 - ESP32 LED Control Started");
}

void loop() {
  // ====== 基础闪烁：LED亮1秒，灭1秒 ======
  digitalWrite(LED_PIN, HIGH);   // 点亮LED
  Serial.println("LED ON");      // 串口输出提示
  delay(1000);                   // 等待1秒（1000毫秒）
  
  digitalWrite(LED_PIN, LOW);    // 熄灭LED
  Serial.println("LED OFF");     // 串口输出提示
  delay(1000);                   // 等待1秒
  
  // ====== 进阶实验：SOS求救信号 (摩尔斯电码) ======
  // S: 三短闪
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);                  // 短闪200ms
    digitalWrite(LED_PIN, LOW);
    delay(200);                  // 间隔200ms
  }
  delay(500);                    // 字母间隔500ms
  
  // O: 三长闪
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(600);                  // 长闪600ms
    digitalWrite(LED_PIN, LOW);
    delay(200);                  // 间隔200ms
  }
  delay(500);                    // 字母间隔500ms
  
  // S: 三短闪
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);                  // 短闪200ms
    digitalWrite(LED_PIN, LOW);
    delay(200);                  // 间隔200ms
  }
  delay(2000);                   // 单词间隔2000ms
  
  Serial.println("--- SOS Complete ---");
}
