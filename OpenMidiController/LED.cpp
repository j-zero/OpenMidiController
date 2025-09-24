#include <Arduino.h>
#include "LED.h"

void LEDGreenOn(){
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_GREEN, HIGH);
}

void LEDGreen(int val){
  digitalWrite(PIN_LED_RED, LOW);
  analogWrite(PIN_LED_GREEN, val);
}

void LEDRedOn(){
  digitalWrite(PIN_LED_RED, HIGH);
  digitalWrite(PIN_LED_GREEN, LOW);
}

void LEDRed(int val){
  digitalWrite(PIN_LED_GREEN, LOW);
  analogWrite(PIN_LED_RED, val);
}

void LEDOff(){
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);
}
