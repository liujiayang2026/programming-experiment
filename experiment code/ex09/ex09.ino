#include <WiFi.h>
#include <WebServer.h>

// ========== 网络配置 ==========
const char* ssid = "Desolate";      // 修改为您的WiFi名称
const char* password = "12345678";  // 修改为您的WiFi密码

// ========== 硬件引脚定义 ==========
const int TOUCH_PIN = T7;    // 触摸引脚 D27 (GPIO27)

// ========== Web服务器 ==========
WebServer server(80);

// ========== 生成仪表盘HTML页面 ==========
String generateDashboard() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <title>ESP32 触摸传感器仪表盘</title>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
      user-select: none;
    }
    body {
      background: linear-gradient(145deg, #0b1120 0%, #19233c 100%);
      min-height: 100vh;
      display: flex;
      justify-content: center;
      align-items: center;
      font-family: 'Segoe UI', 'Poppins', 'Roboto', sans-serif;
      padding: 20px;
    }
    .card {
      background: rgba(18, 25, 45, 0.75);
      backdrop-filter: blur(8px);
      border-radius: 64px;
      box-shadow: 0 25px 45px rgba(0,0,0,0.3), inset 0 1px 1px rgba(255,255,255,0.1);
      padding: 2rem 2rem 3rem;
      text-align: center;
      width: 100%;
      max-width: 550px;
      border: 1px solid rgba(255,255,255,0.2);
      transition: all 0.3s ease;
    }
    h1 {
      color: #eef5ff;
      font-weight: 600;
      font-size: 1.8rem;
      letter-spacing: 1px;
      margin-bottom: 0.5rem;
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 12px;
    }
    h1::before {
      content: "📡";
      font-size: 2rem;
    }
    .sub {
      color: #8e9fc3;
      margin-bottom: 2rem;
      font-size: 0.9rem;
      border-bottom: 1px dashed #2e3a5c;
      display: inline-block;
      padding-bottom: 6px;
    }
    .sensor-panel {
      background: #0a0f1c;
      border-radius: 48px;
      padding: 30px 20px;
      margin: 20px 0;
      box-shadow: inset 0 2px 6px rgba(0,0,0,0.5), 0 8px 20px rgba(0,0,0,0.3);
    }
    .value-container {
      font-size: 6rem;
      font-weight: 800;
      font-family: 'Courier New', monospace;
      color: #7effb3;
      text-shadow: 0 0 8px #00ffa2;
      background: #00000055;
      border-radius: 60px;
      padding: 20px 10px;
      margin-bottom: 15px;
      letter-spacing: 4px;
    }
    #sensorValue {
      font-size: 5rem;
      transition: 0.1s;
    }
    .unit {
      font-size: 1.5rem;
      color: #bbd9ff;
    }
    .indicator {
      display: flex;
      justify-content: center;
      align-items: center;
      gap: 12px;
      margin: 15px 0;
    }
    .led {
      width: 20px;
      height: 20px;
      border-radius: 50%;
      background-color: #2ecc71;
      box-shadow: 0 0 6px #2ecc71;
      transition: 0.1s;
    }
    .status-text {
      font-size: 1.1rem;
      font-weight: 500;
      background: #1e2a3e;
      padding: 8px 20px;
      border-radius: 40px;
      display: inline-block;
      color: #ccdeff;
    }
    .progress-bar {
      width: 100%;
      height: 12px;
      background: #2a3752;
      border-radius: 20px;
      margin: 20px 0 10px;
      overflow: hidden;
    }
    .progress-fill {
      width: 0%;
      height: 100%;
      background: linear-gradient(90deg, #2ecc71, #f1c40f, #e67e22);
      border-radius: 20px;
      transition: width 0.1s linear;
    }
    .footer-note {
      font-size: 0.7rem;
      color: #5f739b;
      margin-top: 25px;
    }
    button {
      background: #2e3b4e;
      border: none;
      color: white;
      padding: 6px 14px;
      border-radius: 30px;
      font-size: 0.7rem;
      cursor: pointer;
      transition: 0.2s;
    }
    button:hover {
      background: #3e4d66;
    }
    @media (max-width: 500px) {
      .value-container { font-size: 3.5rem; }
      #sensorValue { font-size: 3rem; }
      .card { padding: 1.5rem; }
    }
  </style>
</head>
<body>
<div class="card">
  <h1>✨ 触摸仪表盘 ✨</h1>
  <div class="sub">D27 (GPIO27) 电容式传感器</div>
  
  <div class="sensor-panel">
    <div class="value-container">
      <span id="sensorValue">--</span><span class="unit"> &nbsp;raw</span>
    </div>
    <div class="indicator">
      <div class="led" id="touchLed"></div>
      <div class="status-text" id="touchStatus">等待数据...</div>
    </div>
    <div class="progress-bar">
      <div class="progress-fill" id="progressFill"></div>
    </div>
    <div style="font-size:0.8rem; margin-top: 8px;">
      👆 手指靠近 → 数值下降 &nbsp;&nbsp;|&nbsp;&nbsp; ✋ 远离 → 数值回升
    </div>
  </div>
  <div class="footer-note">
    🔄 实时更新 <span id="updateRate">50</span> ms &nbsp;|&nbsp; 
    <button onclick="toggleUpdate()">⏸️ 暂停</button>
  </div>
</div>

<script>
  let isUpdating = true;
  let intervalId = null;
  const updateIntervalMs = 80;  // 80ms 刷新一次，流畅显示
  
  // 获取元素
  const valueSpan = document.getElementById('sensorValue');
  const touchLed = document.getElementById('touchLed');
  const touchStatus = document.getElementById('touchStatus');
  const progressFill = document.getElementById('progressFill');
  const rateSpan = document.getElementById('updateRate');
  
  rateSpan.innerText = updateIntervalMs;
  
  // 更新UI函数
  function updateUI(rawValue) {
    // 显示数值
    valueSpan.innerText = rawValue;
    
    // 根据数值范围改变样式和状态文字
    let statusMsg = '';
    let ledColor = '#95a5a6';
    let progressPercent = 0;
    
    // 触摸值范围: 未触摸约60~80, 完全触摸可低至10以下
    if (rawValue > 60) {
      statusMsg = '🟢 未触摸';
      ledColor = '#2ecc71';
      progressPercent = 0;
    } else if (rawValue > 40) {
      statusMsg = '🟡 轻微靠近';
      ledColor = '#f1c40f';
      progressPercent = 20;
    } else if (rawValue > 20) {
      statusMsg = '🟠 接近中';
      ledColor = '#e67e22';
      progressPercent = 50;
    } else if (rawValue > 10) {
      statusMsg = '🔴 触摸！';
      ledColor = '#e74c3c';
      progressPercent = 80;
    } else {
      statusMsg = '💪 强触摸';
      ledColor = '#ff3860';
      progressPercent = 100;
    }
    
    touchStatus.innerText = statusMsg;
    touchLed.style.backgroundColor = ledColor;
    touchLed.style.boxShadow = `0 0 12px ${ledColor}`;
    progressFill.style.width = progressPercent + '%';
    
    // 数值越小区块颜色越红
    const intensity = Math.min(255, Math.max(0, 255 - rawValue * 3));
    valueSpan.style.color = `rgb(255, ${255 - intensity}, ${100 + intensity/2})`;
  }
  
  // 从ESP32获取实时触摸数值
  async function fetchTouchValue() {
    try {
      const response = await fetch('/read');
      if (!response.ok) throw new Error('网络错误');
      const data = await response.json();
      if (data.hasOwnProperty('touch')) {
        const rawVal = data.touch;
        updateUI(rawVal);
      }
    } catch (err) {
      console.warn('获取数据失败:', err);
      valueSpan.innerText = 'ERR';
      touchStatus.innerText = '⚠️ 连接中断';
    }
  }
  
  // 启动/停止轮询
  function startUpdate() {
    if (intervalId) clearInterval(intervalId);
    intervalId = setInterval(() => {
      if (isUpdating) fetchTouchValue();
    }, updateIntervalMs);
  }
  
  function toggleUpdate() {
    isUpdating = !isUpdating;
    const btn = document.querySelector('button');
    if (isUpdating) {
      btn.innerHTML = '⏸️ 暂停';
      btn.style.background = '#3e4d66';
      fetchTouchValue(); // 立即刷新一次
    } else {
      btn.innerHTML = '▶️ 启动';
      btn.style.background = '#2c3e50';
    }
  }
  
  // 页面加载后开始监控
  window.addEventListener('load', () => {
    startUpdate();
    fetchTouchValue(); // 立即获取
  });
  
  // 页面关闭时清除定时器（可选）
  window.addEventListener('beforeunload', () => {
    if (intervalId) clearInterval(intervalId);
  });
</script>
</body>
</html>
)rawliteral";
  return html;
}

