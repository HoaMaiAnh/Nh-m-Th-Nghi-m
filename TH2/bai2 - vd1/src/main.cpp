#include <Arduino.h>
#include "esp_sleep.h"

#define LED_PIN 2

RTC_DATA_ATTR int soLanThuc = 0;

int demThuong = 0;

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    delay(1000);

    soLanThuc++;
    demThuong++;

    Serial.println("\n===== DEMO DEEP SLEEP =====");

    Serial.print("So lan thuc day: ");
    Serial.println(soLanThuc);

    Serial.print("Bien thong thuong: ");
    Serial.println(demThuong);

    Serial.println("ESP32 se ngu trong 10 giay...");

    digitalWrite(LED_PIN, LOW);

    Serial.flush();

    esp_sleep_enable_timer_wakeup(10ULL * 1000000ULL);

    esp_deep_sleep_start();
}

void loop() {
}