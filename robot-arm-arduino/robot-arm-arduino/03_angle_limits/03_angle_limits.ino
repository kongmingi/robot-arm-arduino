// 최대·최소 각도 설정: 시리얼로 각도를 입력하면 1도씩 천천히 이동
// 안전 범위 밖 값은 무시, 'x' 입력 시 신호 끔(비상 정지)
//
// 측정 결과 (2026-11-01, SG90, AA 4개 약 6V)
//   CH15 그리퍼 : 100(닫힘) ~ 150(열림), 숫자 ↑ = 열림
//   CH13 손목   : 5 ~ 110, 숫자 ↓ = 아래
//   CH11 팔꿈치 : 이동 시 기어 갈리는 소리 → 측정 중단 (토크 부족 추정)
//   CH10 어깨   : 미측정
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);

const uint8_t CH = 15;       // 테스트할 채널
const int SAFE_MIN = 100;    // 관절별 측정값으로 설정
const int SAFE_MAX = 150;
const int STEP_DELAY = 20;   // 1도 이동마다 대기(ms)

const int PULSE_0   = 102;   // 0도   ≈ 0.5ms
const int PULSE_180 = 512;   // 180도 ≈ 2.5ms

int current = -1;            // 현재 각도 (-1 = 아직 모름)

int angleToPulse(int angle) {
  return map(angle, 0, 180, PULSE_0, PULSE_180);
}

void writeAngle(int angle) {
  pca.setPWM(CH, 0, angleToPulse(angle));
}

void moveSlowly(int target) {
  if (current < 0) {         // 첫 명령: 현재 위치를 모르니 바로 이동
    writeAngle(target);
    current = target;
    return;
  }
  int step = (target > current) ? 1 : -1;
  while (current != target) {
    current += step;
    writeAngle(current);
    delay(STEP_DELAY);
  }
}

void setup() {
  Serial.begin(9600);
  pca.begin();
  pca.setOscillatorFrequency(27000000);
  pca.setPWMFreq(50);
  delay(10);
  pca.setPWM(CH, 0, 4096);   // 시작은 신호 끔
  Serial.print("CH");
  Serial.print(CH);
  Serial.print(" ready. angle ");
  Serial.print(SAFE_MIN);
  Serial.print("~");
  Serial.print(SAFE_MAX);
  Serial.println(", x = signal off");
}

void loop() {
  if (!Serial.available()) return;
  String line = Serial.readStringUntil('\n');
  line.trim();
  if (line.length() == 0) return;

  if (line == "x") {
    pca.setPWM(CH, 0, 4096);
    current = -1;
    Serial.println("signal off");
    return;
  }

  int target = line.toInt();
  if (target < SAFE_MIN || target > SAFE_MAX) {
    Serial.println("Out of safe range. Ignored.");
    return;
  }
  moveSlowly(target);
  Serial.print("-> ");
  Serial.println(current);
}