// ========== 处理根请求：返回仪表盘页面 ==========
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", generateDashboard());
}

// ========== 处理AJAX请求：返回当前触摸数值（JSON格式）==========
void handleReadTouch() {
  int touchValue = touchRead(TOUCH_PIN);
  String json = "{\"touch\":" + String(touchValue) + "}";
  server.send(200, "application/json", json);
  
  // 可选：串口打印调试（高频率会刷屏，调试时可取消注释）
  // static unsigned long lastPrint = 0;
  // if (millis() - lastPrint > 500) {
  //   lastPrint = millis();
  //   Serial.printf("Touch value: %d\n", touchValue);
  // }
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
  Serial.print("📊 触摸仪表盘已启动，打开上述地址即可查看实时数据");
}

// ========== 初始化 ==========
void setup() {
  Serial.begin(115200);
  delay(100);
  
  // 触摸引脚无需初始化，直接使用touchRead即可
  
  // 连接WiFi
  connectWiFi();
  
  // 配置Web服务器路由
  server.on("/", handleRoot);           // 网页仪表盘
  server.on("/read", handleReadTouch);  // 数据API
  
  server.begin();
  Serial.println("HTTP服务器已启动");
}

// ========== 主循环 ==========
void loop() {
  server.handleClient();  // 处理所有HTTP请求
}