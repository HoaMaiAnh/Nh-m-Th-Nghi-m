
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <ArduinoJson.h>

// ================= WIFI =================
const char* ssid = "VJU Office";
const char* password = "VJuOffice@2023";

// ================= MQTT =================
const char* mqtt_server = "172.16.6.164";
const int mqtt_port = 1883;

// Ten nhom
const char* groupName = "nhom08";

// MQTT Topics
const char* topic_data = "vju/iot/nhom08";
const char* topic_status = "vju/iot/nhom08/status";
const char* topic_output = "vju/iot/nhom08/output";

// ================= GPIO =================
#define BUTTON_PIN 4
#define LED_PIN 18

// ================= OBJECTS =================
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_AHTX0 aht;

// ================= VARIABLES =================
bool ledState = false;

int btnState = 0;

bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long lastMsg = 0;

const unsigned long debounceDelay = 50;
const unsigned long interval = 5000;

// ================= WIFI =================
void setupWiFi() {
    Serial.print("Connecting to WiFi");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

// ================= LED =================
void setLED(bool state) {
    ledState = state;

    digitalWrite(LED_PIN, ledState ? HIGH : LOW);

    Serial.print("LED: ");
    Serial.println(ledState ? "ON" : "OFF");
}

// ================= PUBLISH JSON =================
void publishData() {
    if (!client.connected()) {
        return;
    }

    sensors_event_t humidity, temperature;

    aht.getEvent(&humidity, &temperature);

    // Tao goi JSON
    JsonDocument doc;

    doc["nhom"] = groupName;
    doc["temp"] = temperature.temperature;
    doc["hum"] = humidity.relative_humidity;
    doc["led"] = ledState ? 1 : 0;
    doc["btn"] = btnState;

    char buffer[160];

    serializeJson(doc, buffer);

    // Gui len MQTT Broker
    if (client.publish(topic_data, buffer)) {
        Serial.println("MQTT Published:");
        Serial.println(buffer);
    } else {
        Serial.println("Publish failed!");
    }
}

// ================= MQTT CALLBACK =================
void callback(char* topic, byte* payload, unsigned int length) {
    String message = "";

    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    message.toLowerCase();

    Serial.print("Received [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(message);

    // Nhan lenh dieu khien LED
    if (String(topic) == topic_output) {

        if (message == "on") {
            setLED(true);
            publishData();
        }

        else if (message == "off") {
            setLED(false);
            publishData();
        }
    }
}

// ================= MQTT RECONNECT =================
void reconnect() {
    while (!client.connected()) {

        Serial.print("Connecting to MQTT...");

        String clientId = String(groupName) + "-esp32";

        // Last Will: neu ESP32 mat ket noi, Broker gui offline
        if (client.connect(
            clientId.c_str(),
            topic_status,
            1,
            true,
            "offline"
        )) {

            Serial.println("MQTT Connected!");

            // Bao trang thai online
            client.publish(topic_status, "online", true);

            // Dang ky nhan lenh dieu khien LED
            client.subscribe(topic_output);

            Serial.println("Subscribed to output topic!");

            // Gui du lieu ngay sau khi ket noi
            publishData();
        }

        else {
            Serial.print("MQTT Failed, state: ");
            Serial.println(client.state());

            delay(5000);
        }
    }
}

// ================= BUTTON =================
void checkButton() {

    bool reading = digitalRead(BUTTON_PIN);

    if (reading != lastButtonReading) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {

        if (reading != stableButtonState) {

            stableButtonState = reading;

            // 1 = dang nhan, 0 = dang tha
            btnState = (stableButtonState == LOW) ? 1 : 0;

            // Khi nhan nut thi dao trang thai LED
            if (btnState == 1) {
                setLED(!ledState);
            }

            Serial.print("Button: ");
            Serial.println(btnState);

            // Gui JSON ngay khi nut thay doi
            publishData();
        }
    }

    lastButtonReading = reading;
}

// ================= SETUP =================
void setup() {

    Serial.begin(115200);

    // GPIO
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    digitalWrite(LED_PIN, LOW);

    // I2C
    Wire.begin(21, 22);

    // AHT20
    if (!aht.begin()) {
        Serial.println("Cannot find AHT20!");
    } else {
        Serial.println("AHT20 ready!");
    }

    // WiFi
    setupWiFi();

    // MQTT
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

// ================= LOOP =================
void loop() {

    if (!client.connected()) {
        reconnect();
    }

    client.loop();

    // Kiem tra nut nhan
    checkButton();

    // Gui du lieu moi 5 giay
    if (millis() - lastMsg >= interval) {

        lastMsg = millis();

        publishData();
    }
}
