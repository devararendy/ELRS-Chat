
#include "main.h"

void loop() {
  pollHTTPChat();
  pollSerialChat();
  pollStopwatch();
} 

void setup() {
Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SDL, I2C_CLOCK_FREQUENCY);
  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI);

  delay(2000); // Pause for 2 seconds

  SetupJoystick();

  if(!SetupDisplay()) {
    LogInfof("Display Init failed\n");
    for(;;); // Don't proceed, loop forever
  }

  delay(2000); // Pause for 2 seconds

  if(!SetupLora()) {
    LogInfof("Lora Init failed\n");
    for(;;); // Don't proceed, loop forever
  }

  if(!SetupWifiAP()) {
    LogInfof("WIFI Chat Init failed\n");
    for(;;); // Don't proceed, loop forever
  }

  LogInfof("Done All Init %d\n", (int)1);
}