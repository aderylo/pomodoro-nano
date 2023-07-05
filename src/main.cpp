#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

class Pomodoro {
  public:
  Pomodoro() : _lcd(0x27, 16, 2) {
    _time_left = 0;
    _time_per_session = 25 * 60;
    _time_per_break = 5 * 60;
    _time_per_long_break = 15 * 60;
    _lcd_rows = 2;
    _lcd_cols = 16;

    _lcd.clear(); 
  }



  void printProgressBar(int row_index){
    int32_t full_blocks = _time_left / _session_time * _lcd_cols;
    for(int32_t i = 0; i < full_blocks; i++){
      _lcd.setCursor(i, row_index); 
      _lcd.print("#");
    }

  }

void tick()
{
  _lcd.clear();                 // clear display
  _lcd.setCursor(0, 0);         // move cursor to   (0, 0)
  _lcd.print("Arduino");        // print message at (0, 0)
  _lcd.setCursor(2, 1);         // move cursor to   (2, 1)
  _lcd.print("GetStarted.com"); // print message at (2, 1)
  delay(2000);                 // display the above for two seconds

  _lcd.clear();                  // clear display
  _lcd.setCursor(3, 0);          // move cursor to   (3, 0)
  _lcd.print("DIYables");        // print message at (3, 0)
  delay(2000);                  // display the above for two seconds
}


  private:
  int64_t _time_left;
  int64_t _session_time; 
  int64_t _time_per_session;
  int64_t _time_per_break;
  int64_t _time_per_long_break;
  LiquidCrystal_I2C _lcd;
  int64_t _lcd_rows;
  int64_t _lcd_cols; 
};


LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
Pomodoro pomodoro;

void setup()
{
  lcd.init(); // initialize the lcd
  lcd.backlight(); 
}


void loop(){
  pomodoro.tick();
}