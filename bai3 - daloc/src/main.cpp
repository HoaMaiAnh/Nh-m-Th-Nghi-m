
#include <Arduino.h>

#define ADC_PIN 34
#define LED_PIN 18
#define LED_CHANNEL 0
#define N 8

int buf[N] = {0};
int idx = 0;
int count = 0;

void setup() {
    Serial.begin(115200);

    pinMode(ADC_PIN, INPUT);

    // Cau hinh PWM 5 kHz, do phan giai 8 bit
    ledcSetup(LED_CHANNEL, 5000, 8);
    ledcAttachPin(LED_PIN, LED_CHANNEL);

    analogReadResolution(12);

    Serial.println("Bai 3 - ADC va Moving Average");
}

int loc(int m) {
    buf[idx] = m;
    idx = (idx + 1) % N;

    if (count < N) {
        count++;
    }

    long tong = 0;

    for (int i = 0; i < count; i++) {
        tong += buf[i];
    }

    return tong / count;
}

void loop() {
    int tho = analogRead(ADC_PIN);
    int daLoc = loc(tho);

    int doSang = map(daLoc, 0, 4095, 0, 255);
    doSang = constrain(doSang, 0, 255);

    ledcWrite(LED_CHANNEL, doSang);

    Serial.print("ADC tho: ");
    Serial.print(tho);

    Serial.print(" | ADC da loc: ");
    Serial.print(daLoc);

    Serial.print(" | Do sang LED: ");
    Serial.println(doSang);

    delay(20);
}
