#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

class Pomodoro{
  public:
    Pomodoro() : _lcd(0x27, 16, 2) {
      _rows = 2;
      _cols = 16;
    }


    void init(){
      _lcd.init();
      _lcd.backlight();
      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Hello, world!");

    }
    void tick(){
      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Dupa!");
      printProgressBar();
    }

  private:
    void printProgressBar(){
      for(uint8_t i = 0; i < _cols; i++){
        _lcd.setCursor(i, 1);
        _lcd.print("X");
      }
    }



    LiquidCrystal_I2C _lcd; 
    uint8_t _rows; 
    uint8_t _cols;
    
}; 

Pomodoro pomodoro;
void setup()
{
 pomodoro.init(); 
}


void loop(){
  pomodoro.tick();
  delay(1000);
}