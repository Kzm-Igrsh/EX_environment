#include <M5Unified.h>

// ==========================================
// AtomS3 Lite ピン設定
// ==========================================
const int MOTOR1_PIN = 5;  // G5
const int MOTOR2_PIN = 6;  // G6
const int MOTOR3_PIN = 7;  // G7
const int MOTOR4_PIN = 8;  // G8

// ==========================================
// PWM設定
// ==========================================
const int PWM_FREQ = 1000;   // 1kHz
const int PWM_RES  = 8;      // 8bit (0-255)

// チャンネル割り当て
const int CH1 = 0;
const int CH2 = 1;
const int CH3 = 2;
const int CH4 = 3;

// 状態管理
bool isRunning = false;

// ==========================================
// 4モーター同時制御
// ==========================================
void setAllMotors(int speed) {
  speed = constrain(speed, 0, 255);
  
  ledcWrite(CH1, speed);
  ledcWrite(CH2, speed);
  ledcWrite(CH3, speed);
  ledcWrite(CH4, speed);

  Serial.printf("4 Motors: %d/255\n", speed);
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  Serial.begin(115200);
  delay(500);

  Serial.println("=== 4x N20 Motor @ 6000RPM ===");
  Serial.println("Press Button: Start/Stop");

  // PWM初期化（4チャンネル同時）
  ledcSetup(CH1, PWM_FREQ, PWM_RES);
  ledcSetup(CH2, PWM_FREQ, PWM_RES);
  ledcSetup(CH3, PWM_FREQ, PWM_RES);
  ledcSetup(CH4, PWM_FREQ, PWM_RES);

  // ピン割り当て
  ledcAttachPin(MOTOR1_PIN, CH1);
  ledcAttachPin(MOTOR2_PIN, CH2);
  ledcAttachPin(MOTOR3_PIN, CH3);
  ledcAttachPin(MOTOR4_PIN, CH4);

  // 停止状態で開始
  setAllMotors(0);
}

void loop() {
  M5.update();
  
  // ボタン押下でON/OFF
  if (M5.BtnA.wasPressed()) {
    isRunning = !isRunning;
    
    if (isRunning) {
      Serial.println("START");
      setAllMotors(255); // 最大出力（6000rpm想定）
    } else {
      Serial.println("STOP");
      setAllMotors(0);
    }
  }
  
  delay(10);
}