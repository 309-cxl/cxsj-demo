/*
  实验5：多档位触摸调速呼吸灯（调试版本）
  功能：添加详细调试信息，帮助排查触摸档位切换问题
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
int delayTime[] = {10, 5, 2};
int stepSize[] = {5, 10, 20};

// 触摸状态变量
bool lastTouchState = false;
bool currentTouchState = false;

// 防抖相关
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;

// LED亮度值
int brightness = 0;
int fadeStep = 1;

// 调试计数器
int touchCount = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  
  Serial.println("========== 多档位触摸调速呼吸灯（调试模式） ==========");
  Serial.println("提示：打开串口监视器查看调试信息");
  Serial.print("当前档位：");
  Serial.println(speedLevel);
  Serial.print("触摸阈值：");
  Serial.println(touchThreshold);
  Serial.println("======================================================\n");
}

void loop() {
  // 读取触摸传感器
  int touchValue = touchRead(touchPin);
  currentTouchState = (touchValue < touchThreshold);
  
  // ===== 调试信息1：实时触摸值 =====
  Serial.print("[调试] 触摸值: ");
  Serial.print(touchValue);
  Serial.print(" | 阈值: ");
  Serial.print(touchThreshold);
  Serial.print(" | 当前触摸状态: ");
  Serial.print(currentTouchState ? "已触摸" : "未触摸");
  Serial.print(" | 上一次触摸状态: ");
  Serial.println(lastTouchState ? "已触摸" : "未触摸");
  
  // ===== 边缘检测 + 防抖 =====
  if (currentTouchState != lastTouchState) {
    lastDebounceTime = millis();
    Serial.println("[调试] 检测到状态变化，开始防抖...");
  }
  
  // 防抖后判断触摸瞬间
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentTouchState && !lastTouchState) {
      // 触摸瞬间，切换档位
      touchCount++;
      speedLevel++;
      if (speedLevel > maxSpeedLevel) {
        speedLevel = 1;
      }
      
      Serial.println("\n========== 档位切换！ ==========");
      Serial.print("触摸次数: ");
      Serial.println(touchCount);
      Serial.print("新档位: ");
      Serial.println(speedLevel);
      Serial.print("延时: ");
      Serial.print(delayTime[speedLevel - 1]);
      Serial.println("ms");
      Serial.print("步长: ");
      Serial.println(stepSize[speedLevel - 1]);
      Serial.println("===============================\n");
    }
  }
  
  lastTouchState = currentTouchState;
  
  // ===== 呼吸灯效果 =====
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
}
