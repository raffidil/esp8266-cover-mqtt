int readingUp;           // the current reading from the input pin
int readingDown;           // the current reading from the input pin
int previousUp = LOW;    // the previous reading from the input pin
int previousDown = LOW;    // the previous reading from the input pin
long buttonTime = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

void buttonLoop() {
  readingUp = digitalRead(buttonUp);
  readingDown = digitalRead(buttonDown);

  if (readingUp == HIGH && previousUp == LOW && millis() - buttonTime > debounce) {
    if (stateUp == HIGH && stateDown == HIGH) {
      digitalWrite(switchPin2, HIGH);
      stateUp = LOW;
      client.publish("/home/cover/state/", "UP");
    }
    else if (stateUp == LOW) {
      digitalWrite(switchPin2, LOW);
      digitalWrite(switchPin1, LOW);
      stateUp = HIGH;
      client.publish("/home/cover/state/", "STOP");
    }
    else if (stateDown == LOW) {
      digitalWrite(switchPin1, LOW);
      delay(500);
      digitalWrite(switchPin2, HIGH);
      stateUp = LOW;
      stateDown = HIGH;
      client.publish("/home/cover/state/", "DOWN");
    }
  }

  if (readingDown == HIGH && previousDown == LOW && millis() - buttonTime > debounce) {
    if (stateUp == HIGH && stateDown == HIGH) {
      digitalWrite(switchPin2, LOW);
      digitalWrite(switchPin1, HIGH);
      stateDown = LOW;
      client.publish("/home/cover/state/", "DOWN");
    }
    else if (stateDown == LOW) {
      digitalWrite(switchPin1, LOW);
      stateDown = HIGH;
      client.publish("/home/cover/state/", "STOP");

    }
    else if (stateUp == LOW) {
      digitalWrite(switchPin2, LOW);
      delay(500);
      digitalWrite(switchPin1, HIGH);
      stateUp = HIGH;
      stateDown = LOW;
      client.publish("/home/cover/state/", "STOP");

    }
  }


  previousUp = readingUp;
  previousDown = readingDown;

}


bool buttonSetup() {
  Serial.println("Button setup");
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);

  return true; // Task should be enabled
}
