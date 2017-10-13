#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <TaskScheduler.h>
#include <stdlib.h>

void callback(char* topic, byte* payload, unsigned int length);

#define MQTT_SERVER "192.168.1.8"  //you MQTT IP Address
const char* ssid = "your_SSID";
const char* password = "psk";

int stateUp = HIGH;
int stateDown = HIGH;
const int switchPin1 = D0;
const int switchPin2 = D4;
const int buttonUp =  D3;
const int buttonDown =  TX;

char const* home_cover = "/home/cover/set/";



Scheduler runner;
WiFiClient wifiClient;
PubSubClient client(MQTT_SERVER, 1883, callback, wifiClient);

void mqttLoop();
void buttonLoop();

bool buttonSetup();

Task buttonTask(50, TASK_FOREVER, &buttonLoop, NULL, false, &buttonSetup);
Task mqttTask(100, TASK_FOREVER, &mqttLoop, NULL, false);

void setup() {
  //initialize the switch as an output and set to LOW (off)
  pinMode(switchPin1, OUTPUT); // Relay Switch 1
  digitalWrite(switchPin1, LOW);

  pinMode(switchPin2, OUTPUT); // Relay Switch 2
  digitalWrite(switchPin2, LOW);


  ArduinoOTA.setHostname("My Arduino WEMO"); // A name given to your ESP8266 module when discovering it as a port in ARDUINO IDE
  ArduinoOTA.begin(); // OTA initialization

  //start the serial line for debugging
  Serial.begin(230400);
  delay(100);

  //start wifi subsystem
  WiFi.begin(ssid, password);
  //attempt to connect to the WIFI network and then connect to the MQTT server
  reconnect();

  //wait a bit before starting the main loop
  delay(2000);

  // Inint task schedueler
  runner.init();
  runner.addTask(buttonTask);
  runner.addTask(mqttTask);

  mqttTask.enable();
  buttonTask.enable();
}


void loop() {
  runner.execute();
}



























