
#include <Arduino.h>
#include <DHT.h>
#include <Servo.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
Servo fan;

int temperature = 0;
int fanPin = 9;

void setup()
{
  Serial.begin(9600);

  dht.begin();
  fan.attach(fanPin);

  fan.write(0);
}

void loop()
{
  temperature = dht.readTemperature();

  if (isnan(temperature))
  {
    Serial.println("Failed to read from DHT22!");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  if (temperature >= 30)
  {

    for (int angle = 0; angle <= 180; angle++)
    {
      fan.write(angle);
      delay(10);
    }

    for (int angle = 180; angle >= 0; angle--)
    {
      fan.write(angle);
      delay(10);
    }

    Serial.println("Fan: ON");
  }
  else
  {

    fan.write(0);
    Serial.println("Fan: OFF");
  }
  delay(2000);
}
