// 定义触摸引脚 (T0对应GPIO4)
#define TOUCH_PIN 4
// 定义LED引脚 
#define LED_PIN 2
// 中断模式设置：0为轮询模式，1为中断模式（本例使用轮询，保留定义）
#define EXT_ISR_MODE 0

// 触摸阈值（可通过串口监视器观察调整）
int threshold = 70;

// 状态变量
bool ledState = false;        // LED当前状态
bool lastTouchState = false;  // 上一次触摸状态（true=触摸按下，false=未触摸）

// 防抖相关变量
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // 防抖延迟时间（毫秒）

void setup() {
  Serial.begin(115200);
  delay(1000); // 等待串口稳定

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState); // 初始状态：LED灭
}

void loop() {
  // 读取当前触摸值
  int touchValue = touchRead(TOUCH_PIN);
  // 根据阈值判断当前是否处于触摸状态（触摸时数值变小）
  bool currentTouchState = (touchValue < threshold);

  // 边缘检测：检测到触摸瞬间（上一次未触摸，当前触摸）
  if (currentTouchState && !lastTouchState) {
    // 记录本次触发的时间，用于防抖
    lastDebounceTime = millis();
    // 防抖：等待一段时间后再次确认触摸状态
    // 注意：此处使用阻塞式delay会停止其他任务，但简单实验可接受
    delay(debounceDelay);
    // 再次读取触摸值确认是否仍处于触摸状态
    int confirmValue = touchRead(TOUCH_PIN);
    if (confirmValue < threshold) {
      // 确认为有效触摸，翻转LED状态
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      Serial.println("Touch detected! LED toggled.");
    }
    // 等待一段时间，避免在防抖期间重复检测
    // 注意：简单防抖中，再次读取后无需额外等待，因为millis()已记录
  }

  // 更新上一次触摸状态，供下次循环使用
  lastTouchState = currentTouchState;

  // 可选：输出触摸值（降低输出频率以避免影响时序）
  static unsigned long lastPrintTime = 0;
  if (millis() - lastPrintTime > 500) { // 每500ms打印一次
    Serial.print("Touch Value: ");
    Serial.println(touchValue);
    lastPrintTime = millis();
  }

  
  delay(10);
}