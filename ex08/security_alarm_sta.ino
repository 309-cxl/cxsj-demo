/*
  实验8：物联网安防报警器（STA模式 - 连接路由器）
  功能：Web页面布防/撤防，触摸传感器触发报警
  硬件：ESP32开发板 + LED(GPIO2) + 触摸引脚(GPIO4)
  使用：ESP32连接路由器，手机/电脑访问网页控制
*/

#include <WiFi.h>
#include <WebServer.h>

// ===== WiFi配置 - 修改为你的WiFi信息 =====
const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

// ===== 硬件引脚定义 =====
const int TOUCH_PIN = T0;     // 触摸引脚0 (GPIO4)
const int LED_PIN = 2;         // 内置LED引脚

// ===== 状态变量 =====
bool isArmed = false;          // 布防状态
bool isAlarming = false;       // 报警状态
int touchThreshold = 40;       // 触摸阈值

// ===== Web服务器 =====
WebServer server(80);

// ===== HTML网页内容 =====
const char* htmlPage = R"(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 安防报警器</title>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 0;
      padding: 20px;
      background: linear-gradient(135deg, #667eea 0%, #764ba 100%);
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
    .status {
      font-size: 20px;
      margin: 20px 0;
      padding: 15px;
      border-radius: 8px;
      font-weight: bold;
    }
    .status-disarmed {
      background: #d4edda;
      color: #155724;
      border: 1px solid #c3e6cb;
    }
    .status-armed {
      background: #fff3cd;
      color: #856404;
      border: 1px solid #ffeaa7;
    }
    .status-alarming {
      background: #f8d7da;
      color: #721c24;
      border: 1px solid #f5c6cb;
      animation: blink 0.5s infinite;
    }
    @keyframes blink {
      0%, 100% { opacity: 1; }
      50% { opacity: 0.5; }
    }
    button {
      padding: 15px 40px;
      font-size: 18px;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      margin: 10px;
      transition: all 0.3s;
      font-weight: bold;
    }
    .btn-arm {
      background: #28a745;
      color: white;
    }
    .btn-arm:hover:not(:disabled) {
      background: #218838;
      transform: translateY(-2px);
    }
    .btn-disarm {
      background: #dc3545;
      color: white;
    }
    .btn-disarm:hover:not(:disabled) {
      background: #c82333;
      transform: translateY(-2px);
    }
    button:disabled {
      background: #ccc;
      cursor: not-allowed;
      transform: none;
    }
    .info {
      margin-top: 20px;
      font-size: 14px;
      color: #999;
    }
    .log {
      margin-top: 20px;
      padding: 10px;
      background: #f8f9fa;
      border-radius: 5px;
      font-size: 12px;
      color: #666;
      max-height: 150px;
      overflow-y: auto;
      text-align: left;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>🚨 ESP32 安防报警器</h1>
    <p class="subtitle">物联网安防系统模拟实验</p>
    
    <div class="status {{STATUS_CLASS}}" id="statusDisplay">
      {{STATUS_TEXT}}
    </div>
    
    <button class="btn-arm" id="armBtn" onclick="arm()">🛡️ 布防 (Arm)</button>
    <button class="btn-disarm" id="disarmBtn" onclick="disarm()">🔓 撤防 (Disarm)</button>
    
    <div class="log" id="log">
      <strong>系统日志:</strong><br>
      <span id="logContent">系统启动...</span>
    </div>
    
    <div class="info">
      🌐 已连接WiFi路由器<br>
      👆 触摸GPIO4引脚触发报警
    </div>
  </div>

  <script>
    let logEntries = [];
    
    function addLog(message) {
      const timestamp = new Date().toLocaleTimeString();
      logEntries.unshift(`[${timestamp}] ${message}`);
      if (logEntries.length > 5) logEntries.pop();
      document.getElementById('logContent').innerHTML = logEntries.join("<br>");
    }
    
    function updateStatus() {
      fetch("/getStatus")
        .then(response => response.json())
        .then(data => {
          const statusDisplay = document.getElementById("statusDisplay");
          const armBtn = document.getElementById("armBtn");
          const disarmBtn = document.getElementById("disarmBtn");
          
          if (data.alarming) {
            statusDisplay.className = "status status-alarming";
            statusDisplay.textContent = "🚨 报警中！检测到入侵！";
            armBtn.disabled = true;
            disarmBtn.disabled = false;
            addLog("触发报警！");
          } else if (data.armed) {
            statusDisplay.className = "status status-armed";
            statusDisplay.textContent = "🛡️ 已布防 - 监控中";
            armBtn.disabled = true;
            disarmBtn.disabled = false;
            addLog("系统已布防");
          } else {
            statusDisplay.className = "status status-disarmed";
            statusDisplay.textContent = "✅ 已撤防 - 系统安全";
            armBtn.disabled = false;
            disarmBtn.disabled = true;
            addLog("系统已撤防");
          }
        });
    }
    
    function arm() {
      fetch("/arm")
        .then(response => response.text())
        .then(data => {
          addLog("执行布防...");
          setTimeout(updateStatus, 500);
        });
    }
    
    function disarm() {
      fetch("/disarm")
        .then(response => response.text())
        .then(data => {
          addLog("执行撤防...");
          setTimeout(updateStatus, 500);
        });
    }
    
    // 每2秒自动更新状态
    setInterval(updateStatus, 2000);
    
    // 页面加载时获取状态
    window.addEventListener("load", updateStatus);
  </script>
</body>
</html>
)";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // 配置引脚
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
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
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.on("/getStatus", handleGetStatus);
  
  // 启动服务器
  server.begin();
  Serial.println("Web服务器已启动！");
  Serial.println("在浏览器中打开上面的IP地址");
  Serial.println("触摸GPIO4引脚触发报警");
}

