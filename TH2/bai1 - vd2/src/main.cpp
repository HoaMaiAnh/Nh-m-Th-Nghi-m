#include <Arduino.h>

#define LED1 2
#define ADC 34

unsigned long lanTruocLed = 0;
unsigned long lanTruocAdc = 0;
unsigned long lanTruocHeart = 0;

bool trangThaiLed = LOW;

void setup() {
    Serial.begin(115200);
    pinMode(LED1, OUTPUT);

    Serial.println("Bat dau bai 1 - Non-blocking millis()");
}

void loop() {
    unsigned long now = millis();

    // Cong viec 1: Nhay LED moi 500ms
    if (now - lanTruocLed >= 500) {
        lanTruocLed = now;

        trangThaiLed = !trangThaiLed;
        digitalWrite(LED1, trangThaiLed);
    }

    // Cong viec 2: Doc bien tro moi 2000ms
    if (now - lanTruocAdc >= 2000) {
        lanTruocAdc = now;

        int tho = analogRead(ADC);

        Serial.print("bienTro:");
        Serial.println(tho);
    }

    // Cong viec 3: In heartbeat moi 5000ms
    if (now - lanTruocHeart >= 5000) {
        lanTruocHeart = now;

        Serial.println("heartbeat");
    }
}
