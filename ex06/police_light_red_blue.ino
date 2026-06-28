/*
  实验6：警车双闪灯效 - 红蓝闪烁版（双通道PWM）
  功能：模拟警车红蓝灯交替闪烁效果
  硬件：ESP32开发板 + 红色LED(GPIO18) + 蓝色LED(GPIO19)
  提示：建议使用红蓝两种颜色的LED，效果更好
*/

// 定义LED引脚
const int RED_LED_PIN = 18;   // 红色LED引脚
const int BLUE_LED_PIN = 19;  // 蓝色LED引脚

// PWM通道
const int PWM_CHANNEL_RED = 0;   // 红色LED使用通道0
const int PWM_CHANNEL_BLUE = 1;  // 蓝色LED使用通道1

// PWM参数
const int PWM_FREQ = 5000;     // PWM频率5kHz
const int PWM_RESOLUTION = 8;  // 8位分辨率（0-255）

// 闪烁模式参数
int brightness = 0;          // 当前亮度
int fadeStep = 8;           // 变化步长（越大变化越快）
int delayTime = 15;         // 延时（毫秒）

// 闪烁模式选择
// 0: 平滑呼吸（反相）
// 1: 快速闪烁（模拟警车）
// 2: 交替闪烁
int flashMode = 1;          // 默认使用警车模式

void setup() {
  Serial.begin(115200);
  
  // 配置PWM通道
  ledcSetup(PWM_CHANNEL_RED, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(RED_LED_PIN, PWM_CHANNEL_RED);
  
  ledcSetup(PWM_CHANNEL_BLUE, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(BLUE_LED_PIN, PWM_CHANNEL_BLUE);
  
  Serial.println("========== 警车双闪灯效（红蓝闪烁版） ==========");
  Serial.println("红色LED(GPIO18) 和 蓝色LED(GPIO19)");
  Serial.println("模式1: 警车闪烁效果");
  Serial.println("==============================================");
}

void loop() {
  int redBrightness, blueBrightness;
  
  switch(flashMode) {
    case 0:  // 模式0: 平滑呼吸（反相）
      redBrightness = brightness;
      blueBrightness = 255 - brightness;
      break;
      
    case 1:  // 模式1: 警车闪烁（快速交替）
      // 使用正弦波效果，让过渡更自然
      if (brightness < 128) {
        // 前半周期：红色亮，蓝色暗
        redBrightness = brightness * 2;
        blueBrightness = 0;
      } else {
        // 后半周期：红色暗，蓝色亮
        redBrightness = 0;
        blueBrightness = (255 - brightness) * 2;
      }
      break;
      
    case 2:  // 模式2: 交替闪烁（硬切换）
      if (brightness < 128) {
        redBrightness = 255;
        blueBrightness = 0;
      } else {
        redBrightness = 0;
        blueBrightness = 255;
      }
      break;
      
    default:
      redBrightness = brightness;
      blueBrightness = 255 - brightness;
  }
  
  // 确保亮度值在0-255范围内
  redBrightness = constrain(redBrightness, 0, 255);
  blueBrightness = constrain(blueBrightness, 0, 255);
  
  // 输出PWM
  ledcWrite(PWM_CHANNEL_RED, redBrightness);
  ledcWrite(PWM_CHANNEL_BLUE, blueBrightness);
  
  // 调试输出
  Serial.print("红: ");
  Serial.print(redBrightness);
  Serial.print(" | 蓝: ");
  Serial.println(blueBrightness);
  
  // 改变亮度值
  brightness += fadeStep;
  
  // 到达边界时反转方向
  if (brightness <= 0 || brightness >= 255) {
    fadeStep = -fadeStep;
    brightness = constrain(brightness, 0, 255);
  }
  
  delay(delayTime);
}
