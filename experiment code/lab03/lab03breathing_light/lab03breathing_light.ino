// 定义LED引脚
const int ledPin = 2;  

// 设置PWM属性
const int freq = 5000;          // 频率 5000Hz
const int resolution = 8;       // 分辨率 8位 (0-255)
const int pwmChannel = 0;       // 选择一个PWM通道（0-15，ESP32共有16个通道）

void setup() {
  Serial.begin(115200);

  // 1.0.6版本用法：
  // 1. 设置通道的PWM参数
  ledcSetup(pwmChannel, freq, resolution);
  // 2. 将LED引脚附加到该通道
  ledcAttachPin(ledPin, pwmChannel);
}

void loop() {
  // 逐渐变亮（占空比从0到255）
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // 通过通道号写入占空比
    ledcWrite(pwmChannel, dutyCycle);   
    delay(10);
  }

  // 逐渐变暗（占空比从255到0）
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    ledcWrite(pwmChannel, dutyCycle);   
    delay(10);
  }
  
  Serial.println("Breathing cycle completed");
}