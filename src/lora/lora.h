#if !defined(_LORA_H_)
#define _LORA_H_
#include "helper.h"

volatile bool loraInterruptRXTX, loraRXFlag, loraTXFlag = false;
int transmissionState = RADIOLIB_ERR_NONE;
int count = 0;

String receivedMessage = "";
String sentMessage = "";

void setFlagLora(void) {
  // we got a packet, set the flag
  loraInterruptRXTX = true;
}

void displayRFinfo() {
    display.println();
    display.print(F("RSSI: "));
    display.print(radio.getRSSI());
    display.println(F(" dBm"));
    display.print(F("SNR: "));
    display.print(radio.getSNR());
    display.println(F(" dB"));
    display.print(F("Fq err: "));
    display.print(radio.getFrequencyError());
    display.println(F(" Hz"));
    display.display();
}

void pollLoraTX() {
  if (Serial.available()) {
    sentMessage = Serial.readStringUntil('\n');
    int status = radio.transmit(sentMessage);

    display.clearDisplay();
    display.setCursor(0, 0);
    if (status == RADIOLIB_ERR_NONE) {
      display.println("Me: " + sentMessage);
      Serial.println("Sent: " + sentMessage);
    } else {
      display.println("Send error!");
    }
    display.display();
  }
}

void pollLoraRX(){
    String incoming;
    int state = radio.receive(incoming);
    if (state == RADIOLIB_ERR_NONE) {
        receivedMessage = incoming;

        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Them: " + receivedMessage);
        displayRFinfo();

        Serial.println("Received: " + receivedMessage);
    }
}

bool SetupLora() {
    return InitLora(setFlagLora);
}

#endif