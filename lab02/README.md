# Lab02: 基础IO实验 - LED闪烁控制

## 实验目标
使用 ESP32 开发板自带的 LED 灯（连接在 GPIO2），实现最基本的 LED 点亮和熄灭控制。

## 实验器材
- ESP32 开发板 ×1
- USB-C 数据线 ×1
- 电脑（已安装 Arduino IDE） ×1

## 实验原理
- **GPIO输出控制**: 通过 `pinMode()` 设置引脚为输出模式
- **digitalWrite()**: 输出高电平(HIGH)点亮LED，低电平(LOW)熄灭LED
- **delay()**: 延时函数，单位为毫秒(ms)

## 代码说明
- `const int ledPin = 2`: 定义LED引脚为GPIO2
- `pinMode(ledPin, OUTPUT)`: 设置GPIO2为输出模式
- `digitalWrite(ledPin, HIGH/LOW)`: 控制LED亮灭
- `delay(1000)`: 延时1秒

## 实验现象
LED以1秒为间隔循环闪烁（亮1秒，灭1秒）

## 串口输出
```
Lab02 - LED Blink Started
LED ON
LED OFF
LED ON
LED OFF
...
```

## 作者
实验日期: 2026-06-20
