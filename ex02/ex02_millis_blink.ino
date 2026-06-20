/*
 * 作业2: 使用 millis() 函数控制LED稳定闪烁
 * 实现非阻塞式的LED控制
 * 
 * 硬件: ESP32开发板 (板载LED在GPIO2)
 * 功能: LED以1Hz频率稳定闪烁（500ms亮，500ms灭）
 */

const int ledPin = 2;           // LED引脚

// 闪烁参数
const long interval = 500;      // 间隔时间500ms（1Hz = 周期1000ms）
unsigned long previousMillis = 0;  // 记录上次切换时间
int ledState = LOW;             // LED当前状态

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  Serial.println("Ex02 - LED Blink with millis() Started");
  Serial.println("Frequency: 1Hz (500ms ON, 500ms OFF)");
}

void loop() {
  // 获取当前时间
  unsigned long currentMillis = millis();
  
  // 检查是否到达切换时间
  if (currentMillis - previousMillis >= interval) {
    // 保存上次切换时间
    previousMillis = currentMillis;
    
    // 切换LED状态
    if (ledState == LOW) {
      ledState = HIGH;
      Serial.println("LED ON");
    } else {
      ledState = LOW;
      Serial.println("LED OFF");
    }
    
    // 更新LED输出
    digitalWrite(ledPin, ledState);
  }
  
  // 这里可以添加其他非阻塞代码
  // millis()方法的优势：不会阻塞其他操作
}
