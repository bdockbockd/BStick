//#include <FirebaseCloudMessaging.h>
//#include <Firebase.h>
//#include <FirebaseHttpClient.h>
//#include <FirebaseArduino.h>
//#include <FirebaseError.h>
//#include <FirebaseObject.h>

#define TRIGGER_PIN_L  16
#define ECHO_PIN_L     5
#define TRIGGER_PIN_R  4
#define ECHO_PIN_R     0
#define TOUCH_PIN      13
#define FIREBASE_HOST "nodemcu-a82de.firebaseio.com"
#define FIREBASE_KEY "mwlc2H2iLck6hKf5ybs02RDSMNoElMp8fjlMTqrS"

#include <ArduinoJson.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <String>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
String ret;
String key1;
String key2;
int indexPosition;
String type[] = {"1","2","3","4"};

String getKey(long distance, String text ){
//    Serial.println(text+": " + distance + "cm");
    if(distance <= 150 && distance > 100) {
       HTTPClient http;
       if(text == "LEFT" ) {
          ret = "0";
        } else {
          ret = "3";
        }
//       USE_SERIAL.print("[HTTP] begin...\n");
        http.begin("http://172.20.10.6:5000/?key="+ ret); //HTTP
//        Firebase.set("key0" ,true);
        http.GET();
        http.end();
        Serial.println("1.5 meters coming from " + text);
        if(text == "LEFT" ) {
          return "0";
        } else {
          return "3";
        }
    } else if(distance >= 50 && distance < 100){
       HTTPClient http;
        if(text == "LEFT" ) {
          ret = "6";
        } else {
          ret = "9";
        }
        http.begin("http://172.20.10.6:5000/?key="+ret); //HTTP
//        Firebase.set("key1" ,true);
        http.GET();
        http.end();
      Serial.println("1 meters coming from " + text);
      if(text == "LEFT" ) {
          return "6";
        } else {
          return "9";
        }
    } else if(distance < 50){
        HTTPClient http;
        if(text == "LEFT" ) {
          ret=  "12";
        } else {
          ret = "15";
        }
        http.begin("http://172.20.10.6:5000/?key="+ret); //HTTP
//        Firebase.set("key2" ,true);
        http.GET();
        http.end();
        Serial.println("Meet the wall from the " + text);
         if(text == "LEFT" ) {
          return "12";
        } else {
          return "15";
        }
    }
    return "";
}
void setup() {

  USE_SERIAL.begin(115200);
  pinMode(TRIGGER_PIN_L, OUTPUT);
  pinMode(ECHO_PIN_L, INPUT);
  pinMode(TRIGGER_PIN_R, OUTPUT);
  pinMode(ECHO_PIN_R, INPUT);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }
//
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Bkn", "plmlplml");


} 

void loop() {
  // wait for WiFi connection

  if ((WiFiMulti.run() == WL_CONNECTED)) {
//    Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);

    if(digitalRead(TOUCH_PIN) == HIGH){
      HTTPClient http;
      http.begin("http://maker.ifttt.com/trigger/SOS/with/key/pKyqgMHMZVjAuKOt4Wa-i"); //HTTP
      http.GET();
      http.end();
      Serial.println("Send SOS");
    }
    key1 = "";
    key2 = "";
    long duration_L, distance_L;
    digitalWrite(TRIGGER_PIN_L, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(TRIGGER_PIN_L, HIGH);
    delayMicroseconds(10); // Added this line
    digitalWrite(TRIGGER_PIN_L, LOW);
    duration_L = pulseIn(ECHO_PIN_L, HIGH);
    distance_L = (duration_L/2) / 29.1;
    Serial.println("LEFT:" + String(distance_L));

    long duration_R, distance_R;
    digitalWrite(TRIGGER_PIN_R, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(TRIGGER_PIN_R, HIGH);
    delayMicroseconds(10); // Added this line
    digitalWrite(TRIGGER_PIN_R, LOW);
    duration_R = pulseIn(ECHO_PIN_R, HIGH);
    distance_R = (duration_R/2) / 29.1;
    Serial.println("RIGHT :" +String(distance_R));


    key1 = getKey(distance_L, "LEFT");
    key2 = getKey(distance_R, "RIGHT");
    
  
  
  }
}