void loop() {
  server.handleClient();
  
  // 检查触摸传感器（如果已布防且未报警）
  if (isArmed && !isAlarming) {
    int touchValue = touchRead(TOUCH_PIN);
    
    if (touchValue < touchThreshold) {
      // 触发报警
      isAlarming = true;
      Serial.println("🚨 检测到触摸！触发报警！");
      Serial.println("需要通过网页撤防才能停止报警");
    }
  }
  
  // 控制LED
  if (isAlarming) {
    // 报警状态：LED快速闪烁
    digitalWrite(LED_PIN, (millis() / 100) % 2);
  } else if (isArmed) {
    // 布防状态：LED慢闪（提示已布防）
    digitalWrite(LED_PIN, (millis() / 500) % 2);
  } else {
    // 撤防状态：LED熄灭
    digitalWrite(LED_PIN, LOW);
  }
  
  delay(10);
}

// ===== Web服务器处理函数 =====

void handleRoot() {
  String page = String(htmlPage);
  
  // 替换状态占位符
  if (isAlarming) {
    page.replace("{{STATUS_CLASS}}", "status-alarming");
    page.replace("{{STATUS_TEXT}}", "🚨 报警中！检测到入侵！");
  } else if (isArmed) {
    page.replace("{{STATUS_CLASS}}", "status-armed");
    page.replace("{{STATUS_TEXT}}", "🛡️ 已布防 - 监控中");
  } else {
    page.replace("{{STATUS_CLASS}}", "status-disarmed");
    page.replace("{{STATUS_TEXT}}", "✅ 已撤防 - 系统安全");
  }
  
  server.send(200, "text/html; charset=UTF-8", page);
}

void handleArm() {
  if (!isArmed && !isAlarming) {
    isArmed = true;
    Serial.println("🛡️ 系统已布防");
    server.send(200, "text/plain", "OK: 已布防");
  } else {
    server.send(400, "text/plain", "Error: 请先撤防");
  }
}

void handleDisarm() {
  if (isArmed || isAlarming) {
    isArmed = false;
    isAlarming = false;
    digitalWrite(LED_PIN, LOW);
    Serial.println("🔓 系统已撤防");
    server.send(200, "text/plain", "OK: 已撤防");
  } else {
    server.send(400, "text/plain", "Error: 系统未布防");
  }
}

void handleGetStatus() {
  String json = "{";
  json += "\"armed\":" + String(isArmed ? "true" : "false") + ",";
  json += "\"alarming\":" + String(isAlarming ? "true" : "false");
  json += "}";
  
  server.send(200, "application/json", json);
}
