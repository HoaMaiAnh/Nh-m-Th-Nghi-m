
#include <Arduino.h>

#define BUTTON_PIN 4
#define LED_PIN 2
#define LED_CHANNEL 0

volatile bool buttonPressed = false;

void IRAM_ATTR handleInterrupt() {
    buttonPressed = true;
}

void setup() {
    Serial.begin(115200);

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    ledcSetup(LED_CHANNEL, 5000, 8);
    ledcAttachPin(LED_PIN, LED_CHANNEL);

    attachInterrupt(
        digitalPinToInterrupt(BUTTON_PIN),
        handleInterrupt,
        FALLING
    );
}

void loop() {
    static int brightness = 0;
    static int step = 1;

    // Nhấn nút thì chớp nhanh 1 lần
    if (buttonPressed) {
        noInterrupts();
        buttonPressed = false;
        interrupts();

        ledcWrite(LED_CHANNEL, 255);
        delay(80);

        ledcWrite(LED_CHANNEL, 0);
        delay(80);
    }

    // Tiếp tục fading
    brightness += step;

    if (brightness >= 255) {
        brightness = 255;
        step = -1;
    } else if (brightness <= 0) {
        brightness = 0;
        step = 1;
    }

    ledcWrite(LED_CHANNEL, brightness);
    delay(5);
}
