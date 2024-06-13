#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD 설정
LiquidCrystal_I2C lcd(0x27, 16, 2); // 주소, 열 수, 행 수

// 버튼 핀 설정
#define PLAYER1_INC_BUTTON_PIN 2 // 플레이어 1의 점수 증가 버튼 핀
#define PLAYER1_DEC_BUTTON_PIN 3 // 플레이어 1의 점수 감소 버튼 핀
#define PLAYER2_INC_BUTTON_PIN 12 // 플레이어 2의 점수 증가 버튼 핀
#define PLAYER2_DEC_BUTTON_PIN 13 // 플레이어 2의 점수 감소 버튼 핀

// 초기 점수
int player1Score = 0;
int player2Score = 0;

// 버튼을 누른 시간 저장 변수
unsigned long player1IncButtonPressedTime = 0;
unsigned long player1DecButtonPressedTime = 0;
unsigned long player2IncButtonPressedTime = 0;
unsigned long player2DecButtonPressedTime = 0;

// 버튼이 눌린 상태 변수
bool player1IncButtonState = false;
bool player1DecButtonState = false;
bool player2IncButtonState = false;
bool player2DecButtonState = false;

// 버튼이 눌린 상태를 확인하기 위한 딜레이
#define LONG_PRESS_DELAY 3000 // 3초
#define DEBOUNCE_DELAY 100 // 디바운스 딜레이

void setup() {
  // LCD 초기화
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); // LCD 커서 위치 설정
  lcd.print("Player 1:0"); // 초기화면 표시
  lcd.setCursor(0, 1);
  lcd.print("Player 2:0");

  // 버튼 핀 초기화
  pinMode(PLAYER1_INC_BUTTON_PIN, INPUT_PULLUP); // 내장 풀업 저항 사용
  pinMode(PLAYER1_DEC_BUTTON_PIN, INPUT_PULLUP); // 내장 풀업 저항 사용
  pinMode(PLAYER2_INC_BUTTON_PIN, INPUT_PULLUP); // 내장 풀업 저항 사용
  pinMode(PLAYER2_DEC_BUTTON_PIN, INPUT_PULLUP); // 내장 풀업 저항 사용
}

void loop() {
  // 버튼 상태 감지 및 처리
  checkButtonState(PLAYER1_INC_BUTTON_PIN, player1IncButtonState, player1IncButtonPressedTime, player1Score);
  checkButtonState(PLAYER1_DEC_BUTTON_PIN, player1DecButtonState, player1DecButtonPressedTime, player1Score);
  checkButtonState(PLAYER2_INC_BUTTON_PIN, player2IncButtonState, player2IncButtonPressedTime, player2Score);
  checkButtonState(PLAYER2_DEC_BUTTON_PIN, player2DecButtonState, player2DecButtonPressedTime, player2Score);
}

// 버튼 상태를 체크하고 딜레이를 확인하는 함수
void checkButtonState(int buttonPin, bool &buttonState, unsigned long &pressedTime, int &playerScore) {
  bool state = digitalRead(buttonPin);

  if (state != buttonState) {
    if (state == LOW) {
      if (millis() - pressedTime > DEBOUNCE_DELAY) { // 디바운스 딜레이 확인
        pressedTime = millis(); // 버튼을 누른 시간 기록
        if (millis() - pressedTime > LONG_PRESS_DELAY) {
          playerScore = 0; // 버튼이 일정 시간 이상 눌려있으면 점수 초기화
          displayScore(); // 점수 표시
        } else {
          // 1초 이상 누르면 점수 증가/감소
          if (buttonPin == PLAYER1_INC_BUTTON_PIN || buttonPin == PLAYER2_INC_BUTTON_PIN) {
            playerScore++;
          } else {
            if (playerScore > 0) {
              playerScore--;
            }
          }
          displayScore(); // 점수 표시
        }
      }
    }
    buttonState = state;
  }
}

// 점수 표시 함수
void displayScore() {
  lcd.setCursor(9, 0); // 플레이어 1 점수 위치
  lcd.print("   "); // 이전 점수 지우기
  lcd.setCursor(9, 0);
  lcd.print(player1Score);

  lcd.setCursor(9, 1); // 플레이어 2 점수 위치
  lcd.print("   "); // 이전 점수 지우기
  lcd.setCursor(9, 1);
  lcd.print(player2Score);
}
