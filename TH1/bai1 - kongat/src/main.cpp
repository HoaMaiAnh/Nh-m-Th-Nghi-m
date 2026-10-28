
#include <Arduino.h>

#define BUTTON_PIN 4
#define LED_PIN 2
#define LED_CHANNEL 0

void setup() {
    Serial.begin(115200);

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    ledcSetup(LED_CHANNEL, 5000, 8);
    ledcAttachPin(LED_PIN, LED_CHANNEL);
}

void loop() {
    for (int b = 0; b <= 255; b++) {
        ledcWrite(LED_CHANNEL, b);
        delay(10);
    }

    for (int b = 255; b >= 0; b--) {
        ledcWrite(LED_CHANNEL, b);
        delay(10);
    }

    if (digitalRead(BUTTON_PIN) == LOW) {
        Serial.println("Button pressed!");

        ledcWrite(LED_CHANNEL, 255);
        delay(100);
        ledcWrite(LED_CHANNEL, 0);
        delay(100);
    }
}
