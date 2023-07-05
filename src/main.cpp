#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

byte fullBlock[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111};

class Pomodoro{
  public:
    Pomodoro() : _lcd(0x27, 16, 2) {
      _rows = 2;
      _cols = 16;
    }


    void init(){
      _lcd.init();
      _lcd.backlight();
      initCustomChars();
    }

    void start()
    {
      _start_time = millis();
      _total_time = 25 * 60 * 1000;
    }

    void tick(){
      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Dupa!");
      updateTime();
      printProgressBar();
    }

  private:
    void printProgressBar(){
      uint8_t full_blocks = min(_remaining_time * _cols / _total_time, _cols);
      for(uint8_t i = 0; i < full_blocks; i++){
        _lcd.setCursor(i, 1);
        _lcd.write((byte)0);
      }

      if(full_blocks < _cols){
        _lcd.setCursor(full_blocks, 1);
      }
    }

    void updateTime()
    {
      _remaining_time = _total_time - (millis() - _start_time);
    }

    void initCustomChars()
    {
      _lcd.createChar(0, fullBlock);
      for(uint8_t i = 1; i < 5; i++)
      {
        for(uint8_t j = 0; j < 8; j++)
        {
          fullBlock[j] = fullBlock[j] << 1;
        }


        _lcd.createChar(i, fullBlock);
      }
    }

    LiquidCrystal_I2C _lcd; 
    uint8_t _rows; 
    uint8_t _cols;

    uint64_t _remaining_time;
    uint64_t _start_time;
    uint64_t _total_time;
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