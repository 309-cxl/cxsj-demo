/*
 * 实验3: LED呼吸灯实验
 * 使用 ESP32 PWM 技术实现呼吸灯效果
 * 
 * 硬件: ESP32开发板 (板载LED在GPIO2)
 * 功能: LED呈现呼吸灯效果（渐亮渐暗）
 */

// 定义LED引脚
const int ledPin = 2;

// 设置PWM属性
const int freq = 5000;      // 频率 5000Hz
const int resolution = 8;   // 分辨率 8位 (0-255)

void setup() {
  Serial.begin(115200);
  
  // 【新版用法】直接将引脚、频率和分辨率绑定
  ledcAttach(ledPin, freq, resolution);
  
  Serial.println("Lab03 - LED Breathing Started");
}

void loop() {
  // 逐渐变亮
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    // 【新版用法】直接通过引脚号写入
    ledcWrite(ledPin, dutyCycle);
    delay(10);
  }

  // 逐渐变暗
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    ledcWrite(ledPin, dutyCycle);
    delay(10);
  }

  Serial.println("Breathing cycle completed");
}
