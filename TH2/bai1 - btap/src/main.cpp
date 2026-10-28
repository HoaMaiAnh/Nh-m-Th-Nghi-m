#include <Arduino.h>

#define CHU_KY 100000UL
#define THOI_GIAN 3000

hw_timer_t *timer = NULL;

volatile unsigned long demT = 0;
volatile unsigned long truocT = 0;
volatile unsigned long tongT = 0;
volatile unsigned long maxT = 0;

unsigned long lech(unsigned long t) {
    return t > CHU_KY ? t - CHU_KY : CHU_KY - t;
}

void IRAM_ATTR onTimer() {
    unsigned long now = micros();

    if (truocT != 0) {
        unsigned long e = lech(now - truocT);
        tongT += e;
        if (e > maxT) maxT = e;
        demT++;
    }

    truocT = now;
}

void inKetQua(const char *ten, unsigned long dem,
              unsigned long tong, unsigned long maxLech) {
    Serial.println();
    Serial.println(ten);
    Serial.print("So lan: ");
    Serial.println(dem);

    if (dem > 0) {
        Serial.print("Lech trung binh: ");
        Serial.print(tong / (float)dem / 1000, 3);
        Serial.println(" ms");

        Serial.print("Lech lon nhat: ");
        Serial.print(maxLech / 1000.0, 3);
        Serial.println(" ms");
    }
}

void testDelay() {
    unsigned long dem = 0, tong = 0, maxLech = 0, truoc = 0;
    unsigned long start = millis();

    while (millis() - start < THOI_GIAN) {
        delay(100);
        unsigned long now = micros();

        if (truoc != 0) {
            unsigned long e = lech(now - truoc);
            tong += e;
            if (e > maxLech) maxLech = e;
            dem++;
        }
        truoc = now;
    }

    inKetQua("=== DELAY ===", dem, tong, maxLech);
}

void testMillis() {
    unsigned long dem = 0, tong = 0, maxLech = 0;
    unsigned long start = millis(), moc = millis(), truoc = 0;

    while (millis() - start < THOI_GIAN) {
        if (millis() - moc >= 100) {
            moc += 100;
            unsigned long now = micros();

            if (truoc != 0) {
                unsigned long e = lech(now - truoc);
                tong += e;
                if (e > maxLech) maxLech = e;
                dem++;
            }
            truoc = now;
        }
    }

    inKetQua("=== MILLIS ===", dem, tong, maxLech);
}

void testTimer() {
    demT = 0;
    truocT = 0;
    tongT = 0;
    maxT = 0;

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, CHU_KY, true);
    timerAlarmEnable(timer);

    delay(THOI_GIAN);

    timerAlarmDisable(timer);
    timerEnd(timer);
    timer = NULL;

    unsigned long dem, tong, maxLech;

    noInterrupts();
    dem = demT;
    tong = tongT;
    maxLech = maxT;
    interrupts();

    inKetQua("=== HARDWARE TIMER ===", dem, tong, maxLech);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("SO SANH 3 PHUONG PHAP");
    Serial.println("Chu ky muc tieu: 100 ms");

    testDelay();
    testMillis();
    testTimer();

    Serial.println("\n=== HOAN THANH ===");
}

void loop() {
}
