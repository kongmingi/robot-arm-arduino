// 채널 확인: CH0~CH15를 하나씩 살짝 움직여 어느 관절이 몇 번 채널인지 찾기
// 시리얼 모니터(9600)에서 배터리 연결 후 'a' 입력하면 시작
// 결과: CH10 = 어깨, CH11 = 팔꿈치, CH13 = 손목, CH15 = 그리퍼
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);
const int CENTER = 307;   // 약 1.5ms = 약 90도
const int WIGGLE = 20;    // 약 10도만 살짝

void setup() {
  Serial.begin(9600);
  pca.begin();
  pca.setOscillatorFrequency(27000000);
  pca.setPWMFreq(50);
  delay(10);
  for (int ch = 0; ch < 16; ch++) pca.setPWM(ch, 0, 4096);  // 전 채널 신호 끔

  Serial.println("Connect battery, support the arm, then type a and press Enter");
  while (!Serial.available()) {}
  while (Serial.available()) Serial.read();

  for (int ch = 0; ch < 16; ch++) {
    Serial.print("CH");
    Serial.println(ch);
    pca.setPWM(ch, 0, CENTER);           delay(800);
    pca.setPWM(ch, 0, CENTER + WIGGLE);  delay(500);
    pca.setPWM(ch, 0, CENTER);           delay(500);
    pca.setPWM(ch, 0, 4096);             // 이 채널 신호 끔
    delay(1000);
  }
  Serial.println("Done");
}

void loop() {}
