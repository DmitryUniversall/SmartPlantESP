#include <Arduino.h>
#include <EEPROM.h>
#include "transfer/TransferServer.h"
#include "settings/Settings.h"
#include "utils/tasks/tasks.h"
#include "utils/wifi.hpp"
#include "ws_handlers.h"
#include "sensors/sensors.h"
#include "controllers/controllers.h"
#include "web_server/ESPWebServer.h"
#include "sensors/sensors.h"

void setup() {
    Serial.begin(115200);
    EEPROM.begin(512);
    Sensors::setupSensors();
    Controllers::setupControllers();

    project_settings.read_from_storage();
    Settings::AppConfig* config = project_settings.getConfig();
    config->show();

//    createSoftAP(config->esp_wifi_ssid, config->esp_wifi_password);
//    web_server.begin();

    syncWIFIConnect(config->user_wifi_ssid, config->user_wifi_password);
//
    action_processor.self_register();
    WSHandlers::register_ws_handlers();
    String accessToken = TransferServer::getAccessToken(config->transfer_server_login.c_str(), config->transfer_server_password.c_str());
    Serial.printf("ACCESS TOKEN: %s", accessToken.c_str());
    transfer_ws.begin(config->transfer_server_host, config->transfer_server_port, "/api/storage/smartplant/", accessToken);
}


void loop() {
    taskManager.update();
//    web_server.serve();
    transfer_ws.loop();
}
