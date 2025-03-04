#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "HX711.h"

#define SERVO_PIN  4
#define START_BUTTON  5
#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2
#define INCREASE_BUTTON 6
#define DECREASE_BUTTON 7

#define STANDARD_TIME_TEA 60
#define EMPTY_CUP 0.643
#define FULL_CUP 1.332

Servo Servo1;
LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711 scale;

const float calibration_factor = 1040300;
int waitTime = STANDARD_TIME_TEA;

int lastStartButtonState = LOW;
int startButtonState = LOW;

int lastIncreaseButtonState = LOW;
int increaseButtonState = LOW;

int lastDecreaseButtonState = LOW;
int decreaseButtonState = LOW;

unsigned long lastDebounceTimeStart = 0;
unsigned long lastDebounceTimeIncrease = 0;
unsigned long lastDebounceTimeDecrease = 0;

const unsigned long debounceDelay = 50;
bool startButtonPressed = false;
bool increaseButtonPressed = false;
bool decreaseButtonPressed = false;

enum State { IDLE, COMPUTE, WAIT_FOR_CONFIRM, BREW, COMPLETE };
State currentState = IDLE;

void setup() {
  Servo1.attach(SERVO_PIN);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Press Start");

  pinMode(START_BUTTON, INPUT);
  pinMode(INCREASE_BUTTON, INPUT);
  pinMode(DECREASE_BUTTON, INPUT);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();
}

void loop() {
  debounceButtons();

  switch (currentState) {
    case IDLE:
      handleIdleState();
      break;

    case COMPUTE:
      handleComputeState();
      break;

    case WAIT_FOR_CONFIRM:
      handleWaitForConfirmState();
      break;

    case BREW:
      handleBrewState();
      break;

    case COMPLETE:
      handleCompleteState();
      break;
  }
}

void debounceButtons() {
  int readingStart = digitalRead(START_BUTTON);
  if (readingStart != lastStartButtonState) {
    lastDebounceTimeStart = millis();
  }
  if ((millis() - lastDebounceTimeStart) > debounceDelay) {
    if (readingStart != startButtonState) {
      startButtonState = readingStart;
      if (startButtonState == HIGH) {
        startButtonPressed = true;
      }
    }
  }
  lastStartButtonState = readingStart;


  int readingIncrease = digitalRead(INCREASE_BUTTON);
  if (readingIncrease != lastIncreaseButtonState) {
    lastDebounceTimeIncrease = millis();
  }
  if ((millis() - lastDebounceTimeIncrease) > debounceDelay) {
    if (readingIncrease != increaseButtonState) {
      increaseButtonState = readingIncrease;
      if (increaseButtonState == HIGH) {
        increaseButtonPressed = true;
      }
    }
  }
  lastIncreaseButtonState = readingIncrease;


  int readingDecrease = digitalRead(DECREASE_BUTTON);
  if (readingDecrease != lastDecreaseButtonState) {
    lastDebounceTimeDecrease = millis();
  }
  if ((millis() - lastDebounceTimeDecrease) > debounceDelay) {
    if (readingDecrease != decreaseButtonState) {
      decreaseButtonState = readingDecrease;
      if (decreaseButtonState == HIGH) {
        decreaseButtonPressed = true;
      }
    }
  }
  lastDecreaseButtonState = readingDecrease;
}

void handleIdleState() {
  if (startButtonPressed) {
    startButtonPressed = false;
    currentState = COMPUTE;
  }
}

void handleComputeState() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Computing Time...");
  delay(2000);
  float weight = scale.get_units();
  waitTime = computeWaitTime(weight);
  currentState = WAIT_FOR_CONFIRM;
  printWaitTime();
}

void handleWaitForConfirmState() {
  if (startButtonPressed) {
    startButtonPressed = false; 
    currentState = BREW;
  }

  if (increaseButtonPressed) {
    increaseButtonPressed = false; 
    waitTime = min(waitTime + 10, STANDARD_TIME_TEA * 2);
    printWaitTime();
  }

  if (decreaseButtonPressed) {
    decreaseButtonPressed = false; 
    waitTime = max(waitTime - 10, 10);
    printWaitTime();
  }
}

void handleBrewState() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Brewing...");
  Servo1.write(0);

  for (int remainingTime = waitTime; remainingTime > 0; remainingTime--) {
    lcd.setCursor(0, 1);
    lcd.print("Time left: ");
    lcd.print(remainingTime);
    lcd.print(" sec");

    delay(1000);
  }

  Servo1.write(90);

  currentState = COMPLETE;
}


void handleCompleteState() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enjoy your tea!");
  delay(5000);
  currentState = IDLE;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press Start");
}

int computeWaitTime(float weight) {
  if (weight > (FULL_CUP - EMPTY_CUP) / 2 + EMPTY_CUP ) {
    return STANDARD_TIME_TEA;
  }
  return STANDARD_TIME_TEA / 2; 
}

void printWaitTime() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wait time:");
  lcd.setCursor(0, 1);
  lcd.print(waitTime);
  lcd.print(" sec");
}
