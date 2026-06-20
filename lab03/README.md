# Lab03: LED呼吸灯实验

## 实验目标
使用 ESP32 开发板自带的 LED 灯（连接在 GPIO2），通过 PWM（脉冲宽度调制）技术实现呼吸灯效果。

## 实验器材
- ESP32 开发板 ×1
- USB-C 数据线 ×1
- 电脑（已安装 Arduino IDE） ×1

## PWM 原理简介

### 什么是 PWM?
PWM (Pulse Width Modulation) 即脉冲宽度调制。它是一种通过快速开关数字信号来模拟模拟信号的技术。

### 核心概念：占空比 (Duty Cycle)
占空比是指在一个脉冲周期内，高电平（ON）持续时间占整个周期时间的百分比。

- **100% 占空比**: 信号一直是高电平 (3.3V)，LED 最亮
- **50% 占空比**: 一半时间高电平，一半时间低电平，LED 亮度约为一半
- **0% 占空比**: 信号一直是低电平 (0V)，LED 熄灭

由于 LED 开关速度非常快（远超人眼识别频率），人眼看到的不是闪烁，而是平均亮度的变化。

## ESP32 PWM 函数（新版API）
- `ledcAttach(pin, freq, resolution)`: 绑定引脚到PWM通道
- `ledcWrite(pin, duty)`: 设置占空比值

## 代码说明
- `freq = 5000`: PWM频率5000Hz
- `resolution = 8`: 8位分辨率，占空比范围0-255
- `for` 循环: 逐渐递增/递减占空比实现渐变效果
- `delay(10)`: 控制渐变速度

## 实验现象
LED呈现呼吸灯效果，缓慢变亮再缓慢变暗，循环往复。

## 串口输出
```
Lab03 - LED Breathing Started
Breathing cycle completed
Breathing cycle completed
...
```

## 作者
实验日期: 2026-06-20
