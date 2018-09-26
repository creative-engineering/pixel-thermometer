#include Arduino.h>
#include ESP8266WiFi.h>
#include ESP8266HTTPClient.h>
#include ArduinoJson.h>
#include Adafruit_NeoPixel.h>
#ifdef __AVR__
#include avr/power.h>
#endif
#define PIN 12 //D6 on NodeMCU
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

const char* ssid = "Your_Wifi_name";
const char* password = "Your_wifi_password";

int newTemp = 0;

void setup() {

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(57600);

  WiFi.begin(ssid, password); //connect to the wifi access point

  Serial.print("Connecting  ");
  while (WiFi.status() != WL_CONNECTED) {
    rainbow(20);
    Serial.print(".");
    // delay(250);
  }

  Serial.println("Connected ! ");

  for (uint16_t y = 0; y < strip.numPixels(); y++) {
    strip.setPixelColor(y, 255, 255, 255);
    strip.show();
    delay(30);
  }
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http; //Declare an object of class HTTPClient

    // REMEMBER to replace YOURKEY from the url with your own key
    http.begin("http://api.openweathermap.org/data/2.5/forecast?q=Copenhagen,dk&appid=(Your-API-Key)&unit=metric&cnt=3"); //Specify request destination
    int httpCode = http.GET(); //Send the request

    if (httpCode > 0) { //Check the returning code

      String payload = http.getString(); //Get the request response payload - the json object as a plain text
      DynamicJsonBuffer jsonBuffer; //define a json object buffer
      JsonObject& data = jsonBuffer.parseObject(payload); //try convert the plain text to an arduino - json object

      if (!data.success()) {
        Serial.println("parseObject() failed");
        return;
      }

      String temp = data["list"][2]["main"]["temp"];
      Serial.print("temp: ");
      Serial.println(temp);

      newTemp = temp.toInt() - 273; //converts String into int and converts Kelvin into C
      Serial.print("newTemp: ");
      Serial.print(newTemp);

    }
    http.end(); //Close connection
  }
  colorWipe(60);
  delay(2 * 60 * 1000);
}

void colorWipe(uint8_t wait) {

  updateScale();
  newTemp = newTemp * 2;

   if (newTemp >= -5) {
    for (int y = 60; y >= 58; y--) {
      strip.setPixelColor(y, 0, 0, 255);
      strip.show();
      delay(30);
    }}
   if (newTemp >= -4) {
    for (int y = 60; y >= 56; y--) {
      strip.setPixelColor(y, 0, 0, 255);
      strip.show();
      delay(wait);
    }}
   if (newTemp >= -3) {
    for (int y = 55; y >= 54; y--) {
      strip.setPixelColor(y, 0, 0, 255);
      strip.show();
      delay(wait);
    }}
   if (newTemp >= -2) {
    for (int y = 53; y >= 52; y--) {
      strip.setPixelColor(y, 0, 0, 255);
      strip.show();
      delay(wait);
    }}
   if (newTemp >= -1) {
    for (int y = 51; y >= 50; y--) {
      strip.setPixelColor(y, 0, 0, 255);
      strip.show();
      delay(wait);
     }}

  if (newTemp >= 0) {
    for (int i = 49; i >= 50-newTemp ; i--) {
      strip.setPixelColor(i, 255, 0, 0);
      strip.show();
      delay(wait);
    }}

}
void updateScale() {
  for (uint16_t y = 0; y < strip.numPixels(); y++) {
    strip.setPixelColor(y, 255, 255, 255);
    strip.show();
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
