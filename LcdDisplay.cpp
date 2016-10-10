#include "LcdDisplay.h"
#include <Arduino.h>

 LcdDisplay:: LcdDisplay (uint8_t backLightPin,uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3):
                          LiquidCrystal(rs, enable, d0, d1, d2, d3)
 {
    _backLightPin = backLightPin;
 }

void LcdDisplay::init()
 {
   pinMode(this->_backLightPin, OUTPUT);
   digitalWrite(this->_backLightPin, LOW);
   this->begin(16,2);

 }

void LcdDisplay::on()
 {
   this->display();
   digitalWrite(this->_backLightPin, HIGH);
}

void LcdDisplay::off()
 {
   this->noDisplay();
   digitalWrite(this->_backLightPin, LOW);
}
