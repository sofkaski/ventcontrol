#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
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
#define ONE_WIRE_BUS 7

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

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


void setup() {

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  buttonPrevState = HIGH;
  lcd->init();
  lcd->off();
  lcd->print("Temp:");
  Timer1.initialize(1000);
  Timer1.attachInterrupt(displayTimerIsr);
  sensors.begin();
}

void loop() {
 

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

  sensors.requestTemperatures();
  float temp0 = sensors.getTempCByIndex(0);
  float temp1 = sensors.getTempCByIndex(1);
  float temp2 = sensors.getTempCByIndex(2);
  printTemperature("Sensor 0", temp0);
  delay(1000);
  printTemperature("Sensor 1", temp1);
  delay(1000);
  printTemperature("Sensor 2", temp2);
  delay(1000);

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

void printTemperature(String sensor, float temperature) {
  lcd->setCursor(0, 0);
  lcd->print(sensor);
  lcd->setCursor(0, 1);
  char fAsChars[10];
  dtostrf(temperature, 6, 2, fAsChars);
  lcd->print(fAsChars);
  
}
