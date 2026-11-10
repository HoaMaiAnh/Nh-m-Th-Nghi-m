
#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ================= WIFI =================
const char* ssid = "VJU Office";
const char* password = "VJuOffice@2023";

// ================= STUDENT INFO =================
const char* SV_ID = "nhom08";

// ================= MQTT BROKER =================
const char* mqtt_server = "172.16.6.164";
const int mqtt_port = 1883;

// ================= MQTT TOPIC =================
String topic_telemetry;

// ================= HARDWARE =================
#define SDA_PIN 21
#define SCL_PIN 22

// ================= OBJECTS =================
WiFiClient espClient;
PubSubClient mqtt(espClient);
Adafruit_AHTX0 aht;

// ================= SENSOR DATA =================
float temperature = 0;
float humidity = 0;

unsigned long lastPublish = 0;
const unsigned long publishInterval = 2000;

// ================= READ SENSOR =================
void readSensor() {
    sensors_event_t hum, temp;

    aht.getEvent(&hum, &temp);

    temperature = temp.temperature;
    humidity = hum.relative_humidity;
}

// ================= WIFI CONNECTION =================
void connectWiFi() {

    if (WiFi.status() == WL_CONNECTED) {
        return;
    }

    Serial.println("Connecting to WiFi...");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected!");

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

// ================= MQTT CONNECTION =================
void reconnectMQTT() {

    if (mqtt.connected()) {
        return;
    }

    // Client ID duy nhất theo MAC ESP32
    String clientId = "ESP32-";
    clientId += WiFi.macAddress();

    Serial.println("Connecting to MQTT Broker...");

    while (!mqtt.connected()) {

        if (mqtt.connect(clientId.c_str())) {

            Serial.println("MQTT connected!");

            Serial.print("Client ID: ");
            Serial.println(clientId);

        } else {

            Serial.print("MQTT connection failed. State: ");
            Serial.println(mqtt.state());

            Serial.println("Retry after 3 seconds...");
            delay(3000);
        }
    }
}

// ================= PUBLISH TELEMETRY =================
void publishTelemetry() {

    readSensor();

    // Tạo JSON
    JsonDocument doc;

    doc["sv_id"] = SV_ID;
    doc["temp"] = temperature;
    doc["hum"] = humidity;
    doc["uptime"] = millis() / 1000;
    doc["rssi"] = WiFi.RSSI();

    // Chuyển JSON thành chuỗi
    char payload[256];

    serializeJson(doc, payload, sizeof(payload));

    // Publish lên MQTT
    bool success = mqtt.publish(
        topic_telemetry.c_str(),
        payload
    );

    if (success) {

        Serial.println("\nMQTT Published:");
        Serial.print("Topic: ");
        Serial.println(topic_telemetry);

        Serial.print("Payload: ");
        Serial.println(payload);

    } else {

        Serial.println("MQTT Publish failed!");
    }
}

// ================= SETUP =================
void setup() {

    Serial.begin(115200);
    delay(1000);

    Serial.println("\n===== TH4 - BAI 2 MQTT PUBLISH =====");

    // Khởi tạo I2C
    Wire.begin(SDA_PIN, SCL_PIN);

    // Khởi tạo AHT20
    if (!aht.begin()) {

        Serial.println("ERROR: Cannot find AHT20!");

        while (true) {
            delay(1000);
        }
    }

    Serial.println("AHT20 initialized!");

    // Tạo topic theo SV_ID
    topic_telemetry =
        "aet2022/" + String(SV_ID) + "/telemetry";

    Serial.print("MQTT Topic: ");
    Serial.println(topic_telemetry);

    // Kết nối WiFi
    connectWiFi();

    // Cấu hình MQTT
    mqtt.setServer(mqtt_server, mqtt_port);

    // Kết nối MQTT
    reconnectMQTT();

    Serial.println("System ready!");
}

// ================= LOOP =================
void loop() {

    // Kiểm tra WiFi
    if (WiFi.status() != WL_CONNECTED) {
        connectWiFi();
    }

    // Kiểm tra MQTT
    if (!mqtt.connected()) {
        reconnectMQTT();
    }

    // Xử lý MQTT thường xuyên
    mqtt.loop();

    // Publish mỗi 2 giây
    unsigned long currentMillis = millis();

    if (currentMillis - lastPublish >= publishInterval) {

        lastPublish = currentMillis;

        publishTelemetry();
    }
}
