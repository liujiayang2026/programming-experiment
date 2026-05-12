// 定义LED引脚
#define LED_A_PIN 25   // 灯A引脚
#define LED_B_PIN 26   // 灯B引脚

// PWM通道配置（旧版API，兼容ESP32 Arduino 1.0.6）
const int pwmChannelA = 0;      // 灯A使用通道0
const int pwmChannelB = 1;      // 灯B使用通道1
const int pwmFreq = 5000;       // PWM频率 5kHz
const int pwmResolution = 8;    // 分辨率 8位 (0-255)

// 渐变参数
int dutyA = 0;          // 灯A当前占空比
int direction = 1;      // 1=增加，-1=减少
int step = 1;           // 每次变化步长
int delayTime = 10;     // 每步延时时间(ms)，控制渐变速度

void setup() {
  // 初始化串口（调试用）
  Serial.begin(115200);
  delay(1000);

  // 配置两个PWM通道
  ledcSetup(pwmChannelA, pwmFreq, pwmResolution);
  ledcSetup(pwmChannelB, pwmFreq, pwmResolution);
  
  // 将引脚绑定到对应的PWM通道
  ledcAttachPin(LED_A_PIN, pwmChannelA);
  ledcAttachPin(LED_B_PIN, pwmChannelB);

  // 初始占空比
  ledcWrite(pwmChannelA, dutyA);
  ledcWrite(pwmChannelB, 255 - dutyA);
  
  Serial.println("Two LEDs with opposite breathing started");
}

void loop() {
  // 更新灯A占空比（步进）
  dutyA += step * direction;

  // 边界判断：到达0或255时改变方向
  if (dutyA >= 255) {
    dutyA = 255;
    direction = -1;   // 开始减小
  } else if (dutyA <= 0) {
    dutyA = 0;
    direction = 1;    // 开始增加
  }

  // 灯B占空比 = 255 - 灯A占空比（实现反相）
  int dutyB = 255 - dutyA;

  // 输出PWM信号
  ledcWrite(pwmChannelA, dutyA);
  ledcWrite(pwmChannelB, dutyB);

  // 可选：打印占空比（每500ms打印一次，避免串口占用过多时间）
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 500) {
    Serial.print("Duty A: ");
    Serial.print(dutyA);
    Serial.print("\tDuty B: ");
    Serial.println(dutyB);
    lastPrint = millis();
  }

  // 延时控制渐变速度（调整此值改变呼吸快慢）
  delay(delayTime);
}