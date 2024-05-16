#include <EEPROM.h>
#include "wifi/WIFIUtils.hpp"
#include "settings/Settings.h"
#include "ESPWebServer/ESPWebServer.h"

Settings::ProjectSettings project_settings;
ESPWeb::ESPWebServer server;


void setup() {
    Serial.begin(19200);
    EEPROM.begin(512);

    project_settings.read_from_storage();
    Settings::AppConfig* config = project_settings.getConfig();
    WIFIUtils::createSoftAP(config->esp_wifi_ssid, config->esp_wifi_password);

    server.setup(project_settings);
    server.begin();
}


void loop() {
    server.serve();
}
