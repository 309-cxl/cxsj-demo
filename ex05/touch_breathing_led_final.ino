/*
  实验5：多档位触摸调速呼吸灯（实时调试版）
  功能：实时显示触摸值，方便调试阈值
*/

// 定义引脚
const int TOUCH_PIN = T0;  // 触摸引脚0 (GPIO4)
const int LED_PIN = 2;     // 内置LED引脚

// 触摸阈值（根据你的ESP32调整这个值！）
int touchThreshold = 40;    // 如果触摸没反应，尝试改成 30 或 50

// 速度档位参数
int speedLevel = 1;
const int maxLevel = 3;
const int delayTime[] = {10, 5, 2};
const int stepSize[] = {5, 10, 20};

// 状态变量
bool lastTouchState = false;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;

// LED变量
int brightness = 0;
int fadeStep = 1;

// 调试计数器
int loopCount = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("========== 多档位触摸调速呼吸灯 ==========");
  Serial.println("触摸T0引脚(GPIO4)切换速度档位");
  Serial.print("当前档位：");
  Serial.println(speedLevel);
  Serial.print("触摸阈值：");
  Serial.println(touchThreshold);
  Serial.println("==========================================");
  Serial.println("观察下面的触摸值，触摸GPIO4时数值应该变小\n");
}

void loop() {
  // 读取触摸传感器
  int touchValue = touchRead(TOUCH_PIN);
  bool currentTouchState = (touchValue < touchThreshold);
  
  // ===== 每50次循环打印一次触摸值（避免刷屏）=====
  loopCount++;
  if (loopCount % 50 == 0) {
    Serial.print("触摸值: ");
    Serial.print(touchValue);
    Serial.print(" | 阈值: ");
    Serial.print(touchThreshold);
    Serial.print(" | 触摸状态: ");
    Serial.println(currentTouchState ? "已触摸" : "未触摸");
  }
  
  // ===== 边缘检测 + 防抖 =====
  if (currentTouchState != lastTouchState) {
    lastDebounceTime = millis();
  }
  
  // 防抖后判断触摸瞬间
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentTouchState && !lastTouchState) {
      // 切换档位
      speedLevel++;
      if (speedLevel > maxLevel) {
        speedLevel = 1;
      }
      
      Serial.println("\n========== 档位切换！ ==========");
      Serial.print("新档位: ");
      Serial.println(speedLevel);
      Serial.println("===============================\n");
    }
  }
  
  lastTouchState = currentTouchState;
  
  // ===== 呼吸灯效果 =====
  brightness = brightness + (fadeStep * stepSize[speedLevel - 1]);
  
  if (brightness <= 0 || brightness >= 255) {
    fadeStep = -fadeStep;
    brightness = constrain(brightness, 0, 255);
  }
  
  analogWrite(LED_PIN, brightness);
  delay(delayTime[speedLevel - 1]);
}
