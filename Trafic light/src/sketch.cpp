
#include <Arduino.h>
#include <math.h>

int ldrPin = A0;
int ldrVal = 0;
int ledPin = 3;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  ldrVal = analogRead(ldrPin);

  Serial.print("LDR Value: ");
  Serial.println(ldrVal);

  if (ldrVal < 500) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(500);
}

