#include "config.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <DHT11.h>
#include <ESP8266HTTPClient.h>

JsonDocument doc;

void enterDeepsleep() {
  ESP.deepSleep(DEEPSLEEP_TIME);
}

void performRead() {
  int light = analogRead(PHOTORESISTOR_PIN);
  doc["light"] = light;

  DHT11 dht11(DHT_PIN);
  int temperature = 0;
  int humidity = 0;
  int result = dht11.readTemperatureHumidity(temperature, humidity);
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
}

void connectToNetwork() {

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void upload() {
  char output[UPLOAD_ARRAY_MAX_LENGTH];
  serializeJson(doc, output);

  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClientSecure client;
    client.setInsecure(); // Do not check certificate
    HTTPClient http;

    http.begin(client, ENDPOINT);
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.POST(output);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.printf("received payload: %s\n",payload);
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("ESP8266 start");

  Serial.println("ESP8266 connecting to network");
  connectToNetwork();

  Serial.println("ESP8266 reading from sensors");
  performRead();

  Serial.println("ESP8266 uploading data");
  upload();

  Serial.println("ESP8266 entering deep sleep");
  enterDeepsleep();


}

void loop() {
  // put your main code here, to run repeatedly:
  // unused because of deep sleep
}
