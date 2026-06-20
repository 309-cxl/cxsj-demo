# Ex02: 使用 millis() 控制LED稳定闪烁

## 实验目标
使用 `millis()` 函数替代 `delay()` 实现非阻塞式LED控制，以1Hz频率稳定闪烁。

## millis() 函数说明
- 返回Arduino启动后的毫秒数（unsigned long类型，约49天溢出归零）
- **核心优势**: 非阻塞式，不会暂停程序执行
- 可以同时执行多个任务（多任务处理基础）

## 1Hz频率计算
```
1Hz = 1次完整周期/秒
周期 = 1000ms
半个周期(亮或灭) = 500ms
```

## 核心代码逻辑 (状态机模式)
```cpp
unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    // 切换LED状态
}
```

## delay() vs millis() 对比
| 特性 | delay() | millis() |
|------|---------|----------|
| 阻塞性 | 阻塞整个程序 | 非阻塞 |
| 多任务 | ❌ 无法同时做其他事 | ✅ 可以同时处理多任务 |
| 精度 | 一般 | 更精确 |
| 适用场景 | 简单延时 | 复杂时序控制 |

## 作者: 陈先炼
## 实验日期: 2026-06-20
