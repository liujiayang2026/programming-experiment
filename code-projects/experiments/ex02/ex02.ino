// 引脚25接LED
const int ledPin = 25;

// 时间变量
unsigned long previousMillis = 0;   // 上次LED状态更新时间
const long interval = 500;          // 闪烁间隔500ms（1Hz周期 = 亮500ms + 暗500ms）
int ledState = LOW;                  // LED当前状态

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);              // 可选，用于调试
}

void loop() {
  unsigned long currentMillis = millis();

  // 检查是否达到切换间隔
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // 保存上次更新时间

    // 翻转LED状态
    ledState = !ledState;
    digitalWrite(ledPin, ledState);

    // 可选：串口输出状态
    Serial.println(ledState ? "ON" : "OFF");
  }

  // 此处可以放置其他非阻塞任务
  // ...
}