

#include <WiFi.h>  
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <SPI.h>
#include "esp_wpa2.h"  //wpa2 library for connections to Enterprise networks
#include "Freenove_WS2812_Lib_for_ESP32.h"
#include <ArduinoJson.h>

// ***ADD VARIABLE BELOW INITIAL CONFIG***
//---- WiFi settings
#define EAP_IDENTITY \
  "jvincent@uni-muenster.de"  // if connecting from another corporation, use
                         // identity@organisation.domain in Eduroam
#define EAP_USERNAME \
  "jvincent@uni-muenster.de"             // oftentimes just a repeat of the identity
#define EAP_PASSWORD "Rinjer@123456$"  // your Eduroam password
const char* ssid = "eduroam";

//---- MQTT Broker settings
const char* mqtt_server = "71a024e3105e41d19034a12e80c4382b.s1.eu.hivemq.cloud"; // replace with your broker url
const char* mqtt_username = "iot-block-device";
const char* mqtt_password = "IoT-block-2025";
const int mqtt_port =8883;
const int soilMoisturePin = 3; // Pin 3 on the MCU S2 board
//===========================

WiFiClientSecure espClient;  
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

//============= RGB LED ============================
Freenove_ESP32_WS2812 led = Freenove_ESP32_WS2812(1, 1, 0, TYPE_GRB);

//============= Configure OUTPUT Here============================
const char* sensor1_topic= "garden/soil_moisture/status/Jerry";

//============= Config Command Here==============================
const char* command1_topic="command1";
const char* command2_topic="command2";

static const char *root_ca  =
    "-----BEGIN CERTIFICATE-----\n"
    "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\n"
    "-----END CERTIFICATE-----\n";

String topic;

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];
  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);
  //==============MESSAGE RECEPTION HERE (Uncomment to edit)====================
   if (String(topic) == String("set/led/rgb"))
   // Switch on the LED if an 1 was received as first character
    if (incommingMessage == String(1)) {
      setLED(0,255,0);   // Turn the LED green
    } else {
      setLED(255,255,0); // Blink yellow
      delay(100);
      setLED(255,0,0);  // Then turn the LED red
    }
    if (String(topic) == String("garden/soil_moisture/status/ishan")){
      setLED(255,0,0);
      delay(100);

    }
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "SenseBox-MCU-S2-Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      //SUBSCRIBE TO TOPIC HERE
     client.subscribe("set/led/rgb");
     client.subscribe("garden/soil_moisture/status/ishan");
     client.subscribe("test");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setLED(uint8_t r,uint8_t g,uint8_t b) {
  led.setLedColorData(0, r, g, b);
  led.show();
}
//============================================ADD USER VARIABLE HERE====================================



void setup() {
//=========================KEEP THIS==============================================================
  pinMode(2,OUTPUT);
  Serial.begin(115200);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);
  client.setCallback(callback);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
  while (!Serial) delay(1);
  espClient.setCACert(root_ca);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  delay(1000);
//==================================ADD USER SETUP HERE===================================================
  led.begin();
  led.setBrightness(30);  

}

void loop() {
//============== KEEP THIS =======================
   int sensorValue = analogRead(soilMoisturePin);
   int moisturePercentage = map(sensorValue, 7200, 2900, 0, 100); 
   StaticJsonDocument<200> jsonDoc; 
   jsonDoc["location"] = "Garden Bed 1"; 
   jsonDoc["moisture_percentage"] = moisturePercentage; 
   jsonDoc["status"] = (moisturePercentage < 60) ? "Low" : "Adequate"; 
   jsonDoc["value"] = moisturePercentage;

  // Convert JSON object to a string
  String jsonString;
  serializeJson(jsonDoc, jsonString);

   if(moisturePercentage<50){
    publishMessage(sensor1_topic, jsonString,true); 
    delay(6000);
    digitalWrite(2,HIGH);
    delay(1000);
    digitalWrite(2,LOW);
    delay(1000);
   }else{
     publishMessage(sensor1_topic, jsonString,true); 
     delay(50000); 
   }
   if (!client.connected()) reconnect();
      client.loop();
//============== ADD LOOP CODE HERE==============


//===============Publish MQTT MESSAGE=======================
  // String moistureMessage = "MoisturePercentage " + String(moisturePercentage) + "%";
  // publishMessage(sensor1_topic, String(moistureMessage),true); 
  // delay(50000);
}

//======================================= publising as string
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true))
      Serial.println("Message published ["+String(topic)+"]: "+payload);
}
