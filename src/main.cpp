#include <Arduino.h>
#include "transfer/TransferServer.h"
#include "settings/Settings.h"
#include "utils/wifi.hpp"
#include "ws_handlers.hpp"

using namespace TransferServer;


//void startWiFiAP() {
//    Serial.println("Starting WiFi Access Point...");
//    WiFi.softAP("ESP32_AP", "12345678"); // SSID and Password
//    Serial.println("WiFi Access Point Started");
//    digitalWrite(ledPin, HIGH); // Turn on LED to indicate AP is active
//    wifiAPActive = true;
//}
//
//void stopWiFiAP() {
//    Serial.println("Stopping WiFi Access Point...");
//    WiFi.softAPdisconnect(true);
//    Serial.println("WiFi Access Point Stopped");
//    digitalWrite(ledPin, LOW); // Turn off LED to indicate AP is inactive
//    wifiAPActive = false;
//}


void setup() {
    Serial.begin(115200);

    project_settings.read_from_storage();
    Settings::AppConfig* config = project_settings.getConfig();

    syncWIFIConnect(config->user_wifi_ssid, config->user_wifi_password);

    transfer_ws.set_transfer_response_handler(handle_transfer_response);
    transfer_ws.set_data_message_handler(handle_data_message);
    transfer_ws.begin("172.27.11.5", 8000, "/api/storage/smartplant/", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjM0MzIxMTMyMjEsInV1aWQiOiIzZTExYjljZWI1YzI0ODYxOTJiMmMzNzYzM2MyYzkzZiIsInRva2VuX3R5cGUiOiJhY2Nlc3MiLCJ1c2VyX2lkIjoxfQ.GypjsI0tsu8o2Cgsc-NxZJvkcf47Kwu-cljjwameyTE");
}


void loop() {
    transfer_ws.loop();
}
