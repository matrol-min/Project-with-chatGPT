#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>

#define DHTPIN 2 // DHT11 데이터 핀
#define DHTTYPE DHT11 // DHT11 사용 정의
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C 주소는 0x27, 16x2 LCD 설정

void setup() {
  lcd.init(); // LCD 초기화
  lcd.backlight(); // LCD 백라이트 켜기
  dht.begin(); // DHT11 시작
}

void loop() {
  // 온도와 습도 읽기
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // 데이터 읽기 실패 체크
  if (isnan(h) || isnan(t)) {
    lcd.clear();
    lcd.print("Error reading");
    return;
  }

  // LCD에 온도와 습도 표시
  lcd.clear(); // 화면 지우기
  lcd.setCursor(0, 0); // 첫 번째 줄에 온도 표시
  lcd.print("Temp:");
  lcd.print(t);
  lcd.print("  C");

  lcd.setCursor(0, 1); // 두 번째 줄에 습도 표시
  lcd.print("Hum:  ");
  lcd.print(h);
  lcd.print(" p");
 
  delay(500); // 2초마다 측정
}
