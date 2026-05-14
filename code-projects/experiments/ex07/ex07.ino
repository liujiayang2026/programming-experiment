#include <WiFi.h>
#include <WebServer.h>

// ========== 网络配置 ==========
const char* ssid = "Desolate";
const char* password = "12345678";

// ========== 硬件配置 ==========
const int LED_PIN = 2;          // GPIO2 控制LED（大多数ESP32开发板板载LED）
const int PWM_CHANNEL = 0;      // 使用LEDC通道0
const int PWM_FREQ = 5000;      // PWM频率5kHz
const int PWM_RESOLUTION = 8;   // 8位分辨率，占空比0~255

// 全局亮度值 (0-255)
int currentBrightness = 0;

WebServer server(80);

// ========== 生成网页 ==========
String makePage() {
  // 将当前亮度值嵌入HTML，使滑动条初始位置与LED实际亮度同步
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=yes">
  <title>ESP32 无极调光器</title>
  <style>
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      text-align: center;
      margin-top: 60px;
      background-color: #f5f7fa;
      color: #333;
    }
    .container {
      background: white;
      max-width: 500px;
      margin: 0 auto;
      padding: 30px 20px 40px;
      border-radius: 32px;
      box-shadow: 0 10px 25px rgba(0,0,0,0.05);
    }
    h1 {
      font-size: 1.8rem;
      color: #2c3e50;
      margin-bottom: 0.5rem;
    }
    .sub {
      color: #7f8c8d;
      margin-bottom: 2rem;
      font-size: 0.9rem;
    }
    .brightness-value {
      font-size: 2.2rem;
      font-weight: bold;
      margin: 20px 0 10px;
      color: #e67e22;
    }
    input[type="range"] {
      width: 85%;
      height: 8px;
      -webkit-appearance: none;
      background: #ddd;
      border-radius: 5px;
      outline: none;
      margin: 20px 0;
    }
    input[type="range"]:focus {
      outline: none;
    }
    input[type="range"]::-webkit-slider-thumb {
      -webkit-appearance: none;
      width: 24px;
      height: 24px;
      border-radius: 50%;
      background: #e67e22;
      cursor: pointer;
      box-shadow: 0 2px 6px rgba(0,0,0,0.2);
      border: none;
    }
    .btn-group {
      display: flex;
      justify-content: center;
      gap: 20px;
      margin-top: 20px;
    }
    button {
      background-color: #3498db;
      border: none;
      color: white;
      padding: 12px 28px;
      font-size: 1rem;
      border-radius: 40px;
      cursor: pointer;
      transition: 0.2s;
      font-weight: bold;
      box-shadow: 0 2px 5px rgba(0,0,0,0.1);
    }
    button:hover {
      background-color: #2980b9;
      transform: scale(1.02);
    }
    button:active {
      transform: scale(0.98);
    }
    .off-btn {
      background-color: #95a5a6;
    }
    .off-btn:hover {
      background-color: #7f8c8d;
    }
    footer {
      margin-top: 35px;
      font-size: 0.75rem;
      color: #bdc3c7;
    }
  </style>
</head>
<body>
<div class="container">
  <h1>💡 无极调光器</h1>
  <div class="sub">PWM 平滑调光 · 实时响应</div>
  
  <div class="brightness-value">
    <span id="brightnessDisplay">)rawliteral" + String(currentBrightness) + R"rawliteral(</span>
    <span style="font-size:1rem;"> / 255</span>
  </div>
  
  <input type="range" id="brightnessSlider" min="0" max="255" value=")rawliteral" + String(currentBrightness) + R"rawliteral(">
  
  <div class="btn-group">
    <button id="maxBtn">🔆 最亮</button>
    <button id="offBtn" class="off-btn">⚫ 熄灭</button>
  </div>
  <footer>拖动滑块 → LED 亮度实时变化</footer>
</div>

<script>
  const slider = document.getElementById('brightnessSlider');
  const display = document.getElementById('brightnessDisplay');
  
  // 发送亮度值到ESP32
  function setBrightness(value) {
    // 更新界面显示（立即反馈）
    display.innerText = value;
    slider.value = value;
    
    // 发送 GET 请求到 /set?value=xxx
    fetch('/set?value=' + value)
      .then(response => {
        if (!response.ok) {
          console.warn('服务器响应异常');
        }
      })
      .catch(err => console.error('请求失败:', err));
  }
  
  // 监听滑块拖动（实时）
  slider.addEventListener('input', function(e) {
    const val = parseInt(e.target.value, 10);
    display.innerText = val;
    setBrightness(val);
  });
  
  // 最亮按钮
  document.getElementById('maxBtn').addEventListener('click', function() {
    setBrightness(255);
  });
  
  // 熄灭按钮
  document.getElementById('offBtn').addEventListener('click', function() {
    setBrightness(0);
  });
</script>
</body>
</html>
)rawliteral";
  return html;
}

// ========== 处理网页根请求 ==========
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

// ========== 处理调光请求 ==========
void handleSetBrightness() {
  if (server.hasArg("value")) {
    String valueStr = server.arg("value");
    int brightness = valueStr.toInt();
    // 限制范围 0-255
    if (brightness < 0) brightness = 0;
    if (brightness > 255) brightness = 255;
    
    // 更新全局亮度并应用到PWM
    currentBrightness = brightness;
    ledcWrite(PWM_CHANNEL, currentBrightness);
    
    // 可选：串口打印调试信息
    Serial.print("亮度设置为: ");
    Serial.println(currentBrightness);
    
    // 返回成功响应（无刷新，纯API）
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing 'value' parameter");
  }
}

// ========== 初始化PWM ==========
void initPWM() {
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(LED_PIN, PWM_CHANNEL);
  ledcWrite(PWM_CHANNEL, 0);   // 初始熄灭
}

// ========== 连接WiFi ==========
void connectWiFi() {
  Serial.print("正在连接WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi连接成功");
  Serial.print("🌐 访问地址: http://");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  
  // 初始化PWM和LED引脚
  initPWM();
  
  // 连接网络
  connectWiFi();
  
  // 配置Web服务器路由
  server.on("/", handleRoot);
  server.on("/set", handleSetBrightness);   // 调光API
  
  server.begin();
  Serial.println("HTTP服务器已启动");
}

void loop() {
  server.handleClient();  // 处理客户端请求
}