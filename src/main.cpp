#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"



const int led_Pin_1 = 12;
const int led_Pin_2 = 13;
const int led_Pin_3 = 14;

bool braffboss [8][3]={  {false,false,false},
                        {false,false,true },
                        {false,true ,false},
                        {false,true ,true },
                        {true ,false,false},
                        {true ,false,true },
                        {true ,true ,false},
                        {true ,true ,true },};

void setup() {
  Serial.begin(9600);
  pinMode(led_Pin_1, OUTPUT);
  pinMode(led_Pin_2, OUTPUT);
  pinMode(led_Pin_3, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  Serial.println("");

  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    String http_response;


    String url = "https://" + String(host) + "/api/state";
    http.begin(url);

     for (int i = 0 ; i < 8;i++){
      
      // Specify content-type header
      http.addHeader("X-API-Key",API_KEY);
      http.addHeader("Content-Type", "application/json");

      StaticJsonDocument<1024> doc;
      String httpRequestData;

      // Serialise JSON object into a string to be sent to the API
    

      doc["light_switch_1"] = braffboss[i][0];
      doc["light_switch_2"] = braffboss[i][1];
      doc["light_switch_3"] = braffboss[i][2];

      digitalWrite(led_Pin_1,braffboss[i][0]);
      digitalWrite(led_Pin_2,braffboss[i][1]);
      digitalWrite(led_Pin_3,braffboss[i][2]);


      // convert JSON document, doc, to string and copies it into httpRequestData
      serializeJson(doc, httpRequestData);

      // Send HTTP PUT request
      int httpResponseCode = http.PUT(httpRequestData);
      String http_response;

      // check reuslt of PUT request. negative response code means server wasn't reached
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        Serial.print("HTTP Response from server: ");
        http_response = http.getString();
        Serial.println(http_response);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      delay(2000);
    }
    http.end();

   }
  else {
    return;
  }
}
