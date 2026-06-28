/*
  实验9：实时传感器Web仪表盘（AP模式）
  功能：Web页面实时显示触摸传感器数值
  硬件：ESP32开发板 + 触摸引脚(GPIO4)
  使用：手机/电脑连接ESP32热点，访问 http://192.168.4.1
  特点：双向通信 - 不仅下发控制，还上报传感器数据
*/

#include <WiFi.h>
#include <WebServer.h>

// ===== AP模式配置 =====
const char* ap_ssid = "ESP32_Sensor";   // WiFi名称
const char* ap_password = "12345678";     // WiFi密码（至少8位）

// ===== 硬件引脚定义 =====
const int TOUCH_PIN = T0;      // 触摸引脚0 (GPIO4)

// ===== Web服务器 =====
WebServer server(80);

// ===== 传感器数据 =====
int touchValue = 100;          // 当前触摸值
int minValue = 100;           // 最小值（用于校准显示）
int maxValue = 0;             // 最大值（用于校准显示）
bool isCalibrated = false;     // 是否已校准

// ===== HTML网页内容 =====
const char* htmlPage = R"(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 实时传感器仪表盘</title>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }
    
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      min-height: 100vh;
      display: flex;
      justify-content: center;
      align-items: center;
      padding: 20px;
    }
    
    .dashboard {
      background: white;
      border-radius: 20px;
      box-shadow: 0 20px 60px rgba(0,0,0,0.3);
      padding: 50px;
      max-width: 600px;
      width: 100%;
      text-align: center;
    }
    
    h1 {
      color: #333;
      margin-bottom: 10px;
      font-size: 28px;
    }
    
    .subtitle {
      color: #666;
      margin-bottom: 40px;
      font-size: 14px;
    }
    
    .sensor-display {
      margin: 40px 0;
      padding: 40px;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      border-radius: 15px;
      box-shadow: 0 10px 30px rgba(102, 126, 234, 0.4);
    }
    
    .value-label {
      color: rgba(255,255,255,0.8);
      font-size: 16px;
      margin-bottom: 10px;
      text-transform: uppercase;
      letter-spacing: 2px;
    }
    
    .value {
      color: white;
      font-size: 72px;
      font-weight: bold;
      text-shadow: 0 2px 10px rgba(0,0,0,0.2);
      transition: all 0.3s ease;
    }
    
    .value.small {
      font-size: 48px;
    }
    
    .unit {
      color: rgba(255,255,255,0.9);
      font-size: 20px;
      margin-left: 5px;
    }
    
    .status-bar {
      display: flex;
      justify-content: space-around;
      margin: 30px 0;
      padding: 20px;
      background: #f8f9fa;
      border-radius: 10px;
    }
    
    .status-item {
      text-align: center;
    }
    
    .status-label {
      color: #999;
      font-size: 12px;
      margin-bottom: 5px;
    }
    
    .status-value {
      color: #667eea;
      font-size: 20px;
      font-weight: bold;
    }
    
    .gauge {
      margin: 30px 0;
      height: 10px;
      background: #e0e0e0;
      border-radius: 5px;
      overflow: hidden;
      position: relative;
    }
    
    .gauge-fill {
      height: 100%;
      background: linear-gradient(90deg, #667eea 0%, #764ba2 100%);
      border-radius: 5px;
      transition: width 0.3s ease;
      width: 0%;
    }
    
    .info {
      margin-top: 30px;
      padding: 15px;
      background: #fff3cd;
      border-left: 4px solid #ffc107;
      border-radius: 5px;
      font-size: 14px;
      color: #856404;
      text-align: left;
    }
    
    .connection-status {
      display: inline-block;
      padding: 5px 15px;
      border-radius: 20px;
      font-size: 12px;
      font-weight: bold;
      margin-top: 20px;
    }
    
    .connected {
      background: #d4edda;
      color: #155724;
    }
    
    .disconnected {
      background: #f8d7da;
      color: #721c24;
    }
  </style>
</head>
<body>
  <div class="dashboard">
    <h1>📊 实时传感器仪表盘</h1>
    <p class="subtitle">触摸传感器数据实时监控</p>
    
    <div class="sensor-display">
      <div class="value-label">触摸传感器数值</div>
      <div>
        <span class="value" id="sensorValue">--</span>
        <span class="unit">单位</span>
      </div>
    </div>
    
    <div class="gauge">
      <div class="gauge-fill" id="gaugeFill"></div>
    </div>
    
    <div class="status-bar">
      <div class="status-item">
        <div class="status-label">最小值</div>
        <div class="status-value" id="minValue">--</div>
      </div>
      <div class="status-item">
        <div class="status-label">最大值</div>
        <div class="status-value" id="maxValue">--</div>
      </div>
      <div class="status-item">
        <div class="status-label">更新频率</div>
        <div class="status-value">200ms</div>
      </div>
    </div>
    
    <div class="info">
      💡 <strong>使用提示：</strong><br>
      • 手指靠近 GPIO4 引脚，数值会变小<br>
      • 手指离开，数值会恢复<br>
      • 数据每 200ms 自动刷新一次
    </div>
    
    <div class="connection-status connected" id="connStatus">
      ✅ 实时连接中...
    </div>
  </div>

  <script>
    let updateInterval = null;
    let errorCount = 0;
    
    // 更新传感器数据
    function updateSensorData() {
      fetch('/sensor')
        .then(response => response.json())
        .then(data => {
          // 更新显示
          document.getElementById('sensorValue').textContent = data.value;
          
          // 更新仪表条
          let percentage = (data.value / 100) * 100;
          percentage = Math.max(0, Math.min(100, percentage));
          document.getElementById('gaugeFill').style.width = percentage + '%';
          
          // 更新最值
          document.getElementById('minValue').textContent = data.min;
          document.getElementById('maxValue').textContent = data.max;
          
          // 根据数值调整显示样式
          const valueElem = document.getElementById('sensorValue');
          if (data.value < 30) {
            valueElem.classList.add('small');
          } else {
            valueElem.classList.remove('small');
          }
          
          // 连接状态
          errorCount = 0;
          document.getElementById('connStatus').className = 'connection-status connected';
          document.getElementById('connStatus').textContent = '✅ 实时连接中...';
        })
        .catch(error => {
          console.error('Error:', error);
          errorCount++;
          
          if (errorCount > 5) {
            document.getElementById('connStatus').className = 'connection-status disconnected';
            document.getElementById('connStatus').textContent = '❌ 连接断开，正在重试...';
          }
        });
    }
    
    // 页面加载时启动定时更新
    window.addEventListener('load', function() {
      // 立即更新一次
      updateSensorData();
      
      // 每200ms更新一次（5Hz）
      updateInterval = setInterval(updateSensorData, 200);
    });
    
    // 页面卸载时清除定时器
    window.addEventListener('beforeunload', function() {
      if (updateInterval) {
        clearInterval(updateInterval);
      }
    });
  </script>
</body>
</html>
)";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("========== 实时传感器Web仪表盘 ==========");
  
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
  server.on("/sensor", handleSensor);
  
  // 启动服务器
  server.begin();
  Serial.println("Web服务器已启动！");
  Serial.println("手机/电脑连接WiFi后，浏览器访问: http://192.168.4.1");
  Serial.println("==========================================\n");
}

void loop() {
  server.handleClient();
  
  // 读取传感器数据
  touchValue = touchRead(TOUCH_PIN);
  
  // 校准最值
  if (!isCalibrated) {
    minValue = touchValue;
    maxValue = touchValue;
    isCalibrated = true;
  } else {
    if (touchValue < minValue) minValue = touchValue;
    if (touchValue > maxValue) maxValue = touchValue;
  }
  
  // 串口输出（调试用）
  Serial.print("触摸值: ");
  Serial.print(touchValue);
  Serial.print(" | 范围: ");
  Serial.print(minValue);
  Serial.print(" - ");
  Serial.println(maxValue);
  
  delay(100);
}

// ===== Web服务器处理函数 =====

// 处理根路径请求 - 返回HTML页面
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", htmlPage);
}

// 处理传感器数据请求 - 返回JSON
void handleSensor() {
  String json = "{";
  json += "\"value\":" + String(touchValue) + ",";
  json += "\"min\":" + String(minValue) + ",";
  json += "\"max\":" + String(maxValue);
  json += "}";
  
  server.send(200, "application/json", json);
}
