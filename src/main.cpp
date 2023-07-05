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
      _is_break = false;
      _sessions_completed = 0;
      _total_sessions = 1;
      _time_per_break = 1 * 10;
      _time_per_long_break = 2 * 10;
      _time_per_session = 1 * 10;
      _remaining_time = _total_time = _time_per_session;
    }

    void tick(){
      _lcd.clear();
      updateTime();
      if(_remaining_time == 0){
        changeState();
      }

      if(_is_break)
        printBreakInfo();
      else
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

    void printBreakInfo(){
      uint8_t minutes = _remaining_time / 60;
      uint8_t seconds = _remaining_time % 60;
      std::string break_info = "Break!" 
        + std::to_string(minutes) + ":" + std::to_string(seconds); 
      _lcd.setCursor(0, 0);
      _lcd.print(break_info.c_str());
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
      uint64_t time_elapsed = current_time - _start_time;
      _remaining_time = (_total_time > time_elapsed) ? (_total_time - time_elapsed) : 0;
    }

    void changeState(){
      if(!_is_break){
        _sessions_completed++;
        if(_sessions_completed == _total_sessions){
          _total_time = _time_per_long_break;
          _sessions_completed = 0;
        }
        else{
          _total_time = _time_per_break;
        }
      }
      else{
        _total_time = _time_per_session;
      }
      
      _is_break = !_is_break;
      _start_time = millis() / 1000;
      _remaining_time = _total_time;
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

    uint8_t _time_per_session;
    uint8_t _time_per_break;
    uint8_t _time_per_long_break;



    bool _is_break;
    uint8_t _total_sessions;
    uint8_t _sessions_completed;
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