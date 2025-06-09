#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>
#include "../logger/logger.h"
#include "../hw_config.h"

// SX1280 has the following connections:
SX1280 radio = new Module(PIN_LORA_NSS, PIN_LORA_DIO1, PIN_LORA_NRST, PIN_LORA_BUSY);

// or detect the pinout automatically using RadioBoards
// https://github.com/radiolib-org/RadioBoards
/*
#define RADIO_BOARD_AUTO
#include <RadioBoards.h>
Radio radio = new RadioModule();
*/

void initPA() {
    pinMode(PIN_PA_RXEN, OUTPUT);
    pinMode(PIN_PA_TXEN, OUTPUT);

    digitalWrite(PIN_PA_TXEN, LOW);
    digitalWrite(PIN_PA_RXEN, LOW);
}

void initFan() {
    #ifdef PIN_FAN
    pinMode(PIN_FAN, OUTPUT);
    digitalWrite(PIN_FAN, LOW);
    #endif
}

void setFan(int pwm) {
    #ifdef PIN_FAN
    analogWrite(PIN_FAN, pwm);
    #endif
}

bool InitLora(void (*callback_rx)(void), PowerLevels_e pwr=PWR_10mW) {
  initPA();
  radio.setRfSwitchPins(PIN_PA_RXEN, PIN_PA_TXEN);

  initFan();
  setFan(255);
  // initialize SX1280 with default settings
  int state = radio.begin();
  if (state != RADIOLIB_ERR_NONE) {
    return false;
  }

  state = radio.setOutputPower(getPowerValues(pwr));
  if (state != RADIOLIB_ERR_NONE) {
    return false;
  }
  // set the function that will be called
  // when new packet is received
  radio.setPacketReceivedAction(callback_rx);

  // start listening for LoRa packets
  Serial.println(F("[SX1280] Starting to listen ... "));
  state = radio.startReceive();
  if (state != RADIOLIB_ERR_NONE) {
    return false;
  }

  LogInfof("[SX1280] Done, Pwr: %d\n", pwr);
  return true;
  // if needed, 'listen' mode can be disabled by calling
  // any of the following methods:
  //
  // radio.standby()
  // radio.sleep()
  // radio.transmit();
  // radio.receive();
  // radio.readData();
  // radio.scanChannel();
}