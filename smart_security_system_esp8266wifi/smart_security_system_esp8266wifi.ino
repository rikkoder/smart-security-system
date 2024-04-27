#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SERVER_IP "192.168.77.56:3000"

#ifndef STASSID
#define STASSID "Mihir"
#define STAPSK "9427110748"
#endif

#define DANGER "Danger!!"

void setup() {

  Serial.begin(115200);

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  String msg = "XX";
  if (Serial.available() > 0) {
    msg = Serial.readStringUntil('\n');
  }
  Serial.flush();

  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;

    // configure traged server and url
    http.begin(client, "http://" SERVER_IP "/arduino-data/");  // HTTP
    http.addHeader("Content-Type", "text/plain");

    // start connection and send HTTP header and body
    int httpCode = 0;
    if (msg != "XX")
      httpCode = http.POST(msg);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
//      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
//        Serial.println("received payload:\n<<");
//        Serial.println(payload);
//        Serial.println(">>");
      }
    } else {
//      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(1000);
}
