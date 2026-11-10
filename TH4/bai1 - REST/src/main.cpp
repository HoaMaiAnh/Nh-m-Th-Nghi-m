
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <ArduinoJson.h>

// ================= WIFI =================
const char* ssid = "VJU Office";
const char* password = "VJuOffice@2023";

// ================= STUDENT INFO =================
const char* SV_ID = "nhom08";

// ================= HARDWARE =================
#define SDA_PIN 21
#define SCL_PIN 22

// ================= OBJECTS =================
WebServer server(80);
Adafruit_AHTX0 aht;

// ================= SENSOR DATA =================
float temperature = 0;
float humidity = 0;

// Đọc dữ liệu cảm biến AHT20
bool readSensor() {
    sensors_event_t hum, temp;

    if (!aht.getEvent(&hum, &temp)) {
        return false;
    }

    temperature = temp.temperature;
    humidity = hum.relative_humidity;

    return true;
}

// ================= API: GET /api/sensor =================
void handleSensor() {

    Serial.println("\nClient requested /api/sensor");

    // Đọc cảm biến
    if (!readSensor()) {
        server.send(
            500,
            "application/json",
            "{\"error\":\"Sensor reading failed\"}"
        );
        return;
    }

    // Tạo JSON
    JsonDocument doc;

    doc["sv_id"] = SV_ID;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["uptime"] = millis() / 1000;
    doc["wifi_rssi"] = WiFi.RSSI();

    // Chuyển JSON thành chuỗi
    String jsonResponse;
    serializeJson(doc, jsonResponse);

    // Trả JSON về client
    server.send(
        200,
        "application/json",
        jsonResponse
    );

    // In kết quả lên Serial Monitor
    Serial.println("REST API Response:");
    Serial.println(jsonResponse);
}

// ================= ROOT PAGE =================
void handleRoot() {
    server.send(
        200,
        "text/plain",
        "ESP32 REST API is running!\n"
        "Access /api/sensor to get sensor data."
    );
}

// ================= SETUP =================
void setup() {

    Serial.begin(115200);

    delay(1000);

    Serial.println("\n===== TH4 - BAI 1 REST API =====");

    // Khởi tạo I2C
    Wire.begin(SDA_PIN, SCL_PIN);

    // Khởi tạo AHT20
    if (!aht.begin()) {
        Serial.println("ERROR: Cannot find AHT20!");

        while (true) {
            delay(1000);
        }
    }

    Serial.println("AHT20 initialized successfully!");

    // Kết nối WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");

    // In địa chỉ IP
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());

    // Đăng ký các route
    server.on("/", HTTP_GET, handleRoot);

    server.on(
        "/api/sensor",
        HTTP_GET,
        handleSensor
    );

    // Khởi động Web Server
    server.begin();

    Serial.println("HTTP Server started!");
    Serial.println("API endpoint: /api/sensor");

    Serial.println("===============================");
}

// ================= LOOP =================
void loop() {

    // Xử lý request từ client
    server.handleClient();
}
