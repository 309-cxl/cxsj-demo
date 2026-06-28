/*
  实验5：多档位触摸调速呼吸灯
  功能：触摸切换速度档位，LED呈现不同速度的呼吸效果
  硬件：ESP32开发板 + 触摸引脚(T0=GPIO4) + LED(GPIO2)
*/

// 定义引脚
const int touchPin = T0;  // 触摸引脚0 (GPIO4)
const int ledPin = 2;     // 内置LED引脚

// 触摸阈值（需要根据实际情况调整）
const int touchThreshold = 40;

// 速度档位（1-3档）
int speedLevel = 1;
const int maxSpeedLevel = 3;

// 档位对应的参数
// 档位1：慢速呼吸（延时50ms，步长1）
// 档位2：中速呼吸（延时20ms，步长2）
// 档位3：快速呼吸（延时5ms，步长5）
int delayTime[] = {50, 20, 5};
int stepSize[] = {1, 2, 5};

// 触摸状态变量
bool lastTouchState = false;
bool currentTouchState = false;

// 防抖相关
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;

// LED亮度值
int brightness = 0;
int fadeStep = 1;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  
  Serial.println("多档位触摸调速呼吸灯实验开始！");
  Serial.println("触摸T0引脚(GPIO4)切换速度档位");
  Serial.print("当前档位：");
  Serial.println(speedLevel);
}

void loop() {
  // 读取触摸传感器
  int touchValue = touchRead(touchPin);
  currentTouchState = (touchValue < touchThreshold);
  
  // 边缘检测 + 防抖
  if (currentTouchState != lastTouchState) {
    lastDebounceTime = millis();
  }
  
  // 防抖后判断触摸瞬间
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentTouchState && !lastTouchState) {
      // 触摸瞬间，切换档位
      speedLevel++;
      if (speedLevel > maxSpeedLevel) {
        speedLevel = 1;
      }
      
      Serial.print("档位切换为：");
      Serial.println(speedLevel);
    }
  }
  
  lastTouchState = currentTouchState;
  
  // 呼吸灯效果
  // 根据当前档位调整步长
  brightness = brightness + (fadeStep * stepSize[speedLevel - 1]);
  
  // 到达边界时反转方向
  if (brightness <= 0 || brightness >= 255) {
    fadeStep = -fadeStep;
    brightness = constrain(brightness, 0, 255);
  }
  
  // PWM输出
  analogWrite(ledPin, brightness);
  
  // 根据档位调整延时
  delay(delayTime[speedLevel - 1]);
  
  // 调试信息（可选，打开会刷屏）
  // Serial.print("档位: ");
  // Serial.print(speedLevel);
  // Serial.print(" | 亮度: ");
  // Serial.println(brightness);
}
