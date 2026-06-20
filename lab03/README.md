# Lab03: LED呼吸灯实验

## 实验目标
使用 ESP32 开发板自带的 LED 灯（GPIO2），通过 PWM 技术实现呼吸灯效果。

## 实验器材
- ESP32 开发板 x1
- USB-C 数据线 x1
- 电脑（已安装 Arduino IDE）x1

## PWM 原理
- **PWM (脉冲宽度调制)**: 通过快速开关数字信号模拟模拟信号
- **占空比 (Duty Cycle)**: 高电平时间占整个周期的百分比
  - 100% = 最亮 | 50% = 半亮 | 0% = 熄灭
- 人眼因视觉暂留效应看到的是平均亮度变化

## ESP32 PWM 新版API
- `ledcAttach(pin, freq, resolution)` - 绑定引脚到PWM通道
- `ledcWrite(pin, duty)` - 设置占空比值

## 代码参数
- freq = 5000 (PWM频率5000Hz)
- resolution = 8 (8位分辨率，占空比范围0-255)
- delay(10) (控制呼吸速度)

## 实验现象
LED缓慢变亮再缓慢变暗，循环往复形成呼吸效果

## 作者: 陈先炼
## 实验日期: 2026-06-20
