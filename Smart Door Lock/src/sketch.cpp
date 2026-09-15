#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

#define SECURITY_SWITCH 4
#define DOOR_SWITCH 5
#define RED_LED 6
#define GREEN_LED 7
#define BUZZER 8

U8G2_SH1107_SEEED_128X128_1_HW_I2C oled(
  U8G2_R0,
  U8X8_PIN_NONE
);

bool doorOpen = false;
bool intrusionDetected = false;
bool securityActive = false;

void showNormalScreen() {

  oled.firstPage();

  do {

    oled.setFont(u8g2_font_6x10_tf);

    oled.drawStr(5, 15, "SMART DOOR");
    oled.drawStr(5, 28, "SECURITY SYSTEM");

    if (securityActive) {

      oled.drawStr(5, 48, "SECURITY: ACTIVE");

      if (doorOpen) {

        oled.drawStr(5, 68, "WARNING!");
        oled.drawStr(5, 83, "DOOR OPEN");

      } else {

        oled.drawStr(5, 68, "DOOR: SECURE");
        oled.drawStr(5, 83, "MONITORING...");
      }

    } else {

      oled.drawStr(5, 48, "SECURITY: OFF");
      oled.drawStr(5, 68, "SYSTEM NORMAL");
      oled.drawStr(5, 83, "DOOR UNLOCKED");
    }

  } while (oled.nextPage());
}

void showIntrusionWarning() {

  oled.firstPage();

  do {

    oled.setFont(u8g2_font_6x10_tf);

    oled.drawStr(15, 20, "!!! WARNING !!!");
    oled.drawStr(20, 45, "INTRUSION");
    oled.drawStr(30, 60, "DETECTED!");
    oled.drawStr(15, 80, "DOOR OPENED");
    oled.drawStr(5, 100, "SECURITY ACTIVE");

  } while (oled.nextPage());
}

void activateSecurity() {

  securityActive = true;

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);

  Serial.println("SECURITY SYSTEM ACTIVE");

  showNormalScreen();
}

void deactivateSecurity() {

  securityActive = false;
  intrusionDetected = false;

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  noTone(BUZZER);

  Serial.println("SECURITY SYSTEM OFF");

  showNormalScreen();
}

void activateIntrusionAlarm() {

  intrusionDetected = true;

  Serial.println("!!! INTRUSION ALERT !!!");

  tone(BUZZER, 1000);

  showIntrusionWarning();
}

void clearIntrusionAlarm() {

  intrusionDetected = false;

  noTone(BUZZER);

  Serial.println("INTRUSION CLEARED");

  showNormalScreen();
}

void setup() {

  Serial.begin(9600);

  pinMode(SECURITY_SWITCH, INPUT_PULLUP);
  pinMode(DOOR_SWITCH, INPUT_PULLUP);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  noTone(BUZZER);

  oled.begin();

  oled.firstPage();

  do {

    oled.setFont(u8g2_font_6x10_tf);

    oled.drawStr(25, 40, "SMART DOOR");
    oled.drawStr(25, 55, "SECURITY");
    oled.drawStr(30, 70, "SYSTEM");

  } while (oled.nextPage());

  delay(2000);

  Serial.println("SMART DOOR SECURITY SYSTEM");
  Serial.println("SYSTEM ONLINE");

  showNormalScreen();
}

void loop() {

  bool securitySwitch =
    digitalRead(SECURITY_SWITCH) == LOW;

  bool doorSwitch =
    digitalRead(DOOR_SWITCH) == LOW;

  if (securitySwitch != securityActive) {

    if (securitySwitch) {
      activateSecurity();
    } else {
      deactivateSecurity();
    }
  }

  if (securityActive) {

    if (doorSwitch && !doorOpen) {

      doorOpen = true;

      activateIntrusionAlarm();
    }

    if (!doorSwitch && doorOpen) {

      doorOpen = false;

      if (intrusionDetected) {

        clearIntrusionAlarm();

      } else {

        showNormalScreen();
      }
    }

  } else {

    if (doorOpen != doorSwitch) {

      doorOpen = doorSwitch;

      showNormalScreen();
    }
  }

  delay(100);
}