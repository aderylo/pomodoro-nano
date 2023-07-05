#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <string>

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
      _start_time = millis() / 1000;
      _remaining_time = _total_time = 10 * 60;
    }

    void tick(){
      _lcd.clear();
      updateTime();
      printFocusInfo(); 
      printProgressBar();
    }

  private:
    void printFocusInfo(){
      uint8_t minutes = _remaining_time / 60;
      uint8_t seconds = _remaining_time % 60;
      std::string focus_info = "Focus!" 
        + std::to_string(minutes) + ":" + std::to_string(seconds); 
      _lcd.setCursor(0, 0);
      _lcd.print(focus_info.c_str());
    }

    void printProgressBar(){
      uint64_t full_blocks = min(_remaining_time * _cols / _total_time, _cols);
      for(uint64_t i = 0; i < full_blocks; i++){
        _lcd.setCursor(i, 1);
        _lcd.write((byte)0);
      }

      if(full_blocks < _cols){
        _lcd.setCursor(full_blocks, 1);
      }
    }

    void updateTime()
    {
      uint64_t current_time = millis() / 1000;
      _remaining_time = _total_time - (current_time - _start_time);
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
 pomodoro.start();
}


void loop(){
  pomodoro.tick();
  delay(1000);
}