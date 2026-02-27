#include "config.h"

void enterDeepsleep() {

}

void performRead() {
  
}

void connectToNetwork() {

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
