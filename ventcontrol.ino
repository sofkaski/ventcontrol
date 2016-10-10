
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "LcdDisplay.h"
#include "TimerOne.h"
#define LCD_LIGHT_PIN A4
#define BUTTON_PIN 10
#define RS_PIN 12
#define ENABLE_PIN 11
#define D0_PIN 5
#define D1_PIN 4
#define D2_PIN 3
#define D3_PIN 2
#define DISPLAY_ON_TIME 10000 //milliseconds

LcdDisplay *lcd = new LcdDisplay(LCD_LIGHT_PIN,
                                RS_PIN,
                                ENABLE_PIN,
                                D0_PIN,
                                D1_PIN,
                                D2_PIN,
                                D3_PIN );

volatile unsigned long displayOnTimer = 0;
volatile boolean displayShouldGoOff = false;
int buttonState = HIGH;
int buttonPrevState = HIGH;

void setup() {

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  buttonPrevState = HIGH;
  lcd->init();
  lcd->off();
  lcd->print("Seconds:");
  Timer1.initialize(1000);
  Timer1.attachInterrupt(displayTimerIsr);
}

void loop() {
  unsigned long now = millis();
  
  lcd->setCursor(0, 1);
  // print the number of seconds since reset:
  lcd->print(now / 1000);

  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW) {
     if (buttonPrevState == HIGH) { // turn display on with HIGH -> LOW transition
      lcd->on();
      noInterrupts();      
      displayOnTimer = DISPLAY_ON_TIME;
      interrupts();
     } 
  }
  buttonPrevState = buttonState;
  setDisplayState();

 delayMicroseconds(1000);

}

void displayTimerIsr(void) {
  if (displayOnTimer > 0) {
    displayOnTimer--;
    if (displayOnTimer == 0) {
       displayShouldGoOff = true;
    }
  }  
}

void setDisplayState(void) {
  noInterrupts();      
  boolean shouldGoOff = displayShouldGoOff;
  displayShouldGoOff = false;
  interrupts();
  if (shouldGoOff) {
    lcd ->off();
  }    
}
