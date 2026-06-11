// Lab01: ESP32 LED Blink Test
// 功能：验证 Arduino IDE + ESP32 开发环境配置成功
// 现象：板载LED每秒闪烁一次，串口输出 "Hello ESP32!"

// 定义板载LED引脚（GPIO 2）
#define LED_PIN 2

void setup() {
  // 初始化串口通信，波特率 115200
  Serial.begin(115200);
  // 初始化板载LED引脚为输出模式
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  Serial.println("Hello ESP32!");
  digitalWrite(LED_PIN, HIGH);  // 点亮LED
  delay(1000);                  // 等待1秒
  digitalWrite(LED_PIN, LOW);   // 熄灭LED
  delay(1000);                  // 等待1秒
}
