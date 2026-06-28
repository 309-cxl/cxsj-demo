/*
  实验7：Web网页端无极调光器（AP模式 - ESP32自建热点）
  功能：通过网页滑动条实时控制LED亮度
  硬件：ESP32开发板 + LED(GPIO2)
  使用：手机/电脑连接ESP32热点，访问 http://192.168.4.1
*/

#include <WiFi.h>
#include <WebServer.h>

// AP模式配置
const char* ap_ssid = "ESP32_LED_Control";  // WiFi名称
const char* ap_password = "12345678";         // WiFi密码（至少8位）

// LED引脚
const int LED_PIN = 2;

// PWM参数
const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8;  // 8位分辨率（0-255）

// Web服务器
WebServer server(80);

// 当前亮度
int brightness = 0;

// HTML网页内容（嵌入式）
const char* htmlPage = R"(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 LED 调光器</title>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 0;
      padding: 20px;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      min-height: 100vh;
      display: flex;
      justify-content: center;
      align-items: center;
    }
    .container {
      background: white;
      padding: 40px;
      border-radius: 15px;
      box-shadow: 0 10px 30px rgba(0,0,0,0.3);
      max-width: 500px;
      width: 90%;
    }
    h1 {
      color: #333;
      margin-bottom: 10px;
    }
    .subtitle {
      color: #666;
      margin-bottom: 30px;
    }
    .slider {
      -webkit-appearance: none;
      width: 100%;
      height: 15px;
      border-radius: 10px;
      background: linear-gradient(to right, #667eea, #764ba2);
      outline: none;
      opacity: 0.9;
      transition: opacity .2s;
      margin: 30px 0;
    }
    .slider:hover {
      opacity: 1;
    }
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 35px;
      height: 35px;
      border-radius: 50%;
      background: #4CAF50;
      cursor: pointer;
      box-shadow: 0 2px 5px rgba(0,0,0,0.3);
    }
    .value-display {
      font-size: 48px;
      font-weight: bold;
      color: #667eea;
      margin: 30px 0;
    }
    .status {
      margin-top: 20px;
      padding: 10px;
      background: #f0f0f0;
      border-radius: 5px;
      color: #666;
    }
    .info {
      margin-top: 20px;
      font-size: 14px;
      color: #999;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>🎚️ ESP32 LED 无极调光器</h1>
    <p class="subtitle">拖动滑动条调整LED亮度（0-255）</p>
    
    <input type="range" min="0" max="255" value="0" class="slider" id="brightnessSlider">
    
    <div class="value-display">
      <span id="brightnessValue">0</span>
    </div>
    
    <div class="status" id="status">
      💡 LED熄灭
    </div>
    
    <div class="info">
      📶 已连接ESP32热点<br>
      🌐 IP地址: 192.168.4.1
    </div>
  </div>

  <script>
    const slider = document.getElementById('brightnessSlider');
    const valueDisplay = document.getElementById('brightnessValue');
    const statusDisplay = document.getElementById('status');
    
    let updateTimeout = null;
    
    // 监听滑动条变化
    slider.addEventListener('input', function() {
      const value = this.value;
      valueDisplay.textContent = value;
      
      // 防抖：延迟200ms发送请求（避免刷屏）
      clearTimeout(updateTimeout);
      updateTimeout = setTimeout(() => {
        updateLED(value);
      }, 200);
    });
    
    function updateLED(value) {
      fetch('/setLED?value=' + value)
        .then(response => response.text())
        .then(data => {
          console.log('LED亮度已更新:', data);
          
          // 更新状态显示
          const numValue = parseInt(value);
          if (numValue === 0) {
            statusDisplay.textContent = '💡 LED熄灭';
          } else if (numValue < 85) {
            statusDisplay.textContent = '💡 LED微亮';
          } else if (numValue < 170) {
            statusDisplay.textContent = '💡 LED中等亮度';
          } else {
            statusDisplay.textContent = '💡 LED最亮';
          }
        })
        .catch(error => {
          console.error('Error:', error);
          statusDisplay.textContent = '❌ 连接失败，请检查网络';
        });
    }
    
    // 页面加载时获取当前亮度
    window.addEventListener('load', function() {
      fetch('/getLED')
        .then(response => response.text())
        .then(value => {
          slider.value = value;
          valueDisplay.textContent = value;
        });
    });
  </script>
</body>
</html>
)";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // 配置PWM
  ledcAttach(LED_PIN, PWM_FREQ, PWM_RESOLUTION);
  ledcWrite(LED_PIN, 0);  // 初始熄灭
  
  // 创建AP热点
  Serial.println("正在创建WiFi热点...");
  WiFi.softAP(ap_ssid, ap_password);
  
  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP IP地址: ");
  Serial.println(apIP);
  Serial.print("WiFi名称: ");
  Serial.println(ap_ssid);
  Serial.print("WiFi密码: ");
  Serial.println(ap_password);
  
  // 配置Web服务器路由
  server.on("/", handleRoot);
  server.on("/setLED", handleLED);
  server.on("/getLED", handleGetLED);
  
  // 启动服务器
  server.begin();
  Serial.println("Web服务器已启动！");
  Serial.println("手机/电脑连接WiFi后，浏览器访问: http://192.168.4.1");
}

void loop() {
  server.handleClient();
}

// 处理根路径请求 - 返回HTML页面
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", htmlPage);
}

// 处理LED控制请求
void handleLED() {
  if (server.hasArg("value")) {
    brightness = server.arg("value").toInt();
    brightness = constrain(brightness, 0, 255);
    
    // 设置PWM输出
    ledcWrite(LED_PIN, brightness);
    
    Serial.print("LED亮度设置为: ");
    Serial.println(brightness);
    
    server.send(200, "text/plain", String(brightness));
  } else {
    server.send(400, "text/plain", "Error: 缺少value参数");
  }
}

// 获取当前LED亮度
void handleGetLED() {
  server.send(200, "text/plain", String(brightness));
}
