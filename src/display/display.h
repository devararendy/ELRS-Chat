#if !defined(_DISPLAY_H_)
#define _DISPLAY_H_

#include "helper.h"

bool SetupDisplay() {
    #ifndef WITH_DISPLAY
        LogInfof("Skip init display\n");
        return true
    #endif
    bool result = display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.setTextColor(SSD1306_WHITE); // Draw white text
    return result;
}

#endif