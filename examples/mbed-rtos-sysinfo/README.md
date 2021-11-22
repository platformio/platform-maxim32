How to build PlatformIO based project
=====================================

1. [Install PlatformIO Core](http://docs.platformio.org/page/core.html)
2. Download [development platform with examples](https://github.com/platformio/platform-maxim32/archive/develop.zip)
3. Extract ZIP archive
4. Run these commands:

```shell
# Change directory to example
$ cd platform-maxim32/examples/mbed-rtos-sysinfo

# Build project
$ pio run

# Upload firmware
$ pio run --target upload

# Build specific environment
$ pio run -e max32625pico

# Upload firmware for the specific environment
$ pio run -e max32625pico --target upload

# Clean build files
$ pio run --target clean
```
