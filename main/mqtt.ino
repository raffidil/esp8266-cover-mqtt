

void mqttLoop() {
  //reconnect if connection is lost
  if (!client.connected() || WiFi.status() != WL_CONNECTED) {
    if (!client.connected())
      Serial.println("mqtt client is not connected");
    if (WiFi.status() != WL_CONNECTED)
      Serial.println("WiFi is not connected");
    reconnect();
    Serial.println("reconnect() done");
    mqttTask.delay(2000);
  }

  //maintain MQTT connection
  client.loop();

  //MUST delay to allow ESP8266 WIFI functions to run
  delay(10);


}

