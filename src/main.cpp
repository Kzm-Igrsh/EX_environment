#include <M5Unified.h>

// 4つのモーターピン
const int MOTOR1_PIN = 5;  // G5
const int MOTOR2_PIN = 6;  // G6
const int MOTOR3_PIN = 7;  // G7
const int MOTOR4_PIN = 8;  // G8

// PWMチャンネル設定
const int MOTOR1_CH = 0;
const int MOTOR2_CH = 1;
const int MOTOR3_CH = 2;
const int MOTOR4_CH = 3;

// PWM設定（参考コードと同じ）
const int PWM_FREQ = 191;   // 200Hz
const int PWM_RES = 8;      // 8bit (0-255)

// 出力強度設定（参考コードと同じ）
const int POWER_STOP = 0;
const int POWER_WEAK = 80;     // 弱い
const int POWER_STRONG = 191;  // 強い

bool isRunning = false;

// 全モーター同時制御関数（4つ対応）
void setAllMotors(int power) {
  // 範囲制限
  if (power < 0) power = 0;
  if (power > 255) power = 255;

  ledcWrite(MOTOR1_CH, power);
  ledcWrite(MOTOR2_CH, power);
  ledcWrite(MOTOR3_CH, power);
  ledcWrite(MOTOR4_CH, power);
  
  Serial.printf("All Motors Set to: %d\n", power);
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  // LEDを青にする（起動中）
  M5.Display.fillScreen(BLUE);
  
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n4 DC Motor Control (AtomS3 Lite)");
  Serial.println("==================================");

  // 4つのモーターのPWM初期化
  Serial.printf("Init Motor1: Pin=%d, Ch=%d\n", MOTOR1_PIN, MOTOR1_CH);
  ledcSetup(MOTOR1_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(MOTOR1_PIN, MOTOR1_CH);
  ledcWrite(MOTOR1_CH, 0);
  
  Serial.printf("Init Motor2: Pin=%d, Ch=%d\n", MOTOR2_PIN, MOTOR2_CH);
  ledcSetup(MOTOR2_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(MOTOR2_PIN, MOTOR2_CH);
  ledcWrite(MOTOR2_CH, 0);
  
  Serial.printf("Init Motor3: Pin=%d, Ch=%d\n", MOTOR3_PIN, MOTOR3_CH);
  ledcSetup(MOTOR3_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(MOTOR3_PIN, MOTOR3_CH);
  ledcWrite(MOTOR3_CH, 0);
  
  Serial.printf("Init Motor4: Pin=%d, Ch=%d\n", MOTOR4_PIN, MOTOR4_CH);
  ledcSetup(MOTOR4_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(MOTOR4_PIN, MOTOR4_CH);
  ledcWrite(MOTOR4_CH, 0);
  
  Serial.println("PWM Init Complete");
  Serial.println("\nPress button to Start/Stop (All motors simultaneously)\n");
  
  // 準備完了したら赤（停止中）にする
  M5.Display.fillScreen(RED);
}

void loop() {
  M5.update();
  
  // ボタン押下：4つ同時にON/OFF切り替え
  if (M5.BtnA.wasPressed() || M5.BtnPWR.wasPressed()) {
    isRunning = !isRunning;
    
    if (isRunning) {
      Serial.println(">>> START (All 4 Motors) <<<");
      M5.Display.fillScreen(GREEN); // 緑：動作中
      setAllMotors(POWER_STRONG);   // 全モーターSTRONG(200)で回転
    } else {
      Serial.println(">>> STOP <<<");
      M5.Display.fillScreen(RED);   // 赤：停止
      setAllMotors(POWER_STOP);     // 停止
    }
  }
  
  delay(10);
}