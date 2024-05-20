#include <Arduino.h>
#include <EEPROM.h>
#include "transfer/TransferServer.h"
#include "settings/Settings.h"
#include "utils/wifi.hpp"
#include "ws_handlers.hpp"
// #include "ESPWebServer/ESPWebServer.h"


void setup() {
    Serial.begin(115200);
    EEPROM.begin(512);

    project_settings.read_from_storage();
    Settings::AppConfig* config = project_settings.getConfig();

//        createSoftAP(config->esp_wifi_ssid, config->esp_wifi_password);
//        web_server.begin();

    Serial.printf("Config field transfer_server_port: %d\n", config->transfer_server_port);
    Serial.printf("Config field transfer_server_host: %s\n", config->transfer_server_host.c_str());
    Serial.printf("Config field esp_wifi_name: %s\n", config->esp_wifi_ssid.c_str());
    Serial.printf("Config field esp_wifi_password: %s\n", config->esp_wifi_password.c_str());
    Serial.printf("Config field user_wifi_name: %s\n", config->user_wifi_ssid.c_str());
    Serial.printf("Config field user_wifi_password: %s\n", config->user_wifi_password.c_str());
    Serial.printf("Config field transfer_server_login: %s\n", config->transfer_server_login.c_str());
    Serial.printf("Config field transfer_server_password: %s\n", config->transfer_server_password.c_str());

    syncWIFIConnect(config->user_wifi_ssid, config->user_wifi_password);

    action_processor.self_register();
    register_ws_handlers();
    transfer_ws.begin("172.27.11.12", 8000, "/api/storage/smartplant/", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjM0MzIzNDQ5NDUsInV1aWQiOiJkNGYwNmJhZjlhNGQ0YjYyYjJmZTQwZDNkZjA3M2ExYyIsInRva2VuX3R5cGUiOiJhY2Nlc3MiLCJ1c2VyX2lkIjoxfQ.t5gxRzfAnXfex7d-UfFyaFXK-PU7laMWDfcpT2Ojicg");
}


void loop() {
    transfer_ws.loop();
//    web_server.serve();
}
