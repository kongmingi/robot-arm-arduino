// I2C 스캔: 아두이노가 PCA9685(0x40)를 찾는지 확인
// 배선: Uno A4 -> SDA, A5 -> SCL, 5V -> VCC, GND -> GND
#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("I2C scan start");

  int found = 0;
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    byte err = Wire.endTransmission();
    if (err == 0) {
      Serial.print("Found: 0x");
      if (addr < 16) Serial.print("0");
      Serial.println(addr, HEX);
      found++;
    }
  }
  if (found == 0) Serial.println("Nothing found");
  Serial.println("Done");
}

void loop() {}
