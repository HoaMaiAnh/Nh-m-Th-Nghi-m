
#include <Arduino.h>

#define BUTTON_PIN 4
#define LED_PIN 2
#define LED_CHANNEL 0
#define DEBOUNCE_MS 50

volatile uint32_t debouncedCount = 0;
volatile uint32_t rawEdgeCount = 0;
volatile unsigned long lastInterruptTime = 0;

void IRAM_ATTR handleInterrupt() {
    rawEdgeCount++;

    unsigned long now = millis();

    if (now - lastInterruptTime >= DEBOUNCE_MS) {
        debouncedCount++;
        lastInterruptTime = now;
    }
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

    Serial.println("Demo Debounce started!");
}

void loop() {
    static uint32_t lastRaw = 0;
    static uint32_t lastDebounced = 0;

    uint32_t raw;
    uint32_t debounced;

    noInterrupts();
    raw = rawEdgeCount;
    debounced = debouncedCount;
    interrupts();

    if (debounced != lastDebounced) {
        lastDebounced = debounced;

        // Chop LED khi co lan nhan hop le
        ledcWrite(LED_CHANNEL, 255);
        delay(100);
        ledcWrite(LED_CHANNEL, 0);

        Serial.print("So lan nhan hop le: ");
        Serial.println(debounced);
    }

    if (raw != lastRaw) {
        lastRaw = raw;

        Serial.print("Tong so suon xuong: ");
        Serial.println(raw);
    }

    delay(5);
}
