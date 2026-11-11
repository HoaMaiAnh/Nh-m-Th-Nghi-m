
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// ================= WIFI =================

const char* ssid = "VJU Office";
const char* password = "VJuOffice@2023";

// ================= LED =================

#define LED_PIN 2

bool ledState = false;

// ================= WEB SERVER =================

WebServer server(80);

// ================= HTML PAGE =================

String getHTML() {

    String statusText = ledState ? "DANG BAT" : "DANG TAT";
    String statusColor = ledState ? "#22c55e" : "#ef4444";

    String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">

        <title>ESP32 LED Control</title>

        <style>
            body {
                font-family: Arial;
                text-align: center;
                background: #f1f5f9;
                margin: 0;
                padding: 30px;
            }

            .container {
                background: white;
                max-width: 400px;
                margin: auto;
                padding: 30px;
                border-radius: 15px;
                box-shadow: 0 4px 15px rgba(0,0,0,0.1);
            }

            h1 {
                color: #334155;
            }

            .status {
                font-size: 25px;
                font-weight: bold;
                margin: 25px;
            }

            button {
                width: 120px;
                padding: 15px;
                border: none;
                border-radius: 10px;
                color: white;
                font-size: 18px;
                cursor: pointer;
                margin: 10px;
            }

            .on {
                background: #22c55e;
            }

            .off {
                background: #ef4444;
            }

            button:hover {
                opacity: 0.8;
            }
        </style>
    </head>

    <body>

        <div class="container">

            <h1>ESP32 LED CONTROL</h1>

            <p>Trang web dieu khien LED</p>

            <p>Trang thai LED:</p>

            <div class="status" style="color: STATUS_COLOR;">
                STATUS_TEXT
            </div>

            <a href="/on">
                <button class="on">BAT LED</button>
            </a>

            <a href="/off">
                <button class="off">TAT LED</button>
            </a>

            <p>ESP32 HTTP Web Server</p>

        </div>

    </body>
    </html>
    )rawliteral";

    html.replace("STATUS_COLOR", statusColor);
    html.replace("STATUS_TEXT", statusText);

    return html;
}

// ================= ROUTES =================

// Trang chu
void handleRoot() {
    server.send(200, "text/html; charset=utf-8", getHTML());
}

// Bat LED
void handleLEDOn() {

    digitalWrite(LED_PIN, HIGH);
    ledState = true;

    Serial.println("LED: ON");

    server.send(200, "text/html; charset=utf-8", getHTML());
}

// Tat LED
void handleLEDOff() {

    digitalWrite(LED_PIN, LOW);
    ledState = false;

    Serial.println("LED: OFF");

    server.send(200, "text/html; charset=utf-8", getHTML());
}

// ================= SETUP =================

void setup() {

    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.println();
    Serial.println("ESP32 WEB SERVER");

    // Ket noi WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();

    Serial.println("WiFi Connected!");

    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());

    // Dang ky routes
    server.on("/", handleRoot);
    server.on("/on", handleLEDOn);
    server.on("/off", handleLEDOff);

    // Bat dau Web Server
    server.begin();

    Serial.println("HTTP Server Started!");
}

// ================= LOOP =================

void loop() {

    server.handleClient();

}
