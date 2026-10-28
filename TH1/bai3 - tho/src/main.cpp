
#include <Arduino.h>

#define ADC_PIN 34
#define LED_PIN 18
#define LED_CHANNEL 0

void setup() {
    Serial.begin(115200);

    pinMode(ADC_PIN, INPUT);

    ledcSetup(LED_CHANNEL, 5000, 8);
    ledcAttachPin(LED_PIN, LED_CHANNEL);

    analogReadResolution(12);
}

void loop() {
    int tho = analogRead(ADC_PIN);

    int doSang = map(tho, 0, 4095, 0, 255);
    doSang = constrain(doSang, 0, 255);

    ledcWrite(LED_CHANNEL, doSang);

    Serial.print("ADC tho: ");
    Serial.print(tho);

    Serial.print(" | Do sang LED: ");
    Serial.println(doSang);

    delay(20);
}
