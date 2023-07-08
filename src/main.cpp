#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <string>

#define SERIAL_PORT 9600

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
      Serial.begin(SERIAL_PORT);
    }

    void setup(){
      std::string msg; 

      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print("Pomodoro Timer"); 
      delay(1000); 

      _time_per_session = inputIntRequest("Specify time per session on Serial:" + std::to_string(SERIAL_PORT));
      _time_per_break = inputIntRequest("Specify time per break on Serial:" + std::to_string(SERIAL_PORT));
      _time_per_long_break = inputIntRequest("Specify time per long break on Serial:" + std::to_string(SERIAL_PORT));
    }

    void start()
    {
      _start_time = millis() / 1000;
      _is_break = false;
      _sessions_completed = 0;
      _total_sessions = 4;
      _time_per_break = 1 * 5;
      _time_per_long_break = 2 * 5;
      _remaining_time = _total_time = _time_per_session;
    }

    void tick(){
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
    uint64_t inputIntRequest(std::string msg){
      uint64_t result;
      _lcd.clear();
      _lcd.setCursor(0, 0);
      _lcd.print(msg.c_str());

      while(true){
        if(Serial.available()){
          arduino::String input = Serial.readStringUntil('\n');
          result = std::stoi(input.c_str());
          break;
        }
      }

      _lcd.clear();

      return result;
    }



    void printFocusInfo(){
      std::string focus_info = "Focus!" 
        + getRemainingTime();
      _lcd.setCursor(0, 0);
      _lcd.print(focus_info.c_str());
    }

    void printBreakInfo(){
      std::string break_info = "Break!" 
        + getRemainingTime();
      _lcd.setCursor(0, 0);
      _lcd.print(break_info.c_str());
    }

    std::string getRemainingTime(){
      std::string result; 
      uint8_t minutes = _remaining_time / 60;
      uint8_t seconds = _remaining_time % 60;
      result += std::to_string(minutes) + ":"; 
      result += (seconds < 10) ? "0" + std::to_string(seconds) : std::to_string(seconds);
      return result + " ";
    }

    void printProgressBar(){
      uint64_t full_blocks = min(_remaining_time * _cols / _total_time, _cols);
      for(uint64_t i = 0; i < full_blocks; i++){
        _lcd.setCursor(i, 1);
        _lcd.write((byte)0);
      }

      for(uint64_t i = full_blocks; i < _cols; i++){
        _lcd.setCursor(i, 1);
        _lcd.write((byte)5);
      }
    }

    void updateTime()
    {
      uint64_t current_time = millis() / 1000;
      uint64_t time_elapsed = current_time - _start_time;
      _remaining_time = (_total_time > time_elapsed) ? (_total_time - time_elapsed) : 0;
    }

    void changeState(){
      _lcd.clear();
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
      for(uint8_t i = 1; i <= 5; i++)
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
 Serial.begin(9600);
 pomodoro.init(); 
 pomodoro.setup();
 pomodoro.start();
}


void loop(){
  pomodoro.tick();
  Serial.println("Tick");
  delay(1000);
}