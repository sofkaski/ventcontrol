#ifndef LcdDisplay_h
#define LcdDisplay_h

#include <LiquidCrystal.h>

class LcdDisplay: public LiquidCrystal {
  public:
     LcdDisplay(uint8_t backLightPin, uint8_t rs, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
     void init();
     void on();
     void off();
  private:
    int _backLightPin;

};

#endif
