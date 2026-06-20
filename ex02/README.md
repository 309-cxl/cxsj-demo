# Ex02: 使用 millis() 控制LED稳定闪烁

## 实验目标
使用 `millis()` 函数替代 `delay()` 实现非阻塞式的LED控制，LED以1Hz频率稳定闪烁。

## millis() 函数说明
- `millis()`: 返回Arduino启动后的毫秒数（unsigned long类型）
- 优势: 非阻塞式，不会暂停程序执行
- 可以同时执行其他任务

## 实现原理
```
1Hz = 1次完整周期/秒 = 1000ms/周期
半个周期 = 500ms (亮500ms, 灭500ms)
```

## 核心代码逻辑
```cpp
unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval) {
  previousMillis = currentMillis;
  // 切换LED状态
}
```

## 代码特点
- ✅ 非阻塞式设计
- ✅ 精确的时间控制
- ✅ 可以同时执行其他任务
- ✅ 避免delay()带来的程序停顿

## 实验现象
LED以1Hz频率稳定闪烁（亮500ms，灭500ms）

## 串口输出
```
Ex02 - LED Blink with millis() Started
Frequency: 1Hz (500ms ON, 500ms OFF)
LED ON
LED OFF
LED ON
LED OFF
...
```

## 作者
实验日期: 2026-06-20
