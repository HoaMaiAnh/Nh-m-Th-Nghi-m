#include <Arduino.h>

hw_timer_t *hwTimer = NULL;

void IRAM_ATTR onTimer() {
    Serial.println("Timer ngat!");
}

void setup() {
    Serial.begin(115200);

    hwTimer = timerBegin(0, 80, true);
    timerAttachInterrupt(hwTimer, &onTimer, true);
    timerAlarmWrite(hwTimer, 100000, true);
    timerAlarmEnable(hwTimer);
}

void loop() {
}
