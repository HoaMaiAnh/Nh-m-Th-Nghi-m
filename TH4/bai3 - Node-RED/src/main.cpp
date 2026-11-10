
#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_SSD1306.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

const char* SV_ID = "nhom08";
const char* broker = "172.16.6.164";

#define LED 14
#define BTN 4

WiFiClient esp;
PubSubClient mqtt(esp);
Adafruit_AHTX0 aht;
Adafruit_SSD1306 oled(128, 64, &Wire, -1);

String tele, cmd;
bool led = false;
unsigned long timer = 0;

void showOLED() {
    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setTextSize(1);
    oled.setCursor(0, 0);
    oled.println("TH4 - ESP32");
    oled.print("LED: ");
    oled.println(led ? "ON" : "OFF");
    oled.display();
}

void callback(char* topic, byte* payload, unsigned int len) {
    String msg;
    for (int i = 0; i < len; i++) msg += (char)payload[i];

    if (String(topic) == cmd) {
        if (msg == "1") led = true;
        if (msg == "0") led = false;

        digitalWrite(LED, led);
        showOLED();
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(LED, OUTPUT);
    pinMode(BTN, INPUT_PULLUP);

    Wire.begin(21, 22);
    aht.begin();
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(500);

    tele = "aet2022/" + String(SV_ID) + "/telemetry";
    cmd = "aet2022/" + String(SV_ID) + "/cmd/led";

    mqtt.setServer(broker, 1883);
    mqtt.setCallback(callback);

    while (!mqtt.connected()) {
        mqtt.connect(("ESP32-" + WiFi.macAddress()).c_str());
        delay(500);
    }

    mqtt.subscribe(cmd.c_str());
    showOLED();
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.reconnect();
        delay(500);
        return;
    }

    if (!mqtt.connected()) {
        while (!mqtt.connected()) {
            mqtt.connect(("ESP32-" + WiFi.macAddress()).c_str());
            delay(500);
        }
        mqtt.subscribe(cmd.c_str());
    }

    mqtt.loop();

    static bool oldBtn = HIGH;
    bool btn = digitalRead(BTN);

    if (oldBtn == HIGH && btn == LOW) {
        led = !led;
        digitalWrite(LED, led);
        showOLED();
    }
    oldBtn = btn;

    if (millis() - timer >= 2000) {
        timer = millis();

        sensors_event_t h, t;
        aht.getEvent(&h, &t);

        JsonDocument data;
        data["sv_id"] = SV_ID;
        data["temp"] = t.temperature;
        data["hum"] = h.relative_humidity;
        data["uptime"] = millis() / 1000;
        data["rssi"] = WiFi.RSSI();
        data["led"] = led;
        data["btn"] = !btn;

        char payload[256];
        serializeJson(data, payload);

        mqtt.publish(tele.c_str(), payload);
        Serial.println(payload);
    }
}
