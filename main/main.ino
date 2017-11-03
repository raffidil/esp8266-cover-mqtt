#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <TaskScheduler.h>
#include <stdlib.h>

void callback(char* topic, byte* payload, unsigned int length);

#define MQTT_SERVER "192.168.1.4"  //you MQTT IP Address
const char* ssid = "ssid";
const char* password = "pass";

int stateUp = HIGH;
int stateDown = HIGH;
const int switchPin1 = D6;
const int switchPin2 = D1;

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
  Serial.begin(115200);
  delay(200);
  Serial.println("enter main Setup()");
  delay(200);

  //initialize the switch as an output and set to LOW (off)
  pinMode(switchPin1, OUTPUT); // Relay Switch 1
  digitalWrite(switchPin1, LOW);

  pinMode(switchPin2, OUTPUT); // Relay Switch 2
  digitalWrite(switchPin2, LOW);

  //start the serial line for debugging

  //start wifi subsystem
  WiFi.begin(ssid, password);
  //attempt to connect to the WIFI network and then connect to the MQTT server
  reconnect();
  Serial.println("setup reconnect() done");

  //wait a bit before starting the main loop

  // Inint task schedueler
  runner.init();
  runner.addTask(buttonTask);
  Serial.println("button task added");
  runner.addTask(mqttTask);
  Serial.println("mqtt task added");
  delay(200);
  
  buttonTask.enable();
  Serial.println("buttonTask ENABLED");
  mqttTask.enable();
  Serial.println("mqttTask ENABLED");
  Serial.println("end of Setup()");
}


void loop() {
  runner.execute();
}


//fadeTask.enableIfNot();
























