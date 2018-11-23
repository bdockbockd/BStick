#include <FirebaseCloudMessaging.h>
#include <Firebase.h>
#include <FirebaseHttpClient.h>
#include <FirebaseArduino.h>
#include <FirebaseError.h>
#include <FirebaseObject.h>

#define TRIGGER_PIN_L  16
#define ECHO_PIN_L     5
#define TRIGGER_PIN_R  4
#define ECHO_PIN_R     0
#define TOUCH_INPUT 2
#define FIREBASE_HOST "nodemcu-a82de.firebaseio.com"
#define FIREBASE_KEY "mwlc2H2iLck6hKf5ybs02RDSMNoElMp8fjlMTqrS"

#include <ArduinoJson.h>

/**
   BasicHTTPClient.ino
    Created on: 24.05.2015
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <String>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
String key1;
String key2;
int indexPosition;
String type[] = {"1","2","3","4"};

String getKey(long distance, String text ){
//    Serial.println(text+": " + distance + "cm");
    if(distance <= 100 && distance > 50) {
       HTTPClient http;
//       USE_SERIAL.print("[HTTP] begin...\n");
        http.begin("http://172.20.10.6:5000/?key=0"); //HTTP
        Firebase.set("key0" ,true);
        http.GET();
        http.end();
        Serial.println("1 meters coming from " + text);

        return "0";
    } else if(distance <= 50 && distance >= 25){
       HTTPClient http;
        http.begin("http://172.20.10.6:5000/?key=1"); //HTTP
        Firebase.set("key1" ,true);
        http.GET();
        http.end();
      Serial.println("0.5 meters coming from " + text);
      return  "1";
    } else if(distance < 25){
        HTTPClient http;
        http.begin("http://172.20.10.6:5000/?key=2"); //HTTP
        Firebase.set("key2" ,true);
        http.GET();
        http.end();
        Serial.println("Meet the wall from the " + text);
        return "2";
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
    Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);

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

    long duration_R, distance_R;
    digitalWrite(TRIGGER_PIN_R, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(TRIGGER_PIN_R, HIGH);
    delayMicroseconds(10); // Added this line
    digitalWrite(TRIGGER_PIN_R, LOW);
    duration_R = pulseIn(ECHO_PIN_R, HIGH);
    distance_R = (duration_R/2) / 29.1;

    key1 = getKey(distance_L, "LEFT");
    key2 = getKey(distance_R, "RIGHT");
  
//    Serial.print("distance_left : ");
//    Serial.print(distance_L);
//    Serial.println(" cm");
//    
//    Serial.print("distance_right : ");
//    Serial.print(distance_R);
//    Serial.println(" cm");
//    HTTPClient http;
//
//    USE_SERIAL.print("[HTTP] begin...\n");
//    // configure traged server and url
//    //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
//    http.begin("http://172.20.10.6:5000/?key="+ key1); //HTTP
//        http.GET();
//
//    http.begin("http://172.20.10.6:5000/?key="+ key2); //HTTP
//        http.GET();
//
//
//    USE_SERIAL.print("[HTTP] GET...\n");
//    // start connection and send HTTP header
////    int httpCode = http.GET();
//
//    // httpCode will be negative on error
////    if (httpCode > 0) {
////      // HTTP header has been send and Server response header has been handled
////      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
//  
//      // file found at server
////      if (httpCode == HTTP_CODE_OK) {
////        String payload = http.getString();
////        USE_SERIAL.println(payload);
////      char toJ[27];
////      for(int i = 3; i < payload.length(); i++){
////        toJ[i-3] = payload[i];
////      } 
////      StaticJsonBuffer<400> jsonBuffer;
////      JsonObject& root = jsonBuffer.parseObject(toJ);
////      bool check = true;
////      const int s = root["status"];
////      Serial.println(s);
////      if (s == 2) {
////        check = false;
////        token = "6ERN";
////      }
////      if (check == true) {
////      const char *c = root["token"];
////      
////      token = "";
////      for(int i = 0; i < 4; i++){
////        token += *(c+i);
////      }
////      }
////      }
////    } else {
////      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
////    }
//
//    http.end();
//    }
//  delay(1000);
  
  }
}
