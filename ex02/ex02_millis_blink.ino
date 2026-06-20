/*
 * 作业2: 使用 millis() 函数控制LED稳定闪烁
 * 
 * millis() 返回系统启动后的毫秒数
 * 优势: 非阻塞式，不会暂停程序执行
 * 
 * 功能: LED以1Hz频率稳定闪烁（500ms亮，500ms灭）
 */

const int ledPin = 2;                // LED引脚
const long interval = 500;           // 间隔500ms (1Hz = 周期1000ms)
unsigned long previousMillis = 0;     // 上次切换时间
int ledState = LOW;                  // LED当前状态

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  Serial.println("Ex02 - LED Blink with millis()");
  Serial.println("Frequency: 1Hz (500ms ON, 500ms OFF)");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // 检查是否到达切换时间
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // 切换LED状态
    if (ledState == LOW) {
      ledState = HIGH;
      Serial.println("LED ON");
    } else {
      ledState = LOW;
      Serial.println("LED OFF");
    }
    
    digitalWrite(ledPin, ledState);
  }
  
  // 这里可以添加其他非阻塞代码
  // millis()方法的优势：不会阻塞其他操作
}
