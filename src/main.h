#if !defined(_MAIN_H_)
#define _MAIN_H_

#include <Arduino.h>
#include "display/display.h"
#include "lora/lora.h"
#include "logger/logger.h"
#include "wifi/wifi.h"

// ESP32 SPI Config
#define PIN_SCK     18
#define PIN_MISO    19
#define PIN_MOSI    23

#define I2C_SDA 32
#define I2C_SDL 33
#define I2C_CLOCK_FREQUENCY 4000000

#endif