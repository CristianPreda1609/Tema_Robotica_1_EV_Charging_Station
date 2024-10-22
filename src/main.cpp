#include <Arduino.h>

const int startButtonPin = 3;  
const int stopButtonPin = 2;   
const int redPin = 6;          
const int greenPin = 5;        
const int ledPins[] = {10, 9, 8, 7};  

volatile unsigned long lastLedBlinkTime = 0;   
volatile unsigned long lastLedTransitionTime = 0; 
const long ledInterval = 3000; 
const long blinkInterval = 500; 
volatile unsigned long stopButtonPressStart = 0; 
const long longPressDuration = 1000; 

volatile unsigned long lastStartButtonPressTime = 0; 
volatile unsigned long lastStopButtonPressTime = 0;  
const long debounceDelay = 50; 

volatile bool isCharging = false;
volatile int currentLevel = 0;
volatile bool ledOn = false; 

void blinkLed(int pin) {
  if ((millis() - lastLedBlinkTime) >= blinkInterval) {
    ledOn = !ledOn;  
    digitalWrite(pin, ledOn ? LOW : HIGH); 
    lastLedBlinkTime = millis();
  }
}

void resetStation() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);  
  }
  digitalWrite(redPin, HIGH);   
  digitalWrite(greenPin, LOW);  
  currentLevel = 0;
  isCharging = false;
}

void startCharging() {
  digitalWrite(greenPin, HIGH);  
  digitalWrite(redPin, LOW);     
  isCharging = true;
  currentLevel = 0;
  lastLedTransitionTime = millis(); 
  lastLedBlinkTime = millis();      
}

void stopCharging() {
  for (int i = 0; i <= 3; i++) {
    for (int j = 0; j < 4; j++) {
      digitalWrite(ledPins[j], HIGH);  
    }
    delay(500);  
    for (int j = 0; j < 4; j++) {
      digitalWrite(ledPins[j], LOW);  
    }
    delay(500);
  }
  resetStation();  
}

void handleStopButton(unsigned long currentTime) {
  if (digitalRead(stopButtonPin) == LOW && (currentTime - lastStopButtonPressTime) > debounceDelay && isCharging) {
    if (stopButtonPressStart == 0) {
      stopButtonPressStart = currentTime;  
    }
    lastStopButtonPressTime = currentTime;  
  }

  if (digitalRead(stopButtonPin) == HIGH && stopButtonPressStart != 0) {
    stopButtonPressStart = 0;  
  }

  if (stopButtonPressStart != 0 && (currentTime - stopButtonPressStart) >= longPressDuration && isCharging) {
    stopCharging();  
    stopButtonPressStart = 0;  
  }
}

void setup() {
  pinMode(startButtonPin, INPUT);  
  pinMode(stopButtonPin, INPUT);   
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  
  resetStation(); 
}

void loop() {
  int startButtonState = digitalRead(startButtonPin);
  unsigned long currentTime = millis();
  
  if (startButtonState == LOW && !isCharging && (currentTime - lastStartButtonPressTime) > debounceDelay) {  
    startCharging();
    lastStartButtonPressTime = currentTime;  
  }

  handleStopButton(currentTime);
  
  if (isCharging) {
    blinkLed(ledPins[currentLevel]);

    if ((millis() - lastLedTransitionTime) >= ledInterval) {
      digitalWrite(ledPins[currentLevel], HIGH); 
      
      currentLevel++;
      
      if (currentLevel >= 4) {
        stopCharging();
      } else {
        lastLedTransitionTime = millis();  
      }
    }
  }
}
