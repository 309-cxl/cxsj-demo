/*
  实验6：警车双闪灯效（双通道PWM）
  功能：两个LED呈现反相呼吸效果，一个变亮时另一个变暗
  硬件：ESP32开发板 + LED1(GPIO18) + LED2(GPIO19)
  提示：需要连接两个LED到不同引脚
*/

// 定义LED引脚
const int LED1_PIN = 18;  // 第一个LED引脚
const int LED2_PIN = 19;  // 第二个LED引脚

// PWM通道（ESP32有16个PWM通道，0-15）
const int PWM_CHANNEL_1 = 0;  // LED1使用通道0
const int PWM_CHANNEL_2 = 1;  // LED2使用通道1

// PWM参数
const int PWM_FREQ = 5000;     // PWM频率5kHz
const int PWM_RESOLUTION = 8;  // 8位分辨率（0-255）

// 呼吸灯参数
int brightness = 0;      // 当前亮度值（0-255）
int fadeStep = 5;        // 每次变化的步长
int delayTime = 20;      // 延时（毫秒）

void setup() {
  Serial.begin(115200);
  
  // 配置PWM通道1（LED1）
  ledcSetup(PWM_CHANNEL_1, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(LED1_PIN, PWM_CHANNEL_1);
  
  // 配置PWM通道2（LED2）
  ledcSetup(PWM_CHANNEL_2, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(LED2_PIN, PWM_CHANNEL_2);
  
  Serial.println("========== 警车双闪灯效（双通道PWM） ==========");
  Serial.println("LED1(GPIO18) 和 LED2(GPIO19) 呈现反相呼吸效果");
  Serial.println("一个变亮时，另一个变暗");
  Serial.println("==============================================");
}

void loop() {
  // LED1：从暗到亮（0 -> 255）
  // LED2：从亮到暗（255 -> 0）
  // 两者呈反相关系
  
  // 设置LED1的亮度
  ledcWrite(PWM_CHANNEL_1, brightness);
  
  // 设置LED2的亮度（反相：255 - brightness）
  ledcWrite(PWM_CHANNEL_2, 255 - brightness);
  
  // 调试输出（可选）
  Serial.print("LED1亮度: ");
  Serial.print(brightness);
  Serial.print(" | LED2亮度: ");
  Serial.println(255 - brightness);
  
  // 改变亮度值
  brightness += fadeStep;
  
  // 到达边界时反转方向
  if (brightness <= 0 || brightness >= 255) {
    fadeStep = -fadeStep;
    brightness = constrain(brightness, 0, 255);
  }
  
  // 延时控制速度
  delay(delayTime);
}
