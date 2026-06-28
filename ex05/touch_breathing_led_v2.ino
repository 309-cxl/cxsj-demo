/*
  实验5：多档位触摸调速呼吸灯（改进版v2）
  改进点：
  1. 简化触摸检测逻辑
  2. 增加触摸值校准功能
  3. 添加更多调试输出
  4. 优化防抖算法
*/

// 定义引脚
const int TOUCH_PIN = T0;  // 触摸引脚0 (GPIO4)
const int LED_PIN = 2;     // 内置LED引脚

// ===== 触摸检测参数 =====
int touchThreshold = 40;    // 阈值（会在启动时自动校准）
const int debounceTime = 100;  // 防抖时间（毫秒）

// ===== 速度档位参数 =====
int speedLevel = 1;        // 当前档位（1-3）
const int maxLevel = 3;

// 档位参数：[延时ms, 步长]
const int params[3][2] = {
  {10, 5},   // 档位1：慢速
  {5, 10},    // 档位2：中速
  {2, 20}    // 档位3：快速
};

// ===== 状态变量 =====
bool isTouched = false;         // 当前触摸状态
bool wasTouched = false;        // 上一次触摸状态
unsigned long touchStartTime = 0; // 触摸开始时间

// ===== LED呼吸灯变量 =====
int brightness = 0;             // 当前亮度（0-255）
int fadeStep = 1;              // 渐变方向（+1或-1）

// ===== 启动时校准触摸阈值 =====
void calibrateTouch() {
  Serial.println("正在校准触摸阈值...");
  int sum = 0;
  int samples = 10;
  
  for (int i = 0; i < samples; i++) {
    int val = touchRead(TOUCH_PIN);
    sum += val;
    Serial.print("采样 ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(val);
    delay(100);
  }
  
  int avg = sum / samples;
  touchThreshold = avg - 10;  // 平均值减去10作为阈值
  
  Serial.print("校准完成！平均值: ");
  Serial.print(avg);
  Serial.print("，阈值设置为: ");
  Serial.println(touchThreshold);
  Serial.println("请触摸GPIO4引脚测试...\n");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("========== 多档位触摸调速呼吸灯 ==========");
  Serial.println("功能：触摸T0引脚(GPIO4)切换速度档位");
  Serial.println("档位1：慢速 | 档位2：中速 | 档位3：快速");
  Serial.println("==========================================\n");
  
  // 校准触摸阈值
  calibrateTouch();
  
  Serial.print("当前档位：");
  Serial.println(speedLevel);
}

void loop() {
  // ===== 1. 读取触摸传感器 =====
  int touchValue = touchRead(TOUCH_PIN);
  wasTouched = isTouched;
  isTouched = (touchValue < touchThreshold);
  
  // ===== 2. 调试输出（可选）=====
  // Serial.print("[调试] 触摸值: ");
  // Serial.print(touchValue);
  // Serial.print(" | 阈值: ");
  // Serial.print(touchThreshold);
  // Serial.print(" | 触摸状态: ");
  // Serial.println(isTouched ? "已触摸" : "未触摸");
  
  // ===== 3. 检测触摸瞬间（上升沿）=====
  if (isTouched && !wasTouched) {
    // 触摸开始
    touchStartTime = millis();
    Serial.println("检测到触摸...");
  }
  
  if (!isTouched && wasTouched) {
    // 触摸结束（松开）
    unsigned long touchDuration = millis() - touchStartTime;
    
    // 只有触摸持续时间 > 防抖时间才切换档位
    if (touchDuration > debounceTime) {
      // 切换档位
      speedLevel++;
      if (speedLevel > maxLevel) {
        speedLevel = 1;
      }
      
      Serial.println("========== 档位切换！ ==========");
      Serial.print("新档位: ");
      Serial.println(speedLevel);
      Serial.print("延时: ");
      Serial.print(params[speedLevel - 1][0]);
      Serial.println("ms");
      Serial.print("步长: ");
      Serial.println(params[speedLevel - 1][1]);
      Serial.println("===============================\n");
    } else {
      Serial.println("触摸时间太短，忽略（防抖）\n");
    }
  }
  
  // ===== 4. 呼吸灯效果 =====
  brightness += fadeStep * params[speedLevel - 1][1];
  
  // 到达边界时反转方向
  if (brightness <= 0 || brightness >= 255) {
    fadeStep = -fadeStep;
    brightness = constrain(brightness, 0, 255);
  }
  
  // PWM输出
  analogWrite(LED_PIN, brightness);
  
  // 根据档位延时
  delay(params[speedLevel - 1][0]);
}
