#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD 설정
LiquidCrystal_I2C lcd(0x27, 16, 2); // 주소, 열 수, 행 수

// 버튼 핀 설정
#define PLAYER1_INC_BUTTON_PIN 2 // 플레이어 1의 점수 증가 버튼 핀
#define PLAYER1_DEC_BUTTON_PIN 3 // 플레이어 1의 점수 감소 버튼 핀
#define PLAYER2_INC_BUTTON_PIN 11 // 플레이어 2의 점수 증가 버튼 핀
#define PLAYER2_DEC_BUTTON_PIN 12 // 플레이어 2의 점수 감소 버튼 핀

// 초기 점수
int player1Score = 0;
int player2Score = 0;

// 버튼의 이전 상태를 저장하는 변수
bool player1IncButtonPrevState = HIGH;
bool player1DecButtonPrevState = HIGH;
bool player2IncButtonPrevState = HIGH;
bool player2DecButtonPrevState = HIGH;

// 버튼이 눌린 상태를 확인하기 위한 딜레이
#define DEBOUNCE_DELAY 50 // 디바운스 딜레이

// 버튼이 눌려 있는지 여부를 저장하는 변수
bool player1IncButtonPressed = false;
bool player1DecButtonPressed = false;
bool player2IncButtonPressed = false;
bool player2DecButtonPressed = false;

// 버튼이 눌려 있는지를 추적하기 위한 시간 변수
unsigned long player1IncButtonPressedTime = 0;
unsigned long player1DecButtonPressedTime = 0;
unsigned long player2IncButtonPressedTime = 0;
unsigned long player2DecButtonPressedTime = 0;

// 버튼이 눌려 있는지를 확인하기 위한 기준 시간
#define LONG_PRESS_DELAY 3000 // 3초

void setup() {
  // LCD 초기화
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); // LCD 커서 위치 설정
  lcd.print("Player 1:  0"); // 초기화면 표시
  lcd.setCursor(0, 1);
  lcd.print("Player 2:  0");

  // 버튼 핀 초기화
  pinMode(PLAYER1_INC_BUTTON_PIN, INPUT_PULLUP); // 내장 풀업 저항 사용
  pinMode(PLAYER1_DEC_BUTTON_PIN, INPUT_PULLUP); // 내장 풀업 저항 사용
  pinMode(PLAYER2_INC_BUTTON_PIN, INPUT_PULLUP); // 내장 풀업 저항 사용
  pinMode(PLAYER2_DEC_BUTTON_PIN, INPUT_PULLUP); // 내장 풀업 저항 사용
}

void loop() {
  // 버튼 상태 감지 및 처리
  checkButtonState(PLAYER1_INC_BUTTON_PIN, player1IncButtonPrevState, player1IncButtonPressed, player1IncButtonPressedTime, player1Score);
  checkButtonState(PLAYER1_DEC_BUTTON_PIN, player1DecButtonPrevState, player1DecButtonPressed, player1DecButtonPressedTime, player1Score);
  checkButtonState(PLAYER2_INC_BUTTON_PIN, player2IncButtonPrevState, player2IncButtonPressed, player2IncButtonPressedTime, player2Score);
  checkButtonState(PLAYER2_DEC_BUTTON_PIN, player2DecButtonPrevState, player2DecButtonPressed, player2DecButtonPressedTime, player2Score);
}

// 버튼 상태를 체크하고 딜레이를 확인하는 함수
void checkButtonState(int buttonPin, bool &buttonPrevState, bool &buttonPressed, unsigned long &pressedTime, int &playerScore) {
  bool state = digitalRead(buttonPin);

  if (state != buttonPrevState) {
    // 디바운스 딜레이 확인
    delay(DEBOUNCE_DELAY);
    state = digitalRead(buttonPin);

    if (state != buttonPrevState) {
      if (state == LOW) {
        buttonPressed = true;
        pressedTime = millis(); // 버튼을 누른 시간 기록
      } else {
        // 버튼이 떼어졌을 때
        if (buttonPressed) {
          unsigned long pressedDuration = millis() - pressedTime;

          if (pressedDuration < LONG_PRESS_DELAY) {
            // 3초 미만으로 버튼을 누른 경우
            if (buttonPin == PLAYER1_INC_BUTTON_PIN || buttonPin == PLAYER2_INC_BUTTON_PIN) {
              playerScore++;
            } else {
              if (playerScore > 0) {
                playerScore--;
              }
            }
          } else {
            // 3초 이상으로 버튼을 누른 경우
            if (buttonPin == PLAYER1_INC_BUTTON_PIN || buttonPin == PLAYER2_INC_BUTTON_PIN) {
              playerScore += 5;
            } else {
              playerScore = 0;
            }
          }
          displayScore(); // 점수 표시
        }
        buttonPressed = false;
      }
    }
  }

  buttonPrevState = state;
}

// 점수 표시 함수
void displayScore() {
  lcd.setCursor(11, 0); // 플레이어 1 점수 위치
  lcd.print("     "); // 이전 점수 지우기
  lcd.setCursor(11, 0);
  lcd.print(player1Score);

  lcd.setCursor(11, 1); // 플레이어 2 점수 위치
  lcd.print("     "); // 이전 점수 지우기
  lcd.setCursor(11, 1);
  lcd.print(player2Score);
}
