#include <Arduino.h>
#include "../logger/logger.h"
#include "../hw_config.h"
#include <tuple>

#define JOYSTICK_ADC_TOLERANCE 50 // +- of each JOYSTICK_VALUES
#define JOYSTICK_NUM_POSITION 5 // how many position in JOYSTICK_VALUES
#define JOYSTICK_LONG_PRESS_DURATION 3000 // threshold for long press in millisecond
#define JOYSTICK_UP             0 // based on index of JOYSTICK_VALUES
#define JOYSTICK_DOWN           1 // based on index of JOYSTICK_VALUES
#define JOYSTICK_LEFT           2 // based on index of JOYSTICK_VALUES
#define JOYSTICK_RIGHT          3 // based on index of JOYSTICK_VALUES
#define JOYSTICK_OK             4 // based on index of JOYSTICK_VALUES
#define JOYSTICK_UNPRESSED      5 // based on index of JOYSTICK_VALUES
