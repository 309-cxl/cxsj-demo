/*
 * 作业3: 使用 millis() 函数控制LED产生SOS闪烁信号
 * 
 * SOS摩尔斯电码: 三短(S) 三长(O) 三短(S)
 * - 短闪(DOT):   亮200ms, 灭200ms
 * - 长闪(DASH):  亮600ms, 灭200ms  
 * - 字母间隔:    500ms
 * - 单词间隔:    2000ms
 * 
 * 完全使用millis()实现，非阻塞式状态机设计
 */

const int ledPin = 2;

// SOS信号时间定义 (单位: 毫秒)
const unsigned long DOT_ON     = 200;   // 短闪-亮
const unsigned long DOT_OFF    = 200;   // 短闪-灭(符号间隔)
const unsigned long DASH_ON    = 600;   // 长闪-亮
const unsigned long DASH_OFF   = 200;   // 长闪-灭(符号间隔)
const unsigned long LETTER_GAP = 500;   // 字母间隔
const unsigned long WORD_GAP   = 2000;  // 单词间隔(SOS结束后)

// 状态机变量
unsigned long previousMillis = 0;
int step = 0;            // 当前执行到第几步 (0~20)
bool ledIsOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  Serial.println("====================================");
  Serial.println("Ex03 - SOS Signal with millis()");
  Serial.println("Pattern: ... --- ...  (SOS)");
  Serial.println("====================================");
  delay(500);
}

void loop() {
  unsigned long currentMillis = millis();
  unsigned long waitTime = getWaitTime(step);
  
  if (currentMillis - previousMillis >= waitTime) {
    previousMillis = currentMillis;
    
    if (ledIsOn) {
      // 当前是亮的 → 熄灭LED，进入下一步
      digitalWrite(ledPin, LOW);
      ledIsOn = false;
      step++;
      
      // 打印调试信息
      printStepInfo();
      
      // SOS完成，重置
      if (step >= 20) {
        Serial.println("\n>>> SOS COMPLETE! Restarting...\n");
        step = 0;
        delay(100); // 小延时避免立即重启
      }
    } else {
      // 当前是灭的 → 点亮LED
      digitalWrite(ledPin, HIGH);
      ledIsOn = true;
    }
  }
}

// 根据当前步骤返回需要等待的时间
unsigned long getWaitTime(int s) {
  if (ledIsOn) {
    // LED正在亮，返回"亮"的时间
    // S的三个短闪: 步骤 0,2,4 (0-based偶数) 和 12,14,16
    // O的三个长闪: 步骤 6,8,10
    if (s == 0 || s == 2 || s == 4 || s == 12 || s == 14 || s == 16) return DOT_ON;
    if (s == 6 || s == 8 || s == 10) return DASH_ON;
  } else {
    // LED正在灭，返回"灭"的时间（含各种间隔）
    // 符号间隔(短闪后/长闪后): 大部分步骤
    // 字母间隔(S完成后/O完成后): 步骤5和11
    // 单词间隔(最后S完成): 步骤17
    if (s == 5 || s == 11) return LETTER_GAP;   // 字母间隔
    if (s == 17) return WORD_GAP;                // 单词间隔
    return DOT_OFF;                               // 默认符号间隔
  }
  return 200;
}

// 打印当前步骤信息
void printStepInfo() {
  Serial.print("Step ");
  Serial.print(step);
  Serial.print(": ");
  
  // 判断属于哪个字母
  if (step <= 5) {
    Serial.print("[S] ");
  } else if (step <= 11) {
    Serial.print("[O] ");
  } else {
    Serial.print("[S] ");
  }
  
  // 判断是点还是划
  int flashNum = (step + 1) / 2;
  if (step <= 5) {
    if (step % 2 == 1) { Serial.print("DOT "); Serial.println(flashNum); }
    else { Serial.print("gap"); if (step == 5) Serial.println(" (letter end)"); else Serial.println(); }
  } else if (step <= 11) {
    if (step % 2 == 1) { Serial.print("DASH "); Serial.println(flashNum - 3); }
    else { Serial.print("gap"); if (step == 11) Serial.println(" (letter end)"); else Serial.println(); }
  } else {
    if (step % 2 == 1) { Serial.print("DOT "); Serial.println(flashNum - 6); }
    else { Serial.print("gap"); if (step == 17) Serial.println(" *** WORD END ***"); else Serial.println(); }
  }
}
