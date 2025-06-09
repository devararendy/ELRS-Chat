#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "../display/display.h"
#include "../logger/logger.h"
#include "../lora/lora.h"

String getChipID() {
  uint64_t mac = ESP.getEfuseMac();  // 64-bit MAC
  char id[13];
  sprintf(id, "%04X%08X", (uint16_t)(mac >> 32), (uint32_t)mac);
  return String(id);
}