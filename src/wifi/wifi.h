#if !defined(_WIFI_H_)
#define _WIFI_H_

#include "helper.h"

WebServer server(80);
const char* ssid = "LoRaChatBox";
const char* password = "12345678";
String espID = "";
// Message buffer
#define MAX_MESSAGES 10
String chatLog[MAX_MESSAGES];
int logIndex = 0;

void addToChat(String msg) {
  // Shift messages
  for (int i = 0; i < MAX_MESSAGES - 1; i++) {
    chatLog[i] = chatLog[i + 1];
  }
  chatLog[MAX_MESSAGES - 1] = msg;
  logIndex = min(logIndex + 1, MAX_MESSAGES);
}

// String buildChatPage() {
// //   String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='5'><meta name='viewport' content='width=device-width, initial-scale=1'><title>LoRa Chat</title>";
//   String html = "<!DOCTYPE html><html><head><meta content='5'><meta name='viewport' content='width=device-width, initial-scale=1'><title>LoRa Chat</title>";
//   html += "<style>body{font-family:sans-serif;background:#f0f0f0;text-align:center;padding:20px;}input{padding:10px;font-size:16px;}textarea{width:90%;height:200px;font-size:14px;}</style></head><body>";
//   html += "<h2>ESP32 LoRa Chat</h2><form action='/' method='POST'>";
//   html += "<input type='text' name='msg' placeholder='Enter message' required>";
//   html += "<input type='submit' value='Send'></form>";
//   html += "<h3>Chat Log</h3><textarea readonly>";
//   for (int i = 0; i < MAX_MESSAGES; i++) {
//     if (chatLog[i].length() > 0) {
//       html += chatLog[i] + "\n";
//     }
//   }
//   html += "</textarea></body></html>";
//   return html;
// }

String buildChatPage() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <title>LoRa Chat</title>
    <style>
        body { font-family: sans-serif; text-align: center; padding: 20px; }
        input[type="text"] { width: 80%; padding: 10px; font-size: 16px; }
        textarea { width: 90%; height: 200px; font-size: 14px; }
    </style>
    </head>
    <body>
    <h2>ESP32 LoRa Chat</h2>
    <form action="/" method="GET">
        <input type="text" name="msg" placeholder="Enter message" required autofocus>
        <input type="submit" value="Send">
    </form>

    <h3>Chat Log</h3>
    <textarea id="chatLog" readonly></textarea>

    <script>
        function fetchLog() {
        fetch('/log')
            .then(response => response.text())
            .then(data => {
            document.getElementById('chatLog').value = data;
            });
        }
        setInterval(fetchLog, 3000); // every 3 seconds
        fetchLog(); // initial call
    </script>
    </body>
    </html>
    )rawliteral";

  return html;
}

void handleRoot() {
  if (server.hasArg("msg")) {
    String msg = server.arg("msg");
    msg = espID + ": " + msg;
    String line = "[Me] " + msg;
    addToChat(line);

    // Send over LoRa
    radio.transmit(msg);

    // Display on OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("You:");
    display.println(msg);
    display.display();

    server.send(200, "text/html", buildChatPage());
  } else {
    server.send(200, "text/html", buildChatPage());
  }
}


void pollHTTPChat() {
    #ifndef CHAT_VIA_HTTP
        return;
    #endif

    server.handleClient();

    String incoming;
    int state = radio.receive(incoming);
    if (state == RADIOLIB_ERR_NONE) {
        String line = "[Remote] " + incoming;
        addToChat(line);

        // Show on OLED
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Friend:");
        display.println(incoming);
        displayRFinfo();

        Serial.println("Received: " + incoming);
    }
}

bool SetupWifiAP() {
    #ifndef CHAT_VIA_HTTP
        LogWarnf("skip init setup wifi\n");
        return true;
    #endif

    // Wi-Fi AP
    String ssidWithSuffix = String(ssid);
    espID = getChipID();
    ssidWithSuffix +="-"+espID;

    WiFi.softAP(ssidWithSuffix.c_str(), password);
    IPAddress IP = WiFi.softAPIP();
    LogInfof("AP IP address: %s\n", IP.toString().c_str());
    display.clearDisplay();
    display.printf("WiFi: %s\n", ssidWithSuffix.c_str());
    display.print("IP: ");
    display.println(IP);
    display.display();

    // Web server
    server.on("/", handleRoot);

    server.on("/log", HTTP_GET, []() {
    String logData = "";
    for (int i = 0; i < MAX_MESSAGES; i++) {
        if (chatLog[i].length() > 0) {
        logData += chatLog[i] + "\n";
        }
    }
    server.send(200, "text/plain", logData);
    });

    server.begin();
    return true;
}

#endif