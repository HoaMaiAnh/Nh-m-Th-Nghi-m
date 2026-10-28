#include <Arduino.h>
#include "esp_sleep.h"

RTC_DATA_ATTR int dem = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);

    dem++;

    Serial.println("ESP32 da thuc day!");
    Serial.print("So lan thuc day: ");
    Serial.println(dem);

    Serial.println("Ngu 10 giay...");
    Serial.flush();

    esp_sleep_enable_timer_wakeup(10ULL * 1000000ULL);
    esp_deep_sleep_start();
}

void loop() {
}