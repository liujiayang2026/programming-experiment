// SOS信号LED闪烁代码
// 定义LED引脚，ESP32通常板载LED连接在GPIO 2
const int ledPin = 2;

// 定义时间常量（单位：毫秒）
const int shortBlink = 200;   // 短闪点亮时间
const int shortGap   = 200;   // 短闪之间的熄灭时间
const int longBlink  = 600;   // 长闪点亮时间
const int longGap    = 200;   // 长闪之间的熄灭时间
const int letterGap  = 500;   // 字母之间的间隔
const int wordGap    = 2000;  // 单词之间的间隔（SOS重复间隔）

void setup() {
  // 初始化串口通信，设置波特率为115200
  Serial.begin(115200);
  // 将LED引脚设置为输出模式
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // --- 发送字母 S（三个短闪）---
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON (S)");
    delay(shortBlink);
    
    digitalWrite(ledPin, LOW);
    Serial.println("LED OFF (S)");
    delay(shortGap);
  }
  // 字母间间隔
  delay(letterGap);

  // --- 发送字母 O（三个长闪）---
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON (O)");
    delay(longBlink);
    
    digitalWrite(ledPin, LOW);
    Serial.println("LED OFF (O)");
    delay(longGap);
  }
  // 字母间间隔
  delay(letterGap);

  // --- 再次发送字母 S（三个短闪）---
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON (S)");
    delay(shortBlink);
    
    digitalWrite(ledPin, LOW);
    Serial.println("LED OFF (S)");
    delay(shortGap);
  }
  // 单词间间隔（完成后重复整个SOS序列）
  delay(wordGap);
}