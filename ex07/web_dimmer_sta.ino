/*
  实验7：Web网页端无极调光器（STA模式 - 连接路由器）
  功能：通过网页滑动条实时控制LED亮度
  硬件：ESP32开发板 + LED(GPIO2)
  使用：ESP32连接路由器，手机/电脑访问网页控制
*/

#include <WiFi.h>
#include <WebServer.h>

// WiFi配置 - 修改为你的WiFi信息
const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

// LED引脚
const int LED_PIN = 2;

// PWM参数
const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8;  // 8位分辨率（0-255）

// Web服务器
WebServer server(80);

// 当前亮度
int brightness = 0;

// HTML网页内容
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
      margin: 50px;
      background: #f0f0f0;
    }
    .container {
      background: white;
      padding: 40px;
      border-radius: 10px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.1);
      max-width: 600px;
      margin: 0 auto;
    }
    h1 {
      color: #333;
    }
    .slider {
      width: 80%;
      height: 25px;
      margin: 30px 0;
      background: #ddd;
      outline: none;
      opacity: 0.7;
      -webkit-transition: .2s;
      transition: opacity .2s;
      border-radius: 12px;
    }
    .slider:hover {
      opacity: 1;
    }
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 35px;
      height: 35px;
      background: #4CAF50;
      cursor: pointer;
      border-radius: 50%;
    }
    .value-display {
      font-size: 24px;
      font-weight: bold;
      color: #4CAF50;
      margin: 20px 0;
    }
    .status {
      margin-top: 20px;
      color: #666;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>🎚️ ESP32 LED 无极调光器</h1>
    <p>拖动滑动条调整LED亮度</p>
    
    <input type="range" min="0" max="255" value="0" class="slider" id="brightnessSlider">
    
    <div class="value-display">
      当前亮度: <span id="brightnessValue">0</span>
    </div>
    
    <div class="status" id="status">
      LED熄灭
    </div>
  </div>

  <script>
    const slider = document.getElementById('brightnessSlider');
    const valueDisplay = document.getElementById('brightnessValue');
    const statusDisplay = document.getElementById('status');
    
    let isUpdating = false;
    
    // 监听滑动条变化
    slider.addEventListener('input', function() {
      const value = this.value;
      valueDisplay.textContent = value;
      
      // 发送请求到ESP32
      updateLED(value);
    });
    
    function updateLED(value) {
      if (isUpdating) return;  // 防止重复请求
      isUpdating = true;
      
      fetch('/setLED?value=' + value)
        .then(response => response.text())
        .then(data => {
          console.log('LED亮度已更新:', data);
          statusDisplay.textContent = 'LED亮度: ' + value;
          isUpdating = false;
        })
        .catch(error => {
          console.error('Error:', error);
          statusDisplay.textContent = '连接失败，请检查网络';
          isUpdating = false;
        });
    }
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
  
  // 连接WiFi
  Serial.print("正在连接WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi连接成功！");
  Serial.print("ESP32 IP地址: ");
  Serial.println(WiFi.localIP());
  
  // 配置Web服务器路由
  server.on("/", handleRoot);
  server.on("/setLED", handleLED);
  
  // 启动服务器
  server.begin();
  Serial.println("Web服务器已启动！");
  Serial.println("在浏览器中打开上面的IP地址");
}

void loop() {
  server.handleClient();
}

// 处理根路径请求 - 返回HTML页面
void handleRoot() {
  server.send(200, "text/html", htmlPage);
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
    
    server.send(200, "text/plain", "OK: " + String(brightness));
  } else {
    server.send(400, "text/plain", "Error: 缺少value参数");
  }
}
