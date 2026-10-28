#include <Arduino.h>

const uint32_t timerMs = 100;
uint32_t loopTick = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("Demo millis() trong loop()");
}

void updateLoopTimer() {
    if (millis() - loopTick < timerMs) {
        return;
    }

    loopTick += timerMs;

    if (millis() - loopTick >= timerMs) {
        loopTick = millis();
    }

    uint32_t now = micros();

    Serial.print("Thoi gian: ");
    Serial.print(now / 1000000.0, 3);
    Serial.println(" giay");
}

void loop() {
    updateLoopTimer();
}
