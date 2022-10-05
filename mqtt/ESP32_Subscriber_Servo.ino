#include <WiFi.h>
#include "PubSubClient.h"
#include "ESP32Servo.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.emqx.io";
int port = 1883;
String stMac;
char mac[50];
char clientId[50];

WiFiClient espClient;
PubSubClient client(espClient);

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  wifiConnect();

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
  stMac = WiFi.macAddress();
  stMac.replace(":", "_");
  Serial.println(stMac);
  
  client.setServer(mqttServer, port);
  client.setCallback(callback);

  servo1.attach(26);
  servo2.attach(27);
  servo3.attach(14);
  servo4.attach(12);
  servo5.attach(13);

  servo1.write(180);
  servo2.write(180);
  servo3.write(180);
  servo4.write(180);
  servo5.write(180);

}

void wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void mqttReconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    long r = random(1000);
    sprintf(clientId, "clientId-%ld", r);
    if (client.connect(clientId)) {
      Serial.print(clientId);
      Serial.println(" connected");
      client.subscribe("ArduMeka/openCVmqtt");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {

    if((char)message[0] == '1'){
      servo1.write(0);
    }
    else{
      servo1.write(180);
    }
    if((char)message[1] == '1'){
      servo2.write(0);
    }
    else{
      servo2.write(180);
    }
    if((char)message[2] == '1'){
      servo3.write(0);
    }
    else{
      servo3.write(180);
    }
    if((char)message[3] == '1'){
      servo4.write(0);
    }
    else{
      servo4.write(180);
    }
    if((char)message[4] == '1'){
      servo5.write(0);
    }
    else{
      servo5.write(180);
    }
}

void loop() {
  delay(10);
  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();
}

