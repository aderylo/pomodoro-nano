#  PomodoroNano
Arduino Nano 33 IOT based pomodoro timer. 

## Demo
Please find attach demo of the project:
https://youtu.be/D5Yah-L7upg

## Running 

1. [Install PlatformIO Core](https://docs.platformio.org/page/core.html)
2. Download [development platform with examples](https://github.com/platformio/platform-atmelsam/archive/develop.zip)
3. Extract ZIP archive
4. Run these commands:

```shell
# Build project
$ pio run

# Upload firmware
$ pio run --target upload

# Build specific environment
$ pio run -e due

# Upload firmware for the specific environment
$ pio run -e due --target upload

# Clean build files
$ pio run --target clean
```
