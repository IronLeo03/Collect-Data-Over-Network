#include "config.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <DHT11.h>

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
