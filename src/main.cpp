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

    action_processor.self_register();
    WSHandlers::register_ws_handlers();
    transfer_ws.begin("172.27.11.12", 8000, "/api/storage/smartplant/", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJleHAiOjM0MzI1OTQ0NzQsInV1aWQiOiIxNGUwZjVmMzg2YTY0N2U4OWViNjg5Y2JlMzA4NWM4NCIsInRva2VuX3R5cGUiOiJhY2Nlc3MiLCJ1c2VyX2lkIjoxfQ.3poxMWjU00Ninn5UX9f8_CMdq219OAg37CMqxNLePa8");
}


void loop() {
//    web_server.serve();
    transfer_ws.loop();
    taskManager.update();
}
