
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SV_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CH_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *characteristic;

void setup() {
    Serial.begin(115200);

    // Khoi tao BLE
    BLEDevice::init("ESP32-BLE");

    // Tao Server
    BLEServer *server = BLEDevice::createServer();

    // Tao Service
    BLEService *service = server->createService(SV_UUID);

    // Tao Characteristic co READ va WRITE
    characteristic = service->createCharacteristic(
        CH_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
    );

    // Gia tri ban dau
    characteristic->setValue("Hello World");

    // Bat dau Service
    service->start();

    // Bat dau quang ba BLE
    BLEDevice::getAdvertising()->start();

    Serial.println("BLE Server Started!");
    Serial.println("Device name: ESP32-BLE");
    Serial.println("Waiting for connection...");
}

void loop() {
    delay(2000);

    Serial.print("Current value: ");
    Serial.println(characteristic->getValue().c_str());
}
