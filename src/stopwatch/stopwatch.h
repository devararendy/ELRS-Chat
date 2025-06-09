#if !defined(_STOPWATCH_H_)
#define _STOPWATCH_H_

#include "helper.h"

unsigned long lastDisplayedAt = 0;
unsigned long stopwatchCheckpoint[STOPWATCH_LEN_CHECKPOINT] = {0};
uint8_t checkpointNum = 0;
bool stopwatchIsRunning = false;

void showTime(unsigned long ms) {
  unsigned int milliseconds = ms % 1000;
  unsigned int seconds = (ms / 1000) % 60;
  unsigned int minutes = (ms / 60000) % 60;
  unsigned int hours = (ms / 3600000);

  char buffer[20];
  sprintf(buffer, "%02u:%02u:%02u.%03u", hours, minutes, seconds, milliseconds);
  LogInfof("Stopwatch: %s\n", buffer);
 
  #ifdef WITH_DISPLAY
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 25);
  display.print(buffer);
  display.display();
  #endif
}

void ResetStopwatch() {
    memset((void*)stopwatchCheckpoint, 0, sizeof(stopwatchCheckpoint));
    checkpointNum = 0;
}

void pollStopwatch() {
    #ifndef FEATURE_STOPWATCH
        return;
    #endif

    auto btn = JoystickIsPressed();
    int whichBtn = std::get<0>(btn);
    bool isLongPress = std::get<1>(btn);

    // short press OK to start or checkpoint if already running
    if ((whichBtn == JOYSTICK_OK) && (!isLongPress)){
        if (stopwatchIsRunning) {
            auto curTime = millis();
            stopwatchCheckpoint[checkpointNum] = curTime;
            auto checkpointTime = (curTime-stopwatchCheckpoint[(checkpointNum-1)]);
            LogInfof("Checkpoint: %d, time: %lu\n", checkpointNum, checkpointTime);
            showTime(checkpointTime);
            checkpointNum++;
            delay(200); // debounce button
            return;
        }

        stopwatchCheckpoint[checkpointNum++] = millis();
        stopwatchIsRunning = true;
        LogInfof("Stopwatch Started!\n");
        delay(200); // debounce button
        return;
    }

    if ((whichBtn == JOYSTICK_OK) && (isLongPress) && (stopwatchIsRunning)){
        auto elapsed = millis() - stopwatchCheckpoint[0];
        stopwatchIsRunning = false;
        LogInfof("Stopwatch Stopped, elapsed: %lu\n", elapsed);
        showTime(elapsed);
        ResetStopwatch();
        JoystickWaitUntilUnpressed();
        return;
    }

    if (stopwatchIsRunning) {
        // show time on each interval but don't block using delay
        if (millis() > (lastDisplayedAt+STOPWATCH_DISPLAY_INTERVAL)){
            lastDisplayedAt = millis();
            auto elapsed = (lastDisplayedAt - stopwatchCheckpoint[0]);
            LogInfof("Stopwatch: %lu\n", elapsed);
            showTime(elapsed);
        }
    }
    
    return;
}

bool SetupStopwatch() {
    #ifndef FEATURE_STOPWATCH
        return true;
    #endif

    return true;
}

#endif