#if !defined(_BUTTON_H_)
#define _BUTTON_H_

#include "helper.h"

int8_t ReadJoystick() {
    #ifndef WITH_JOYSTICK
        LogDebugf("not using joystick!\n");
        return -1;
    #endif

    uint16_t adc = analogReadRaw(JOYSTICK);
    uint16_t joystickButton[] = JOYSTICK_VALUES;
    
    for(int8_t x=0;x<=JOYSTICK_NUM_POSITION;x++) {
        if((adc>(joystickButton[x]-JOYSTICK_ADC_TOLERANCE) && adc<(joystickButton[x]+JOYSTICK_ADC_TOLERANCE)))
            return x;
    }
    LogDebugf("joystick pos unknown, val: %u\n", adc);
    return -1;
}

void JoystickWaitUntilUnpressed() {
    while(ReadJoystick()!=JOYSTICK_UNPRESSED);
}

// JoystickIsPressed will return which button is pressed and isLongPress
std::tuple<int8_t, bool> JoystickIsPressed() {
    auto btn = ReadJoystick();
    auto tStart = millis();
    if (btn < JOYSTICK_NUM_POSITION) { // is pressed
        while (ReadJoystick()==btn); // wait until release
        auto deltaT = millis() - tStart;
        if (deltaT > JOYSTICK_LONG_PRESS_DURATION)
            return {btn, true};

        LogDebugf("pressed in: %d\n", deltaT);
        return {btn, false};
    }
    return {btn, false};
}

void SetupJoystick() {
    #ifndef WITH_JOYSTICK
        LogDebugf("skip init joystick\n");
        return;
    #endif

    pinMode(JOYSTICK, INPUT);
    return;
}

#endif