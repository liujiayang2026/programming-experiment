// 引脚定义
const int ledPinFreq = 2;
const int ledPinSOS = 25;

// 频闪LED时间参数（毫秒）
const unsigned long freqInterval = 1000;

// SOS信号时间参数（毫秒）
const unsigned shortBlinkOn = 200;
const unsigned shortBlinkOff = 200;
const unsigned longBlinkOn = 600;
const unsigned longBlinkOff = 200;
const unsigned letterGap = 500;
const unsigned wordGap = 2000;

// 频闪LED状态
unsigned long prevFreqTime = 0;
bool freqLedState = LOW;

// SOS LED状态机（细化到每个亮和灭动作）
enum SOS_State {
  // 第一个S
  SOS_S1_ON_1, SOS_S1_OFF_1,
  SOS_S1_ON_2, SOS_S1_OFF_2,
  SOS_S1_ON_3, SOS_S1_OFF_3,
  // O
  SOS_O_ON_1, SOS_O_OFF_1,
  SOS_O_ON_2, SOS_O_OFF_2,
  SOS_O_ON_3, SOS_O_OFF_3,
  // 第二个S
  SOS_S2_ON_1, SOS_S2_OFF_1,
  SOS_S2_ON_2, SOS_S2_OFF_2,
  SOS_S2_ON_3, SOS_S2_OFF_3,
  // 字母间隔（S->O 和 O->S2）
  SOS_LETTER_GAP,
  SOS_LETTER_GAP2,    // <--- 添加缺失的状态
  // 单词间隔
  SOS_WORD_GAP
};

SOS_State sosState = SOS_S1_ON_1;
unsigned long prevSOSTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPinFreq, OUTPUT);
  pinMode(ledPinSOS, OUTPUT);
  digitalWrite(ledPinFreq, LOW);
  digitalWrite(ledPinSOS, LOW);
  prevSOSTime = millis();
}

void loop() {
  unsigned long now = millis();

  // ----- 频闪LED (引脚2) -----
  if (now - prevFreqTime >= freqInterval) {
    prevFreqTime = now;
    freqLedState = !freqLedState;
    digitalWrite(ledPinFreq, freqLedState);
  }

  // ----- SOS LED (引脚25) -----
  switch (sosState) {
    // ---------- 第一个S的三个短闪 ----------
    case SOS_S1_ON_1:
      digitalWrite(ledPinSOS, HIGH);
      prevSOSTime = now;
      sosState = SOS_S1_OFF_1;
      break;
    case SOS_S1_OFF_1:
      if (now - prevSOSTime >= shortBlinkOn) {
        digitalWrite(ledPinSOS, LOW);
        prevSOSTime = now;
        sosState = SOS_S1_ON_2;
      }
      break;

    case SOS_S1_ON_2:
      if (now - prevSOSTime >= shortBlinkOff) {
        digitalWrite(ledPinSOS, HIGH);
        prevSOSTime = now;
        sosState = SOS_S1_OFF_2;
      }
      break;
    case SOS_S1_OFF_2:
      if (now - prevSOSTime >= shortBlinkOn) {
        digitalWrite(ledPinSOS, LOW);
        prevSOSTime = now;
        sosState = SOS_S1_ON_3;
      }
      break;

    case SOS_S1_ON_3:
      if (now - prevSOSTime >= shortBlinkOff) {
        digitalWrite(ledPinSOS, HIGH);
        prevSOSTime = now;
        sosState = SOS_S1_OFF_3;
      }
      break;
    case SOS_S1_OFF_3:
      if (now - prevSOSTime >= shortBlinkOn) {
        digitalWrite(ledPinSOS, LOW);
        prevSOSTime = now;
        sosState = SOS_LETTER_GAP;  // 进入第一个字母间隔（S->O）
      }
      break;

    // ---------- 字母间隔 S->O ----------
    case SOS_LETTER_GAP:
      if (now - prevSOSTime >= letterGap) {
        sosState = SOS_O_ON_1;
        prevSOSTime = now;
      }
      break;

    // ---------- O的三个长闪 ----------
    case SOS_O_ON_1:
      digitalWrite(ledPinSOS, HIGH);
      prevSOSTime = now;
      sosState = SOS_O_OFF_1;
      break;
    case SOS_O_OFF_1:
      if (now - prevSOSTime >= longBlinkOn) {
        digitalWrite(ledPinSOS, LOW);
        prevSOSTime = now;
        sosState = SOS_O_ON_2;
      }
      break;

    case SOS_O_ON_2:
      if (now - prevSOSTime >= longBlinkOff) {
        digitalWrite(ledPinSOS, HIGH);
        prevSOSTime = now;
        sosState = SOS_O_OFF_2;
      }
      break;
    case SOS_O_OFF_2:
      if (now - prevSOSTime >= longBlinkOn) {
        digitalWrite(ledPinSOS, LOW);
        prevSOSTime = now;
        sosState = SOS_O_ON_3;
      }
      break;

    case SOS_O_ON_3:
      if (now - prevSOSTime >= longBlinkOff) {
        digitalWrite(ledPinSOS, HIGH);
        prevSOSTime = now;
        sosState = SOS_O_OFF_3;
      }
      break;
    case SOS_O_OFF_3:
      if (now - prevSOSTime >= longBlinkOn) {
        digitalWrite(ledPinSOS, LOW);
        prevSOSTime = now;
        sosState = SOS_LETTER_GAP2;  // 进入第二个字母间隔（O->S2）
      }
      break;

    // ---------- 字母间隔 O->S2 ----------
    case SOS_LETTER_GAP2:
      if (now - prevSOSTime >= letterGap) {
        sosState = SOS_S2_ON_1;
        prevSOSTime = now;
      }
      break;

    // ---------- 第二个S的三个短闪 ----------
    case SOS_S2_ON_1:
      digitalWrite(ledPinSOS, HIGH);
      prevSOSTime = now;
      sosState = SOS_S2_OFF_1;
      break;
    case SOS_S2_OFF_1:
      if (now - prevSOSTime >= shortBlinkOn) {
        digitalWrite(ledPinSOS, LOW);
        prevSOSTime = now;
        sosState = SOS_S2_ON_2;
      }
      break;

    case SOS_S2_ON_2:
      if (now - prevSOSTime >= shortBlinkOff) {
        digitalWrite(ledPinSOS, HIGH);
        prevSOSTime = now;
        sosState = SOS_S2_OFF_2;
      }
      break;
    case SOS_S2_OFF_2:
      if (now - prevSOSTime >= shortBlinkOn) {
        digitalWrite(ledPinSOS, LOW);
        prevSOSTime = now;
        sosState = SOS_S2_ON_3;
      }
      break;

    case SOS_S2_ON_3:
      if (now - prevSOSTime >= shortBlinkOff) {
        digitalWrite(ledPinSOS, HIGH);
        prevSOSTime = now;
        sosState = SOS_S2_OFF_3;
      }
      break;
    case SOS_S2_OFF_3:
      if (now - prevSOSTime >= shortBlinkOn) {
        digitalWrite(ledPinSOS, LOW);
        prevSOSTime = now;
        sosState = SOS_WORD_GAP;  // 进入单词间隔
      }
      break;

    // ---------- 单词间隔 ----------
    case SOS_WORD_GAP:
      if (now - prevSOSTime >= wordGap) {
        sosState = SOS_S1_ON_1;   // 重新开始新的一轮SOS
        prevSOSTime = now;
      }
      break;

    default:
      break;
  }
}