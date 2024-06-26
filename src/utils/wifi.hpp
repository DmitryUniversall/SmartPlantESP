#pragma once

#include <Arduino.h>
#include <WiFi.h>


void syncWIFIConnect(
        const String& ssid,
        const String& password,
        const int& tick = 100
) {
    WiFiClass::mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.print("[WIFIUtils]: Connecting to WIFI");

    while (WiFiClass::status() != WL_CONNECTED){
        Serial.print(".");
        delay(tick);
    }

    Serial.print("\n");
}


bool createSoftAP(
        const String& ssid,
        const String& password,
        const IPAddress& local_ip = IPAddress(192, 168, 2, 1),
        const IPAddress& gateway = IPAddress(192, 168, 2, 1),
        const IPAddress& subnet = IPAddress(255, 255, 255, 0)
) {
    return WiFi.softAP(ssid, password) && WiFi.softAPConfig(local_ip, gateway, subnet);
}

void stopWiFiAP() {
    WiFi.softAPdisconnect(true);
}
